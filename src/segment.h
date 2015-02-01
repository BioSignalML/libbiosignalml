#ifndef BSML_SEGMENT_H
#define BSML_SEGMENT_H

#include "timing.h"
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
    PROPERTY_AOBJECT(TemporalEntity, time, BSML::time)

   public:
    Segment() { }
    Segment(const std::string &uri) ;
    } ;

  } ;

#endif
