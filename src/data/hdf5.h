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

#ifndef BSML_HDF5_H
#define BSML_HDF5_H

#include "biosignalml_export.h"
#include "data.h"
#include "../biosignalml.h"

#include <string>


namespace bsml {

  namespace HDF5 {

    const BIOSIGNALML_EXPORT std::string BSML_H5_MAJOR   = "1" ;
    const BIOSIGNALML_EXPORT std::string BSML_H5_MINOR   = "0" ;
    const BIOSIGNALML_EXPORT std::string BSML_H5_VERSION = std::string("BSML ") + BSML_H5_MAJOR + "." + BSML_H5_MINOR ;

    enum H5Compression {
      BSML_H5_COMPRESS_NONE,
      BSML_H5_COMPRESS_GZIP,
      BSML_H5_COMPRESS_SZIP
      } ;


    class File ;        // Declare forward
    class Dataset ;     // Declare forward
    class ClockData ;   // Declare forward
    class SignalData ;  // Declare forward

    class Recording ;   // VS2013 needs class visible for friendship...

    class BIOSIGNALML_EXPORT IOError : public data::Exception
    /*-----------------------------------------------------*/
    {
     public:
      IOError(const std::string &msg) : bsml::data::Exception(msg) { }
      } ;

    class BIOSIGNALML_EXPORT Exception : public data::Exception
    /*-------------------------------------------------------*/
    {
     public:
      Exception(const std::string &msg) : bsml::data::Exception(msg) { }
      } ;


    class BIOSIGNALML_EXPORT Clock : public data::Clock
    /*-----------------------------------------------*/
    {
      TYPED_OBJECT(Clock, BSML::SampleClock)

     public:
      Clock(const std::string &uri, const std::string &units) ;
      void extend(const double *times, const size_t length) ;

     private:
      ClockData *m_data ;
      friend class Recording ;
      } ;


    class BIOSIGNALML_EXPORT Signal : public data::Signal
    /*-------------------------------------------------*/
    {
      TYPED_OBJECT(Signal, BSML::Signal)

     public:
      Signal(const std::string &uri, const std::string &units, double rate) ;
      Signal(const std::string &uri, const std::string &units, Clock *clock) ;
      void extend(const double *points, const size_t length) ;

     private:
      SignalData *m_data ;
      friend class Recording ;
      } ;


    class BIOSIGNALML_EXPORT SignalArray : public data::SignalArray<HDF5::Signal>
    /*-------------------------------------------------------------------------*/
    {
     public:
      void extend(const double *points, const size_t length) ;
      int index(const std::string &uri) const ;

     private:
      SignalData *m_data ;
      friend class Recording ;
      } ;


    class BIOSIGNALML_EXPORT Recording : public data::Recording
    /*-------------------------------------------------------*/
    {
      TYPED_OBJECT(Recording, BSML::Recording)
      RESTRICT_NODE(format, Format::HDF5)

      PROPERTY_OBJECT_RSET(signals, BSML::recording, Signal) // Override...
      PROPERTY_OBJECT_RSET(clockss, BSML::recording, Clock)  // Override...

     public:
      Recording(const std::string &uri, const std::string &filename) ;

      void close(void) ;

      Clock *new_clock(const std::string &uri, const std::string &units,
                       double *times = nullptr, size_t datasize=0) ;

      Signal *new_signal(const std::string &uri, const std::string &units, double rate) ;
      Signal *new_signal(const std::string &uri, const std::string &units, Clock *clock) ;

      SignalArray *new_signalarray(const std::vector<const std::string> &uris,
                                   const std::vector<const std::string> &units,
                                   double rate) ;
      SignalArray *new_signalarray(const std::vector<const std::string> &uris,
                                   const std::vector<const std::string> &units,
                                   Clock *clock) ;

// Variants of new_signal() with rate/period (== regular Clock)

     private:
      File *m_file ;
      std::set<Dataset *> datasets ;
      } ;

    } ;

  } ;

#endif
