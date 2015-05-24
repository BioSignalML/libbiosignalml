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

#ifndef BSML_RECORDING_H
#define BSML_RECORDING_H

#include "biosignalml_export.h"
#include "signal.h"
#include "annotation.h"
#include "timing.h"
#include "object.h"

#include <string>
#include <vector>
#include <cassert>

using namespace rdf ;


namespace bsml {

  class BIOSIGNALML_EXPORT Recording : public Object
  /*----------------------------------------------*/
  {
    TYPED_OBJECT(Recording, BSML::Recording)

    PROPERTY_NODE(format, DCT::format)
    PROPERTY_NODE(dataset, BSML::dataset)
    PROPERTY_NODE_SET(source, DCT::source)

    PROPERTY_NODE(investigation, DCT::subject)
    PROPERTY_NODE(investigator, DCT::creator)  // Also Object::creator
    PROPERTY_DATETIME(starttime, DCT::created) // Also Object::created
    PROPERTY_DURATION(duration, DCT::extent)

    PROPERTY_OBJECT(timeline, TL::timeline, RelativeTimeLine)
    PROPERTY_NODE(generatedBy, PROV::wasGeneratedBy)  // SUBELEMENT/AOBJECT ??

    PROPERTY_OBJECT_RSET(clocks, BSML::recording, Clock)
    PROPERTY_OBJECT_RSET(signals, BSML::recording, Signal)
    PROPERTY_OBJECT_RSET(annotations, DCT::subject, Annotation)

   public:
    template<class CLOCK=Clock>
    CLOCK *new_clock(const std::string &uri, const std::string &units)
    /*--------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Clock, CLOCK>::value, "CLOCK must be derived from Clock") ;
      CLOCK *clock = new CLOCK(uri, units) ;
      clock->set_recording(this->uri()) ;
      this->m_clocks.insert(clock) ;
      return clock ;
      }

    template<class SIGNAL=Signal>
    SIGNAL *new_signal(const std::string &uri, const std::string &units, double rate)
    /*-----------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL>::value, "SIGNAL must be derived from Signal") ;
      return add_signal<SIGNAL>(new SIGNAL(uri, units, rate)) ;
      }

    template<class SIGNAL=Signal, class CLOCK=Clock>
    SIGNAL *new_signal(const std::string &uri, const std::string &units, CLOCK *clock)
    /*------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL>::value, "SIGNAL must be derived from Signal") ;
      return add_signal<SIGNAL>(new SIGNAL(uri, units, clock)) ;
      }

   private:
    template<class SIGNAL>
    SIGNAL *add_signal(SIGNAL *signal)
    /*------------------------------*/
    {
      signal->set_recording(this->uri()) ;
      this->m_signals.insert(signal) ;
      return signal ;
      }

    } ;


  } ;


/**
##            'digest':        PropertyMap(BSML.digest),

  EventClass  = Event        #: The class of :class:`~.event.Event`\s in the Recording

**/

#endif
