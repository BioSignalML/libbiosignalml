#ifndef BSML_RECORDING_H
#define BSML_RECORDING_H

#include "signal.h"
#include "annotation.h"
#include "timing.h"
#include "object.h"

#include <string>
#include <vector>
#include <cassert>

using namespace rdf ;


namespace bsml {

  class Recording : public Object
  /*---------------------------*/
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
    template<class CLOCK = Clock>
    CLOCK *new_clock(const std::string &uri, const std::string &units)
    /*--------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Clock, CLOCK>::value, "CLOCK must be derived from Clock") ;
      CLOCK *clock = new CLOCK(uri, units) ;
      clock->set_recording(this->uri()) ;
      this->m_clocks.insert(clock) ;
      return clock ;
      }

    template<class SIGNAL = Signal>
    SIGNAL *new_signal(const std::string &uri, const std::string &units)
    /*----------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL>::value, "SIGNAL must be derived from Signal") ;
      SIGNAL *signal = new SIGNAL(uri, units) ;
      signal->set_recording(this->uri()) ;
      this->m_signals.insert(signal) ;
      return signal ;
      }

    template<class SIGNAL = Signal>
    SignalVector<SIGNAL> new_signal(const std::vector<const std::string> &uris,
    /*-----------------------------------------------------------------------*/
                                    const std::vector<const std::string> &units)
    {
      return create_signalvector<SignalVector<SIGNAL>, SIGNAL, bsml::Clock>(uris, units) ;
      }


   protected:
    template<class SIGNALVECTOR, class SIGNAL, class CLOCK>
    SIGNALVECTOR create_signalvector(const std::vector<const std::string> &uris,
    /*-----------------------------------------------------------------------*/
                                    const std::vector<const std::string> &units,
                                    CLOCK *clock = nullptr)
    {
      assert(uris.size() == units.size()) ;  // Lengths of `uris` and `units` are different
      auto signals = SIGNALVECTOR() ;
      for (int n = 0 ;  n < uris.size() ;  ++n) {
        auto signal = new SIGNAL(uris[n], units[n], clock) ;
        signals.push_back(signal) ;
        signal->set_recording(this->uri()) ;
        this->m_signals.insert(signal) ;
        }
      return signals ;
      }

    } ;


  } ;


/**
##            'digest':        PropertyMap(BSML.digest),

  EventClass  = Event        #: The class of :class:`~.event.Event`\s in the Recording

**/

#endif
