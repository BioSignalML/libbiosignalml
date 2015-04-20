#ifndef BSML_RECORDING_H
#define BSML_RECORDING_H

#include "signal.h"
#include "annotation.h"
#include "timing.h"
#include "object.h"

#include <string>
#include <vector>

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

    PROPERTY_OBJECT_RSET(signals, BSML::recording, Signal)
    PROPERTY_OBJECT_RSET(annotations, DCT::subject, Annotation)

   public:
    template<class SIGNAL = Signal>
    SIGNAL *new_signal(const std::string &uri)
    /*--------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL>::value, "SIGNAL must be derived from Signal") ;
      SIGNAL *signal = new SIGNAL(uri) ;
      signal->set_recording(this->uri()) ;
      this->m_signals.insert(signal) ;
      return signal ;
      }

    template<class SIGNAL = Signal>
    SignalVector<SIGNAL> new_signal(const std::vector<const std::string> &uris)
    /*------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL>::value, "SIGNAL must be derived from Signal") ;
      SignalVector<SIGNAL> signals ;
      for (const auto uri : uris) {
        signals.push_back(this->new_signal<SIGNAL>(uri)) ;
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
