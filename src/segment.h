#ifndef BSML_SEGMENT_H
#define BSML_SEGMENT_H

#include "object.h"
#include <string>

using namespace rdf ;


namespace bsml {

  class Segment : public Object
  /*-------------------------*/
  {
    A_OBJECT
    METACLASS(BSML::Segment)

    PROPERTY_URI(source, DCT::source)
    PROPERTY_NODE(time, BSML::time, SUBELEMENT)

   public:
    Segment(const std::string &uri) ;

    } ;

  } ;

#endif
