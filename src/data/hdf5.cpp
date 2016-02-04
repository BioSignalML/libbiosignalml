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

#include <biosignalml/data/hdf5.h>
#include "hdf5impl.h"

#include <typedobject/units.h>

#include <stdexcept>
#include <memory>
#include <algorithm>

using namespace bsml ;


HDF5::Clock::Clock(const rdf::URI &uri, const rdf::URI &units)
/*==========================================================*/
: HDF5::Clock(uri)
{
  this->set_units(units) ;
  }

double HDF5::Clock::time(const size_t n) const
/*------------------------------------------*/
{
  return 0.0 ;
size_t HDF5::Clock::index(const double t) const
/*-------------------------------------------*/
{
  return (rate() > 0.0) ? std::floor(t*rate()) : m_data->index(t) ;
  }


size_t HDF5::Clock::index_right(const double t) const
/*-------------------------------------------------*/
{
  return (rate() > 0.0) ? std::ceil(t*rate()) : m_data->index_right(t) ;
  }


void HDF5::Clock::extend(const double *times, const size_t length)
/*--------------------------------------------------------------*/
{
  m_data->extend(times, length, 1) ;
  }


std::vector<double> HDF5::Clock::read(size_t pos, intmax_t length)
/*--------------------------------------------------------------*/
{
  return m_data->read(pos, length) ;
  }


HDF5::Signal::Signal(const rdf::URI &uri, const rdf::URI &units, double rate)
/*=========================================================================*/
: HDF5::Signal(uri)
{
  this->set_units(units) ;  // bsml::Units::get_units_uri(const rdf::URI &u)
  this->set_rate(rate) ;
  }

HDF5::Signal::Signal(const rdf::URI &uri, const rdf::URI &units, HDF5::Clock::Ptr clock)
/*------------------------------------------------------------------------------------*/
: HDF5::Signal(uri)
{
  this->set_units(units) ;  // bsml::Units::get_units_uri(const std::string &u)
  this->set_clock(clock) ;
  }

void HDF5::Signal::extend(const double *points, const size_t length)
/*----------------------------------------------------------------*/
{
  m_data->extend(points, length, 1) ;
  }


data::TimeSeries::Ptr HDF5::Signal::read(Interval::Ptr interval, ssize_t maxpoints)
/*-------------------------------------------------------------------------------*/
{
  return read(0, 10000) ;  // ******* TODO Need to find point range and call read(pos, len)
  }

data::TimeSeries::Ptr HDF5::Signal::read(size_t pos, ssize_t length)    // Point based
/*----------------------------------------------------------------*/
{
  if (rate() > 0)
    return data::UniformTimeSeries::create(rate(), m_data->read(pos, length)) ;
  else
    return data::TimeSeries::create(clock()->m_data->read(pos, length), m_data->read(pos, length)) ;
  }


void HDF5::SignalArray::extend(const double *points, const size_t length)
/*----------------------------------------------------------------------*/
{
  m_data->extend(points, length, this->size()) ;
  }


int HDF5::SignalArray::index(const std::string &uri) const
/*------------------------------------------------------*/
{
  return -1 ;
  }


HDF5::Recording::Recording(const rdf::URI &uri, const std::string &filename, bool create)
/*-------------------------------------------------------------------------------------*/
: HDF5::Recording(uri)
{
  m_readonly = false ;
  if (create) {
    m_file = HDF5::File::create(uri.to_string(), filename, true) ;
    }
  else {
    m_file = HDF5::File::open(filename) ;
    // Need to read metadata and create objects...
    // And ensure URI is as expected.... (as below...)
    }
  }


HDF5::Recording::Recording(const std::string &filename, bool readonly)
/*------------------------------------------------------------------*/
: HDF5::Recording(rdf::URI())
{
  m_file = HDF5::File::open(filename, readonly) ;
  m_readonly = readonly ;

  this->set_uri(rdf::URI(m_file->get_uri())) ;
  this->m_base = uri().to_string() + "/" ;

  auto metadata = m_file->get_metadata() ;
  m_graph = rdf::Graph::create(uri()) ;
  m_graph->parse_string(metadata.first, rdf::Graph::mimetype_to_format(metadata.second)) ;
  this->template add_metadata<HDF5::Recording>(m_graph) ;

  for (auto const & u : get_clock_uris()) {
    auto c = get_clock(u) ;
    c->m_data = m_file->get_clock(c->uri().to_string()) ;
    datasets.insert(c->m_data) ;
    }
  for (auto const & u : get_signal_uris()) {
    auto s = get_signal(u) ;
    s->m_data = m_file->get_signal(s->uri().to_string()) ;
    datasets.insert(s->m_data) ;
    auto clk = s->clock() ;
    if (s->rate() <= 0) {
      if (clk && clk->is_valid()) clk->m_data = m_file->get_clock(clk->uri().to_string()) ;
      else throw HDF5::Exception("Signal with no rate doesn't have a clock") ;
      }
    }
  }


void HDF5::Recording::close(void)
/*-----------------------------*/
{
  if (m_file != nullptr) {
    if (!m_readonly) {
      rdf::Graph::Format format = rdf::Graph::Format::TURTLE ;
// Prefixes are duplicated in file...  (serd bug ??)
      m_file->store_metadata(serialise_metadata(format, m_base, true),
                             rdf::Graph::format_to_mimetype(format)) ;
      }
    for (auto ds : datasets) ds->close() ;
    m_file->close() ;
    delete m_file ;
    m_file = nullptr ;
    }
  }


std::list<rdf::URI> HDF5::Recording::get_clock_uris(void)
/*-----------------------------------------------------*/
{
  return bsml::Recording::get_clock_uris<HDF5::Clock>() ;
  }

HDF5::Clock::Ptr HDF5::Recording::get_clock(const rdf::URI &uri)
/*------------------------------------------------------------*/
{
  auto clk = bsml::Recording::get_clock<HDF5::Clock>(uri) ;
  if (clk) return clk ;
  else throw HDF5::Exception("Unknown signal '" + uri.to_string() + "' in recording") ;
  }

HDF5::Clock::Ptr HDF5::Recording::get_clock(const std::string &uri)
/*---------------------------------------------------------------*/
{
  return get_clock(rdf::URI(uri)) ;
  }


std::list<rdf::URI> HDF5::Recording::get_signal_uris(void)
/*------------------------------------------------------*/
{
  return bsml::Recording::get_signal_uris<HDF5::Signal>() ;
  }

HDF5::Signal::Ptr HDF5::Recording::get_signal(const rdf::URI &uri)
/*--------------------------------------------------------------*/
{
  auto sig = bsml::Recording::get_signal<HDF5::Signal>(uri) ;
  if (sig) return sig ;
  else throw HDF5::Exception("Unknown signal '" + uri.to_string() + "' in recording") ;
  }

HDF5::Signal::Ptr HDF5::Recording::get_signal(const std::string &uri)
/*-----------------------------------------------------------------*/
{
  return get_signal(rdf::URI(uri)) ;
  }


HDF5::Clock::Ptr HDF5::Recording::new_clock(const std::string &uri,
/*---------------------------------------------------------------*/
                                            const rdf::URI &units,
                                            double *data, size_t datasize)
{
  auto clock = bsml::Recording::new_clock<HDF5::Clock>(uri, units) ;
  try {
    std::string u = units.to_string() ;
    size_t pos = u.find_last_of('#') ;  // NEED to work on full URI...
    double resolution = Unit::Converter(u.substr(pos+1), "second").convert(1.0) ;
    if (resolution != 1.0) clock->set_resolution(resolution) ;
    }
  catch (const std::exception &error) {
    }
  clock->m_data = m_file->create_clock(clock->uri().to_string(), units.to_string(), data, datasize) ;
  datasets.insert(clock->m_data) ;
  return clock ;
  }


HDF5::Signal::Ptr HDF5::Recording::new_signal(const std::string &uri,
/*-----------------------------------------------------------------*/
                                              const rdf::URI &units,
                                              double rate)
{
  auto signal = bsml::Recording::new_signal<HDF5::Signal>(uri, units, rate) ;
  signal->m_data = m_file->create_signal(signal->uri().to_string(), units.to_string(),
                                         nullptr, 0, std::vector<hsize_t>(),
                                         1.0, 0.0, rate, nullptr) ;
  datasets.insert(signal->m_data) ;
  return signal ;
  }

HDF5::Signal::Ptr HDF5::Recording::new_signal(const std::string &uri,
/*-----------------------------------------------------------------*/
                                              const rdf::URI &units,
                                              HDF5::Clock::Ptr clock)
{
  auto signal = bsml::Recording::new_signal<HDF5::Signal, HDF5::Clock>(uri, units, clock) ;
  signal->m_data = m_file->create_signal(signal->uri().to_string(), units.to_string(),
                                         nullptr, 0, std::vector<hsize_t>(),
                                         1.0, 0.0, 0.0, clock->m_data) ;
  datasets.insert(signal->m_data) ;
  return signal ;
  }


HDF5::SignalArray::Ptr HDF5::Recording::new_signalarray(const std::vector<std::string> &uris,
/*-----------------------------------------------------------------------------------------*/
                                                        const std::vector<rdf::URI> &units,
                                                        double rate)
{
  auto signals =
    data::Recording::create_signalarray<HDF5::SignalArray, HDF5::Signal, HDF5::Clock>(uris, units, rate, nullptr) ;
  std::vector<std::string> uri_strings ;
  for (auto &s : *signals) uri_strings.push_back(s->uri().to_string()) ;
  std::vector<std::string> unit_strings ;
  for (auto const &unit : units) unit_strings.push_back(unit.to_string()) ;
  signals->m_data = m_file->create_signal(uri_strings, unit_strings, nullptr, 0, 1.0, 0.0, rate, nullptr) ;
  datasets.insert(signals->m_data) ;
  return signals ;
  }

HDF5::SignalArray::Ptr HDF5::Recording::new_signalarray(const std::vector<std::string> &uris,
/*-----------------------------------------------------------------------------------------*/
                                                        const std::vector<rdf::URI> &units,
                                                        HDF5::Clock::Ptr clock)
{
  auto signals =
    data::Recording::create_signalarray<HDF5::SignalArray, HDF5::Signal, HDF5::Clock>(uris, units, 0.0, clock) ;
  std::vector<std::string> uri_strings ;
  for (auto &s : *signals) uri_strings.push_back(s->uri().to_string()) ;
  std::vector<std::string> unit_strings ;
  for (auto const &unit : units) unit_strings.push_back(unit.to_string()) ;
  signals->m_data = m_file->create_signal(uri_strings, unit_strings, nullptr, 0, 1.0, 0.0, 0.0, clock->m_data) ;
  datasets.insert(signals->m_data) ;
  return signals ;
  }
