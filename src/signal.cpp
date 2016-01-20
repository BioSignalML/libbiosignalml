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

#include <biosignalml/signal.h>


bsml::Signal::Signal(const rdf::URI &uri, const rdf::URI &units, double rate)
/*-------------------------------------------------------------------------*/
: bsml::Signal(uri)
{
  this->set_units(units) ;  // bsml::Units::get_units_uri(const std::string &u)
  this->set_rate(rate) ;
  }

bsml::Signal::Signal(const rdf::URI &uri, const rdf::URI &units, bsml::Clock::Ptr clock)
/*------------------------------------------------------------------------------------*/
: bsml::Signal(uri)
{
  this->set_units(units) ;  // bsml::Units::get_units_uri(const std::string &u)
  this->set_clock(clock) ;
  }


void bsml::Signal::extend(const double *points, const size_t length)
/*----------------------------------------------------------------*/
{
  (void)points ;    // Unused parameters
  (void)length ;
  }

void bsml::Signal::extend(const std::vector<double> &points)
/*--------------------------------------------------------*/
{
  extend(points.data(), points.size()) ;
  }

#include <iostream>
bsml::data::TimeSeries::Ptr bsml::Signal::read(bsml::Interval::Ptr interval, intmax_t maxpoints)
/*--------------------------------------------------------------------------------------------*/
{
  return read(0, 0) ;
  }

bsml::data::TimeSeries::Ptr bsml::Signal::read(size_t pos, intmax_t length)
/*-----------------------------------------------------------------------*/
{
  return data::TimeSeries::create() ;
  }
