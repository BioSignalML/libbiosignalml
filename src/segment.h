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
    TYPED_OBJECT(Segment, BSML::Segment)

    PROPERTY_URI(source, DCT::source)
    PROPERTY_AOBJECT(TemporalEntity, time, BSML::time)

    } ;

  } ;

#endif
