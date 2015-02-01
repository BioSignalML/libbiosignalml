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
    A_OBJECT
    METACLASS(TL::RelativeTimeLine)

   public:
    RelativeTimeLine() { }
    RelativeTimeLine(const std::string &uri) ;
    } ;


  class TemporalEntity : public AObject::AObject
  /*------------------------------------------*/
  {
    A_OBJECT
    PROPERTY_AOBJECT(RelativeTimeLine, timeline, TL::timeline)
    PROPERTY_DURATION(start, TL::start)

   public:
    TemporalEntity() { }
    TemporalEntity(const std::string &uri) ;
    } ;


  class Interval : public TemporalEntity
  /*----------------------------------*/
  {
    A_OBJECT
    METACLASS(BSML::Interval)
    PROPERTY_DURATION(duration, TL::duration)

   public:
    Interval() { }
    Interval(const std::string &uri) ;
    } ;


  class Instant : public TemporalEntity
  /*---------------------------------*/
  {
    A_OBJECT
    METACLASS(BSML::Instant)

   public:
    Instant() { }
    Instant(const std::string &uri) ;
    } ;


  class Clock : public Object
  /*-----------------------*/
  {
    A_OBJECT
    METACLASS(BSML::SampleClock)

    // Also have 'frequency' ?? and/or 'period' ??
    PROPERTY_DOUBLE(resolution, BSML::resolution)
    PROPERTY_DOUBLE(rate,       BSML::rate)

   public:
    Clock() { }
    Clock(const std::string &uri) ;
    } ;

  } ;

#endif
