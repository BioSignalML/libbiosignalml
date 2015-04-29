#ifndef BSML_SIGNAL_H
#define BSML_SIGNAL_H

#include "timing.h"
#include "object.h"

#include <string>
#include <vector>
#include <type_traits>

using namespace rdf ;


namespace bsml {

  class Signal : public Object
  /*------------------------*/
  {
    TYPED_OBJECT(Signal, BSML::Signal)
    PROPERTY_URI(recording, BSML::recording)
    PROPERTY_URI(units, BSML::units)
    PROPERTY_NODE(sensor, BSML::sensor)
    PROPERTY_NODE(filter, BSML::preFilter)
    PROPERTY_DECIMAL(rate, BSML::rate)
    PROPERTY_DECIMAL(period, BSML::period)
    PROPERTY_URI(clock, BSML::clock)
    PROPERTY_DECIMAL(minFrequency, BSML::minFrequency)
    PROPERTY_DECIMAL(maxFrequency, BSML::maxFrequency)
    PROPERTY_DECIMAL(minValue, BSML::minValue)
    PROPERTY_DECIMAL(maxValue, BSML::maxValue)
    PROPERTY_INTEGER(dataBits, BSML::dataBits)
    PROPERTY_NODE(signaltype, BSML::signalType)
    PROPERTY_DURATION(offset, BSML::offset)
    PROPERTY_DURATION(duration, DCT::extent)

   public:
    Signal(const std::string &uri, const std::string &units, Clock *clock=nullptr) ;


    } ;

  } ;

#endif
