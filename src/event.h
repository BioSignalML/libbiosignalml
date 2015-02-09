#ifndef BSML_EVENT_H
#define BSML_EVENT_H

#include "object.h"
#include <string>

using namespace rdf ;


namespace bsml {

  class Event : public Object
  /*-----------------------*/
  {
    TYPED_OBJECT(Event, BSML::Event)

    PROPERTY_URI(recording, BSML::recording)
    PROPERTY_URI(eventtype, BSML::eventType)
    PROPERTY_NODE(time, BSML::time, SUBELEMENT)

    } ;

  } ;

#endif
