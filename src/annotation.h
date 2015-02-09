#ifndef BSML_ANNOTATION_H
#define BSML_ANNOTATION_H

#include "object.h"
#include <string>

using namespace rdf ;


namespace bsml {

  class Annotation : public Object
  /*----------------------------*/
  {
    TYPED_OBJECT(Annotation, BSML::Annotation)

    PROPERTY_URI(about, DCT::subject)
    PROPERTY_NODE_SET(tags, BSML::tag)
    } ;

  } ;

#endif
