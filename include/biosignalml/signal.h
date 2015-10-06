/******************************************************************************
 *                                                                            *
 *  BioSignalML Management in C++                                             *
 *                                                                            *
 *  Copyright (c) 2010-2015  David Brooks                                     *
 *                                                                            *
 *  Licensed under the Apache License, Version 2.0 (the "License");           *
 *  you may not use this file except in compliance with the License.          *
 *  You may obtain a copy of the License at                                   *
 *                                                                            *
 *      http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                            *
 *  Unless required by applicable law or agreed to in writing, software       *
 *  distributed under the License is distributed on an "AS IS" BASIS,         *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *  See the License for the specific language governing permissions and       *
 *  limitations under the License.                                            *
 *                                                                            *
 ******************************************************************************/

#ifndef BSML_SIGNAL_H
#define BSML_SIGNAL_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/timing.h>
#include <biosignalml/object.h>

#include <string>
#include <vector>
#include <type_traits>

using namespace rdf ;


namespace bsml {

  class BIOSIGNALML_EXPORT Signal : public Object
  /*-------------------------------------------*/
  {
    TYPED_OBJECT(Signal, BSML::Signal)
    PROPERTY_URI(recording, BSML::recording)
    PROPERTY_URI(units, BSML::units)
    PROPERTY_NODE(sensor, BSML::sensor)
    PROPERTY_NODE(filter, BSML::preFilter)
    PROPERTY_DECIMAL(rate, BSML::rate)
    PROPERTY_DECIMAL(period, BSML::period)
    PROPERTY_REFERENCE(clock, BSML::clock, Clock)
    PROPERTY_DECIMAL(minFrequency, BSML::minFrequency)
    PROPERTY_DECIMAL(maxFrequency, BSML::maxFrequency)
    PROPERTY_DECIMAL(minValue, BSML::minValue)
    PROPERTY_DECIMAL(maxValue, BSML::maxValue)
    PROPERTY_INTEGER(dataBits, BSML::dataBits)
    PROPERTY_NODE(signaltype, BSML::signalType)
    PROPERTY_DURATION(offset, BSML::offset)
    PROPERTY_DURATION(duration, DCT::extent)

   public:
    Signal(const rdf::URI &uri, const rdf::URI &units, double rate) ;
    Signal(const rdf::URI &uri, const rdf::URI &units, Clock::Reference clock) ;

    virtual void extend(const double *points, const size_t length) ;
    void extend(const std::vector<double> &points) ;
    virtual std::vector<double> read(const bsml::Interval &interval) ;   // Time based
    virtual std::vector<double> read(size_t pos=0, intmax_t length=0) ;  // Point based

    } ;

  } ;

#endif
