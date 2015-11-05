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

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/signal.h>
#include <biosignalml/event.h>
#include <biosignalml/annotation.h>
#include <biosignalml/segment.h>
#include <biosignalml/timing.h>
#include <biosignalml/object.h>

#include <string>
#include <vector>
#include <cassert>
#include <list>

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

    // Other reources that directly reference a Recording
    RESOURCE(BSML::recording, Clock)
    RESOURCE(BSML::recording, Signal)
    RESOURCE(BSML::recording, Event)
    RESOURCE(DCT::source,     Segment)
    RESOURCE(DCT::subject,    Annotation)

   public:
    Interval::Reference new_interval(const double start, const double duration, const std::string & units = "second") ;
    Instant::Reference new_instant(const double start, const std::string & units = "second") ;

    Clock::Reference get_clock(const std::string &uri) ;
    std::list<Clock::Reference> get_clocks(void) ;

    Signal::Reference get_signal(const std::string &uri) ;
    std::list<Signal::Reference> get_signals(void) ;

    Event::Reference get_event(const std::string &uri) ;
    std::list<Event::Reference> get_events(const rdf::URI &type=rdf::URI()) ;

    Annotation::Reference get_annotation(const std::string &uri) ;
    std::list<Annotation::Reference> get_annotations(void) ;

    template<class CLOCK_TYPE=Clock>
    typename CLOCK_TYPE::Reference new_clock(const std::string &uri, const rdf::URI &units)
    /*-----------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Clock, CLOCK_TYPE>::value, "CLOCK_TYPE must be derived from Clock") ;
      auto clock = CLOCK_TYPE::new_reference(rdf::URI(uri, m_base), units) ;
      clock->set_recording(this->uri()) ;
      this->add_resource<CLOCK_TYPE>(clock) ;
      return clock ;
      }

    template<class SIGNAL_TYPE=Signal>
    typename SIGNAL_TYPE::Reference new_signal(const std::string &uri, const rdf::URI &units, double rate)
    /*--------------------------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;
      return add_signal<SIGNAL_TYPE>(SIGNAL_TYPE::new_reference(rdf::URI(uri, m_base), units, rate)) ;
      }

    template<class SIGNAL_TYPE=Signal, class CLOCK_TYPE=Clock>
    typename SIGNAL_TYPE::Reference new_signal(const std::string &uri, const rdf::URI &units, typename CLOCK_TYPE::Reference clock)
    /*---------------------------------------------------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;
      return add_signal<SIGNAL_TYPE>(SIGNAL_TYPE::new_reference(rdf::URI(uri, m_base), units, clock)) ;
      }


   protected:
    std::string m_base ;
    INITIALISE(                                 \
      m_base = this->uri().to_string() + "/" ;  \
      )


   private:
    template<class SIGNAL_TYPE>
    typename SIGNAL_TYPE::Reference add_signal(typename SIGNAL_TYPE::Reference signal)
    /*------------------------------------------------------------------------------*/
    {
      signal->set_recording(this->uri()) ;
      add_resource<SIGNAL_TYPE>(signal) ;
      return signal ;
      }

    } ;


  } ;


/**
##            'digest':        PropertyMap(BSML.digest),

  EventClass  = Event        #: The class of :class:`~.event.Event`\s in the Recording

**/

#endif
