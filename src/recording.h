#ifndef BSML_RECORDING_H
#define BSML_RECORDING_H

#include "signal.h"
#include "annotation.h"
#include "timing.h"
#include "object.h"

#include <string>

using namespace rdf ;


namespace bsml {

  class Recording : public Object
  /*---------------------------*/
  {
    TYPED_OBJECT(Recording, BSML::Recording)

    PROPERTY_STRING(format, DCT::format)
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
    } ;

  } ;


/**
##            'digest':        PropertyMap(BSML.digest),

  EventClass  = Event        #: The class of :class:`~.event.Event`\s in the Recording

**/

#endif
