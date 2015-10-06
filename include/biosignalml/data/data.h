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

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/recording.h>

#include <stdexcept>


namespace bsml {

  namespace data {

    class Exception : public std::runtime_error
    /*---------------------------------------*/
    {
     public:
      Exception(const std::string &msg) : std::runtime_error(msg) { }
      } ;

    class BIOSIGNALML_EXPORT Clock : public bsml::Clock
    /*-----------------------------------------------*/
    {
      TYPED_OBJECT(Clock, BSML::SampleClock)
  
     public:
      virtual void extend(const double *times, const size_t length)
      {
        (void)times ;     // Unused parameters
        (void)length ;
        }
      } ;


    class BIOSIGNALML_EXPORT Signal : public bsml::Signal
    /*-------------------------------------------------*/
    template<class SIGNAL_TYPE = bsml::Signal>
    class BIOSIGNALML_EXPORT SignalArray : public std::vector<typename SIGNAL_TYPE::Pointer>
    /*------------------------------------------------------------------------------------*/
    {
      TYPED_OBJECT(Signal, BSML::Signal)
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;

     public:
      virtual void extend(const double *points, const size_t length)
      typedef std::shared_ptr<SignalArray> Pointer ;                                  \

      template<typename... Args>                                                \
      inline static Pointer new_pointer(Args... args)                           \
      {
        (void)points ;    // Unused parameters
        (void)length ;
        return std::make_shared<SignalArray>(args...) ;
        }
      } ;



      virtual void extend(const double *points, const size_t length)
      {
        (void)points ;     // Unused parameters
        (void)length ;
        }

      virtual int index(const std::string &uri) const
      {
        (void)uri ;        // Unused parameter
        return -1 ;
        }
      } ;


    class BIOSIGNALML_EXPORT Recording : public bsml::Recording
    /*-------------------------------------------------------*/
    {
      TYPED_OBJECT(Recording, BSML::Recording)

     public:
      template<class SIGNAL_TYPE=bsml::Signal, class CLOCK_TYPE=bsml::Clock>
      SignalArray<typename SIGNAL_TYPE::Pointer> new_signalarray(const std::vector<std::string> &uris,
      /*--------------------------------------------------------------------------------------------*/
                                                        const std::vector<rdf::URI> &units,
                                                        double rate)
      {
        return create_signalarray<SignalArray<SIGNAL_TYPE>, SIGNAL_TYPE, CLOCK_TYPE>(uris, units, rate, nullptr) ;
        }

      template<class SIGNAL_TYPE=bsml::Signal, class CLOCK_TYPE=bsml::Clock>
      SignalArray<typename SIGNAL_TYPE::Pointer> new_signalarray(const std::vector<std::string> &uris,
      /*--------------------------------------------------------------------------------------------*/
                                                        const std::vector<rdf::URI> &units,
                                                        typename CLOCK_TYPE::Pointer clock)
      {
        return create_signalarray<SignalArray<SIGNAL_TYPE>, SIGNAL_TYPE, CLOCK_TYPE>(uris, units, 0.0, clock) ;
        }


     protected:
      template<class SIGNAL_ARRAY_TYPE, class SIGNAL_TYPE, class CLOCK_TYPE>
      typename SIGNAL_ARRAY_TYPE::Pointer create_signalarray(const std::vector<std::string> &uris,
      /*----------------------------------------------------------------------------------------*/
                                                    const std::vector<rdf::URI> &units,
                                                    double rate, typename CLOCK_TYPE::Pointer clock)
      {
        assert(uris.size() == units.size()) ;  // Lengths of `uris` and `units` are different
        auto sigs = SIGNAL_ARRAY_TYPE::new_pointer() ;
        for (size_t n = 0 ;  n < uris.size() ;  ++n) {
          if (clock == nullptr) sigs->push_back(this->new_signal<SIGNAL_TYPE>(uris[n], units[n], rate)) ;
          else                  sigs->push_back(this->new_signal<SIGNAL_TYPE, CLOCK_TYPE>(uris[n], units[n], clock)) ;
          }
        return sigs ;
        }

      } ;

    } ;

  } ;


#endif
