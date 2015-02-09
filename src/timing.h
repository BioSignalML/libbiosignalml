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


  class TemporalEntity : public AObject::AObject
  /*------------------------------------------*/
  {
    TYPED_OBJECT(TemporalEntity, TIME::TemporalEntity)

    } ;


  class Interval : public TemporalEntity
  /*----------------------------------*/
  {
    TYPED_OBJECT(Interval, BSML::Interval)

    } ;


  class Instant : public TemporalEntity
  /*---------------------------------*/
  {
    TYPED_OBJECT(Instant, BSML::Instant)

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
