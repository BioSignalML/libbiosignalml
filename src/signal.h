#ifndef BSML_SIGNAL_H
#define BSML_SIGNAL_H

#include "object.h"
#include <string>

using namespace rdf ;


namespace bsml {

  class Signal : public Object
  /*------------------------*/
  {
    A_OBJECT
    METACLASS(BSML::Signal)
    PROPERTY_URI(recording, BSML::recording)
    PROPERTY_URI(units, BSML::units)
    PROPERTY_NODE(sensor, BSML::sensor)
    PROPERTY_NODE(filter, BSML::preFilter)
    PROPERTY_DOUBLE(rate, BSML::rate)
    PROPERTY_DOUBLE(period, BSML::period)
    PROPERTY_URI(clock, BSML::clock, SUB_ELEMENT)
    PROPERTY_DOUBLE(minFrequency, BSML::minFrequency)
    PROPERTY_DOUBLE(maxFrequency, BSML::maxFrequency)
    PROPERTY_DOUBLE(minValue, BSML::minValue)
    PROPERTY_DOUBLE(maxValue, BSML::maxValue)
    PROPERTY_INTEGER(dataBits, BSML::dataBits)
    PROPERTY_NODE(signaltype, BSML::signalType)
    PROPERTY_DURATION(offset, BSML::offset)
    PROPERTY_DURATION(duration, DCT::extent)

   public:
    Signal() { }
    Signal(const std::string &uri) ;

    } ;

  } ;

/**
  metaclass = BSML.Signal     #: :attr:`.BSML.Signal`

  mapping = { 'recording':    PropertyMap(BSML.recording, to_rdf=PropertyMap.get_uri),
              'units':        PropertyMap(BSML.units, to_rdf=PropertyMap.get_uri),
              'sensor':       PropertyMap(BSML.sensor),
              'filter':       PropertyMap(BSML.preFilter),
              'rate':        PropertyMap(BSML.rate, XSD.double),
              'period':      PropertyMap(BSML.period, XSD.double),
              'clock':        PropertyMap(BSML.clock, to_rdf=PropertyMap.get_uri, subelement=True),
              'minFrequency': PropertyMap(BSML.minFrequency, XSD.double),
              'maxFrequency': PropertyMap(BSML.maxFrequency, XSD.double),
              'minValue':     PropertyMap(BSML.minValue, XSD.double),
              'maxValue':     PropertyMap(BSML.maxValue, XSD.double),
              'dataBits':     PropertyMap(BSML.dataBits, XSD.integer),
              'signaltype':   PropertyMap(BSML.signalType),
              'offset':       PropertyMap(BSML.offset, XSD.dayTimeDuration,
                                          utils.seconds_to_isoduration,
                                          utils.isoduration_to_seconds),
              'duration':    PropertyMap(DCT.extent, XSD.dayTimeDuration,
                                          utils.seconds_to_isoduration,
                                          utils.isoduration_to_seconds),
            }

**/

#endif
