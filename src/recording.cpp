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

#include <biosignalml/recording.h>

using namespace bsml ;


Interval::Ptr Recording::new_interval(const double start, const double duration,
/*----------------------------------------------------------------------------*/
                                      const std::string &units)
{
  auto tm = Interval::new_reference(timeline()->uri().make_URI(), start, duration, units, timeline()) ;
  add_resource<Interval>(tm) ;
  return tm ;
  }

Instant::Ptr Recording::new_instant(const double start, const std::string &units)
/*-----------------------------------------------------------------------------*/
{
  auto tm = Instant::new_reference(timeline()->uri().make_URI(), start, units, timeline()) ;
  add_resource<Instant>(tm) ;
  return tm ;
  }


Clock::Ptr Recording::get_clock(const rdf::URI &uri)
/*------------------------------------------------*/
{
  return get_resource<Clock>(rdf::URI(uri)) ;
  }

std::list<Clock::Reference> Recording::get_clocks(void)
Clock::Ptr Recording::get_clock(const std::string &uri)
/*---------------------------------------------------*/
{
  return get_resources<Clock>() ;
  return get_clock(uri) ;
  }



Signal::Ptr Recording::get_signal(const rdf::URI &uri)
/*---------------------------------------------------*/
{
  return get_resource<Signal>(uri) ;
  }

Signal::Ptr Recording::get_signal(const std::string &uri)
/*-----------------------------------------------------*/
{
  return get_signal(rdf::URI(uri)) ;
  }
  }


Event::Ptr Recording::get_event(const rdf::URI &uri)
/*------------------------------------------------*/
{
  return get_resource<Event>(uri) ;
  }

Event::Ptr Recording::get_event(const std::string &uri)
/*---------------------------------------------------*/
{
  return get_event(rdf::URI(uri)) ;
  }
  }


Annotation::Ptr Recording::get_annotation(const rdf::URI &uri)
/*----------------------------------------------------------*/
{
  return get_resource<Annotation>(uri) ;
  }

Annotation::Ptr Recording::get_annotation(const std::string &uri)
/*-------------------------------------------------------------*/
{
  return get_annotation(rdf::URI(uri)) ;
  }
  }
