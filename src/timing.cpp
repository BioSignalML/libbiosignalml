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

#include <biosignalml/timing.h>

using namespace bsml ;


Interval::Interval(const rdf::URI &uri, const double start, const double duration,
/*------------------------------------------------------------------------------*/
                   const std::string &units, RelativeTimeLine::Ptr timeline)
: Interval(uri)
{
  set_timeline(timeline) ;
  set_start(xsd::Duration(start, units)) ;
  set_duration(xsd::Duration(duration, units)) ;
  }

Instant::Instant(const rdf::URI &uri, const double start, const std::string &units,
/*-------------------------------------------------------------------------------*/
                 RelativeTimeLine::Ptr timeline)
: Instant(uri)
{
  set_timeline(timeline) ;
  set_start(xsd::Duration(start, units)) ;
  }


Clock::Clock(const rdf::URI &uri, const rdf::URI &units)
/*====================================================*/
: Clock(uri)
{
  set_units(units) ;   // Units::get_units_uri(const std::string &u)
  }

double Clock::time(const size_t n) const
/*------------------------------------*/
{
  return 0.0 ;
  }

void Clock::extend(const double *points, const size_t length)
/*---------------------------------------------------------*/
{
  (void)points ;    // Unused parameters
  (void)length ;
  }

void Clock::extend(const std::vector<double> &points)
/*-------------------------------------------------*/
{
  extend(points.data(), points.size()) ;
  }

std::vector<double> Clock::read(size_t pos, intmax_t length)
/*--------------------------------------------------------*/
{
  return std::vector<double>() ;
  }
