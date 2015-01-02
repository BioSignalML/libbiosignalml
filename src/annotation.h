#ifndef BSML_ANNOTATION_H
#define BSML_ANNOTATION_H

#include "object.h"
#include <string>

using namespace rdf ;


namespace bsml {

  class Annotation : public Object
  /*----------------------------*/
  {
    A_OBJECT
    METACLASS(BSML::Annotation)

    PROPERTY_URI(about, DCT::subject)
    PROPERTY_NODE(tags, BSML::tag, MANYVALUED)

   public:
    Annotation() { }
    Annotation(const std::string &uri) ;

    } ;

  } ;

#endif
