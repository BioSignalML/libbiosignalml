/******************************************************************************
 *                                                                            *
 *  BioSignalML Management in C++                                             *
 *                                                                            *
 *  Copyright (c) 2010-2015  David Brooks                                     *
 *                                                                            *
 *  Licensed under the Apache License, Version 2.0 (the "License");           *
 *  you may not use this file except in compliance with the License.          *
 *  You may obtain a copy of the License at                                   *
 *                                                                            *
 *      http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                            *
 *  Unless required by applicable law or agreed to in writing, software       *
 *  distributed under the License is distributed on an "AS IS" BASIS,         *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *  See the License for the specific language governing permissions and       *
 *  limitations under the License.                                            *
 *                                                                            *
 ******************************************************************************/

#ifndef BSML_HDF5IMPL_H
#define BSML_HDF5IMPL_H

#include <biosignalml/biosignalml_export.h>

#include <H5Cpp.h>

#include <list>
#include <vector>


namespace bsml {

  namespace HDF5 {

#define BSML_H5_DEFAULT_DATATYPE    H5::PredType::IEEE_F64LE
#define BSML_H5_DEFAULT_COMPRESSION BSML_H5_COMPRESS_GZIP
#define BSML_H5_CHUNK_BYTES         (128*1024)


    class DatasetRef : public std::pair<H5::DataSet, hobj_ref_t>
    /*--------------------------------------------------------*/
    {
     public:
      DatasetRef() = default ;
      DatasetRef(H5::DataSet ds, hobj_ref_t t) : std::pair<H5::DataSet, hobj_ref_t>(ds, t) { }
      inline bool valid(void) {
        return this->first.getId() >= 0 ;
        }
      } ;


    class BIOSIGNALML_EXPORT Dataset
    /*----------------------------*/
    {
     public:

      Dataset() ;
      Dataset(const std::string &uri, const DatasetRef &dataref) ;
      virtual ~Dataset() ;

      void close(void) ;
      H5::DataSet get_dataset(void) const ;
      hobj_ref_t get_reference(void) const ;
      size_t size(void) const ;
      std::string name(void) const ;
      void extend(const double *data, ssize_t length, int nsignals) ;
      std::vector<double> read(size_t pos, ssize_t length) ;

     protected:
      std::string m_uri ;
      H5::DataSet m_dataset ;
      hobj_ref_t m_reference ;
      int m_index ;

     private:
      int64_t clock_size(void) ;
      } ;


    class IndexCache
    /*------------*/
    {
     public:
      IndexCache(const ClockData *clock, const bool right) ;
      virtual ~IndexCache() = default ;

      //! Insert `(t, index)` before `pos`.
      void insert(const size_t pos, const double t, const ssize_t index) ;
      //! Find first position in cache with time greater than or equal to `t`.
      //! Returns `-1` if no such time exists.
      ssize_t find(const double t) ;
      //! Search `clock` for the index with time `t` and if not an exact match
      //! return the index to the immediate left (or right if `right` set). Return
      //! `-1` (`clock->size()`) if none to the left (right).
      ssize_t search(size_t start, size_t end, const double t) const ;

     private:
      const ClockData *m_clock ;
      const bool m_right ;
      std::vector<ssize_t> m_indexes ;
      std::vector<double> m_times ;
      } ;


    class BIOSIGNALML_EXPORT ClockData : public Dataset
    /*-----------------------------------------------*/
    {
     public:
      ClockData() ;
      ClockData(const std::string &uri, const DatasetRef &ds) ;

      using Ptr = std::shared_ptr<ClockData> ;
      static Ptr get_clock(const std::string &uri, const DatasetRef &ds) ;

      //! Return the position of the first time point that is
      //! not less than `t`. i.e. largest `n` such that `time(n) <= t`.
      //! Returns -1 if no such `n` exists.
      ssize_t index(const double t) ;
      //! Return the position of the first time point that is
      //! not more than `t`. i.e. smallest `n` such that `t <= time(n)`.
      //! Returns `size()` if no such `n` exists.
      size_t index_right(const double t) ;

     private:
      friend class Clock ;
      friend class IndexCache ;
      double read_time(size_t pos) const ;

      IndexCache m_leftcache ;
      IndexCache m_rightcache ;
      } ;


    class BIOSIGNALML_EXPORT SignalData : public Dataset
    /*------------------------------------------------*/
    {
     public:
      SignalData() ;
      SignalData(const std::string &uri, const DatasetRef &ds) ;

      using Ptr = std::shared_ptr<SignalData> ;
      static Ptr get_signal(const std::string &uri, const DatasetRef &ds) ;
      int signal_count(void) ;
      } ;


    class BIOSIGNALML_EXPORT File
    /*-------------------------*/
    {
     public:
      File(H5::H5File h5file, const std::string &uri) ;
      ~File(void) ;

      static File *create(const std::string &uri, const std::string &fname, bool replace=false) ;
      static File *open(const std::string &fname, bool readonly=false) ;
      void close(void) ;
      const std::string get_uri(void) const ;

      SignalData::Ptr create_signal(const std::string &uri, const std::string &units,
        const double *data=nullptr, size_t datasize=0, std::vector<hsize_t> datashape=std::vector<hsize_t>(),
        double gain=1.0, double offset=0.0, double rate=0.0, ClockData::Ptr clock=nullptr) ;
      SignalData::Ptr create_signal(const std::vector<std::string> &uris,
        const std::vector<std::string> &units,
        const double *data=nullptr, size_t datasize=0,
        double gain=1.0, double offset=0.0, double rate=0.0, ClockData::Ptr clock=nullptr) ;

      SignalData::Ptr get_signal(const std::string &uri) ;
      std::list<SignalData::Ptr> get_signals(void) ;

      ClockData::Ptr create_clock(const std::string &uri, const std::string &units,
        double rate, const double *data=nullptr, size_t datasize=0) ;
      ClockData::Ptr create_clock(const std::string &uri, const std::string &units,
        const double *data, size_t datasize) ;

      ClockData::Ptr get_clock(const std::string &uri) ;
      std::list<ClockData::Ptr> get_clocks(void) ;

      void store_metadata(const std::string &, const std::string &) ;
      std::pair<std::string, std::string> get_metadata(void) ;

     private:
      DatasetRef get_dataref(const std::string &uri, const std::string &prefix) ;
      DatasetRef create_dataset(const std::string &group, int rank,
        hsize_t *shape, hsize_t *maxshape, const double *data) ;

      void set_signal_attributes(const H5::DataSet &dset, double gain=1.0, double offset=0.0,
        double rate=0.0, const std::string &timeunits="", const ClockData::Ptr &clock=nullptr) ;
      ClockData::Ptr check_timing(double rate, const std::string &uri, size_t npoints) ;

      H5::H5File m_h5file ;
      std::string m_uri ;
      bool m_closed ;
      } ;

    } ;

  } ;

#endif
