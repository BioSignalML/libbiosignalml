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

#ifndef BSML_DATA_H
#define BSML_DATA_H

#include "recording.h"
#include "biosignalml_export.h"

#include <stdexcept>


namespace bsml {

  namespace data {

    class BIOSIGNALML_EXPORT Exception : public std::runtime_error
    /*----------------------------------------------------------*/
    {
     public:
      Exception(const std::string &msg) : std::runtime_error(msg) { }
      } ;

    class BIOSIGNALML_EXPORT Clock : public bsml::Clock
    /*-----------------------------------------------*/
    {
      TYPED_OBJECT(Clock, BSML::SampleClock)
  
     public:
      virtual void extend(const double *times, const size_t length) { }
      } ;


    class BIOSIGNALML_EXPORT Signal : public bsml::Signal
    /*-------------------------------------------------*/
    {
      TYPED_OBJECT(Signal, BSML::Signal)

     public:
      virtual void extend(const double *points, const size_t length) { }
      } ;


    template<class SIGNAL = Signal>
    class BIOSIGNALML_EXPORT SignalArray : public std::vector<SIGNAL *>
    /*---------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL>::value, "SIGNAL must be derived from Signal") ;

     public:
      virtual void extend(const double *points, const size_t length) { }
      virtual int index(const std::string &uri) const { return -1 ; }
      } ;


    class BIOSIGNALML_EXPORT Recording : public bsml::Recording
    /*-------------------------------------------------------*/
    {
      TYPED_OBJECT(Recording, BSML::Recording)

     public:
      template<class SIGNAL=Signal, class CLOCK=Clock>
      SignalArray<SIGNAL> *new_signalarray(const std::vector<const std::string> &uris,
      /*----------------------------------------------------------------------------*/
                                           const std::vector<const std::string> &units,
                                           double rate)
      {
        return create_signalarray<SignalArray<SIGNAL>, SIGNAL, CLOCK>(uris, units, rate, nullptr) ;
        }

      template<class SIGNAL=Signal, class CLOCK=Clock>
      SignalArray<SIGNAL> *new_signalarray(const std::vector<const std::string> &uris,
      /*----------------------------------------------------------------------------*/
                                           const std::vector<const std::string> &units,
                                           CLOCK *clock)
      {
        return create_signalarray<SignalArray<SIGNAL>, SIGNAL, CLOCK>(uris, units, 0.0, clock) ;
        }


     protected:
      template<class SIGNALARRAY, class SIGNAL, class CLOCK>
      SIGNALARRAY *create_signalarray(const std::vector<const std::string> &uris,
      /*-----------------------------------------------------------------------*/
                                      const std::vector<const std::string> &units,
                                      double rate, CLOCK *clock)
      {
        assert(uris.size() == units.size()) ;  // Lengths of `uris` and `units` are different
        auto signals = new SIGNALARRAY() ;
        for (int n = 0 ;  n < uris.size() ;  ++n) {
          if (clock == nullptr) signals->push_back(this->new_signal<SIGNAL>(uris[n], units[n], rate)) ;
          else                  signals->push_back(this->new_signal<SIGNAL, CLOCK>(uris[n], units[n], clock)) ;
          }
        return signals ;
        }

      } ;


    } ;

  } ;


#endif
