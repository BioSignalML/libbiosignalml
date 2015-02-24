#ifndef BSML_ANNOTATION_H
#define BSML_ANNOTATION_H

#include "object.h"
#include "timing.h"

using namespace rdf ;


namespace bsml {

  class Annotation : public Object
  /*----------------------------*/
  {
    TYPED_OBJECT(Annotation, BSML::Annotation)

    PROPERTY_URI(about, DCT::subject)
    PROPERTY_NODE_SET(tags, BSML::tag)
    PROPERTY_OBJECT(time, BSML::time, TemporalEntity)
    } ;

  } ;

#endif
