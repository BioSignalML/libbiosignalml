#ifndef BSML_TIMING_H
#define BSML_TIMING_H

#include "timing.h"
#include "object.h"

#include <string>

using namespace rdf ;

namespace bsml {

  class RelativeTimeLine : public Object
  /*----------------------------------*/
  {
    TYPED_OBJECT(RelativeTimeLine, TL::RelativeTimeLine)
    } ;


  class TemporalEntity : public Object
  /*--------------------------------*/
  {
    TYPED_OBJECT(TemporalEntity, TIME::TemporalEntity)

    PROPERTY_OBJECT(timeline, TL::timeline, RelativeTimeLine)
    PROPERTY_DURATION(start, NONE)
    PROPERTY_DURATION(duration, NONE)
    } ;


  class Interval : public TemporalEntity
  /*----------------------------------*/
  {
    TYPED_OBJECT(Interval, BSML::Interval)

    ASSIGN_DURATION(start, TL::start)
    ASSIGN_DURATION(duration, TL::duration)
    } ;


  class Instant : public TemporalEntity
  /*---------------------------------*/
  {
    TYPED_OBJECT(Instant, BSML::Instant)

    ASSIGN_DURATION(start, TL::at)
    } ;


  class Clock : public Object
  /*-----------------------*/
  {
    TYPED_OBJECT(Clock, BSML::SampleClock)

    // Also have 'frequency' ?? and/or 'period' ??
    PROPERTY_DOUBLE(resolution, BSML::resolution)
    PROPERTY_DOUBLE(rate,       BSML::rate)
    } ;

  } ;

#endif
