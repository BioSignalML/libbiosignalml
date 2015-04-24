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

//#include <H5Cpp.h>

#include <string>


namespace bsml {

  namespace HDF5 {

    class Clock : public bsml::Clock
    /*============================*/
    {
      TYPED_OBJECT(Clock, BSML::SampleClock)

     public:
      Clock(const std::string &uri, const std::string &units) ;
      size_t extend(const double *times, const size_t length) { return 1 ; } // TEMP
      } ;


    class Signal : public bsml::Signal
    /*==============================*/
    {
      TYPED_OBJECT(Signal, BSML::Signal)

     public:
      Signal(const std::string &uri, const std::string &units, Clock *clock=nullptr) ;
      size_t extend(const double *points, const size_t length) { return 1 ; } // TEMP
      } ;


    class SignalVector : public bsml::SignalVector<HDF5::Signal>
    /*--------------------------------------------------------*/
    {
     public:
      size_t extend(const double *points, const size_t length) { return 1 ; } // TEMP
      } ;


    class Recording : public bsml::Recording
    /*====================================*/
    {
      TYPED_OBJECT(Recording, BSML::Recording)
      RESTRICTION(format, Format::HDF5)

      PROPERTY_OBJECT_RSET(signals, BSML::recording, Signal) // Override...

     public:
      Recording(const std::string &uri, const std::string &filename) ;

      void close(void) ;

      Clock *new_clock(const std::string &uri, const std::string &units, double *times = nullptr) ;

      HDF5::Signal *new_signal(const std::string &uri, const std::string &units) ;

      HDF5::SignalVector new_signal(const std::vector<const std::string> &uris,
                                    const std::vector<const std::string> &units,
                                    Clock *clock = nullptr) ;

// Variants of new_signal() with rate/period (== regular Clock)




     private:
  //    H5::H5File h5 ;
      bool closed ;
      } ;

    } ;

  } ;

#endif
