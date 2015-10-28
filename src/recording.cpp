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


Interval::Reference Recording::new_interval(const double start, const double duration,
/*----------------------------------------------------------------------------------*/
                                            const std::string &units)
{
  auto tm = Interval::new_reference(timeline()->uri().make_URI(), start, duration, units, timeline()) ;
  add_resource(tm) ;
  return tm ;
  }

Instant::Reference Recording::new_instant(const double start, const std::string &units)
/*-----------------------------------------------------------------------------------*/
{
  auto tm = Instant::new_reference(timeline()->uri().make_URI(), start, units, timeline()) ;
  add_resource(tm) ;
  return tm ;
  }


Clock::Reference Recording::get_clock(const std::string &uri)
/*---------------------------------------------------------*/
{
  return get_resource<Clock>(rdf::URI(uri)) ;
  }

std::list<Clock::Reference> Recording::get_clocks(void)
/*---------------------------------------------------*/
{
  return get_resources<Clock>() ;
  }


Signal::Reference Recording::get_signal(const std::string &uri)
/*------------------------------------------------------------*/
{
  return get_resource<Signal>(rdf::URI(uri)) ;
  }

std::list<Signal::Reference> Recording::get_signals(void)
/*-----------------------------------------------------*/
{
  return get_resources<Signal>() ;
  }


Event::Reference Recording::get_event(const std::string &uri)
/*---------------------------------------------------------*/
{
  return get_resource<Event>(rdf::URI(uri)) ;
  }

std::list<Event::Reference> Recording::get_events(const rdf::URI &type)
/*-------------------------------------------------------------------*/
{
  return get_resources<Event>() ;  // lambda returning bool
  }


Annotation::Reference Recording::get_annotation(const std::string &uri)
/*-------------------------------------------------------------------*/
{
  return get_resource<Annotation>(rdf::URI(uri)) ;
  }

std::list<Annotation::Reference> Recording::get_annotations(void)
/*-------------------------------------------------------------*/
{
  return get_resources<Annotation>() ;
  }
