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

#include "model.h"
#include "../model.h"

#include <string>
#include <stdexcept>


namespace bsml {

  namespace HDF5 {

    class File ;        // Declare forward
    class Dataset ;     // Declare forward
    class ClockData ;   // Declare forward
    class SignalData ;  // Declare forward


    class IOError : public std::runtime_error
    /*=====================================*/
    {
     public:
      IOError(const std::string &msg) : std::runtime_error(msg) { }
      } ;

    class Exception : public std::runtime_error
    /*=======================================*/
    {
     public:
      Exception(const std::string &msg) : std::runtime_error(msg) { }
      } ;


    class Clock : public data::Clock
    /*============================*/
    {
      TYPED_OBJECT(Clock, BSML::SampleClock)

     public:
      Clock(const std::string &uri, const std::string &units) ;
      void extend(const double *times, const size_t length) ;

     private:
      ClockData *m_data ;
      friend class Recording ;
      } ;


    class Signal : public data::Signal
    /*==============================*/
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


    class SignalArray : public data::SignalArray<HDF5::Signal>
    /*======================================================*/
    {
     public:
      void extend(const double *points, const size_t length) ;
      int index(const std::string &uri) const ;

     private:
      SignalData *m_data ;
      friend class Recording ;
      } ;


    class Recording : public data::Recording
    /*====================================*/
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
