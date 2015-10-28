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
      std::string name(void) ;
      size_t size(void) const ;
      void extend(const double *data, intmax_t length, int nsignals) ;
      std::vector<double> read(size_t pos, intmax_t length) ;

     protected:
      std::string m_uri ;
      H5::DataSet m_dataset ;
      hobj_ref_t m_reference ;
      int m_index ;

     private:
      int64_t clock_size(void) ;
      } ;


    class BIOSIGNALML_EXPORT ClockData : public Dataset
    /*-----------------------------------------------*/
    {
     public:
      ClockData() ;
      ClockData(const std::string &uri, const DatasetRef &ds) ;

      static std::shared_ptr<ClockData> get_clock(const std::string &uri, const DatasetRef &ds) ;
      } ;



    class BIOSIGNALML_EXPORT SignalData : public Dataset
    /*------------------------------------------------*/
    {
     public:
      SignalData() ;
      SignalData(const std::string &uri, const DatasetRef &ds) ;

      static std::shared_ptr<SignalData> get_signal(const std::string &uri, const DatasetRef &ds) ;
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

      std::shared_ptr<SignalData> create_signal(const std::string &uri, const std::string &units,
        const double *data=nullptr, size_t datasize=0, std::vector<hsize_t> datashape=std::vector<hsize_t>(),
        double gain=1.0, double offset=0.0, double rate=0.0, std::shared_ptr<ClockData> clock=nullptr) ;
      std::shared_ptr<SignalData> create_signal(const std::vector<std::string> &uris,
        const std::vector<std::string> &units,
        const double *data=nullptr, size_t datasize=0,
        double gain=1.0, double offset=0.0, double rate=0.0, std::shared_ptr<ClockData> clock=nullptr) ;

      std::shared_ptr<SignalData> get_signal(const std::string &uri) ;
      std::list<std::shared_ptr<SignalData>> get_signals(void) ;

      std::shared_ptr<ClockData> create_clock(const std::string &uri, const std::string &units,
        double rate, const double *data=nullptr, size_t datasize=0) ;
      std::shared_ptr<ClockData> create_clock(const std::string &uri, const std::string &units,
        const double *data, size_t datasize) ;

      std::shared_ptr<ClockData> get_clock(const std::string &uri) ;
      std::list<std::shared_ptr<ClockData>> get_clocks(void) ;

      void store_metadata(const std::string &, const std::string &) ;
      std::pair<std::string, std::string> get_metadata(void) ;

     private:
      DatasetRef get_dataref(const std::string &uri, const std::string &prefix) ;
      DatasetRef create_dataset(const std::string &group, int rank,
        hsize_t *shape, hsize_t *maxshape, const double *data) ;

      void set_signal_attributes(const H5::DataSet &dset, double gain=1.0, double offset=0.0,
        double rate=0.0, const std::string &timeunits="", const std::shared_ptr<ClockData> clock=nullptr) ;
      std::shared_ptr<ClockData> check_timing(double rate, const std::string &uri, size_t npoints) ;

      H5::H5File m_h5file ;
      std::string m_uri ;
      bool m_closed ;
      } ;

    } ;

  } ;

#endif
