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

#include "hdf5.h"
#include "hdf5impl.h"


using namespace bsml ;


HDF5::Clock::Clock(const std::string &uri, const std::string &units)
/*----------------------------------------------------------------*/
: HDF5::Clock(uri)
{
  this->set_units(rdf::URI(units)) ;
  }

void HDF5::Clock::extend(const double *times, const size_t length)
/*--------------------------------------------------------------*/
{
  m_data->extend(times, length, 1, -1) ;
  }


HDF5::Signal::Signal(const std::string &uri, const std::string &units, double rate)
/*-------------------------------------------------------------------------------*/
: HDF5::Signal(uri)
{
  this->set_units(rdf::URI(units)) ;  // bsml::Units::get_units_uri(const std::string &u)
  this->set_rate(rate) ;
  }

HDF5::Signal::Signal(const std::string &uri, const std::string &units, HDF5::Clock *clock)
/*--------------------------------------------------------------------------------------*/
: HDF5::Signal(uri)
{
  this->set_units(rdf::URI(units)) ;  // bsml::Units::get_units_uri(const std::string &u)
  this->set_clock(clock->uri()) ;
  }

void HDF5::Signal::extend(const double *points, const size_t length)
/*----------------------------------------------------------------*/
{
  //m_data->extend(points, length, 1, xx) ; // Clock size
  }


void HDF5::SignalArray::extend(const double *points, const size_t length)
/*----------------------------------------------------------------------*/
{
  //m_data->extend(times, length, vectorsize, xx) ;  // clock size
  }


HDF5::Recording::Recording(const std::string &uri, const std::string &filename)
/*---------------------------------------------------------------------------*/
: HDF5::Recording(uri)
{
  m_file = nullptr ;
  try {
    m_file = HDF5::File::open(filename) ;
    }
  catch (HDF5::IOError e) {
    m_file = HDF5::File::create(uri, filename) ;
    }
  }


//HDF5::Recording::~Recording()  // Destructor is generated...
/*-------------------------------*/
//{
//  this->close() ;
//  }


void HDF5::Recording::close(void)
/*-----------------------------*/
{
  if (m_file != nullptr) {
    // First close and delete all signal and clock `m_data` objects.
    // Iterate through the `signals` and `clocks` sets???

    // But isn't `clocks` now just a set of URI strings?

    // Clocks allocated via `new_clock` (below) need to be deleted.
    // This is the time to close/delete `m_data` members.

    m_file->close() ;
    delete m_file ;
    m_file = nullptr ;
    }
  }


HDF5::Clock *HDF5::Recording::new_clock(const std::string &uri, const std::string &units,
/*-------------------------------------------------------------------------------------*/
                                        double *data, size_t datasize)
{
  auto clock = bsml::Recording::new_clock<HDF5::Clock>(uri, units) ;
  clock->m_data = m_file->create_clock(uri, units, data, datasize) ;
  return clock ;
  }


HDF5::Signal *HDF5::Recording::new_signal(const std::string &uri, const std::string &units, double rate)
/*----------------------------------------------------------------------------------------------------*/
{
  auto signal = bsml::Recording::new_signal<HDF5::Signal>(uri, units, rate) ;
  signal->m_data = m_file->create_signal(uri, units, nullptr, 0, std::vector<hsize_t>(),
                                         1.0, 0.0, rate, nullptr) ;
  return signal ;
  }

HDF5::Signal *HDF5::Recording::new_signal(const std::string &uri, const std::string &units, HDF5::Clock *clock)
/*-----------------------------------------------------------------------------------------------------------*/
{
  auto signal = bsml::Recording::new_signal<HDF5::Signal, HDF5::Clock>(uri, units, clock) ;
  signal->m_data = m_file->create_signal(uri, units, nullptr, 0, std::vector<hsize_t>(),
                                         1.0, 0.0, 0.0, clock->m_data) ;
  return signal ;
  }


HDF5::SignalArray *HDF5::Recording::new_signalarray(const std::vector<const std::string> &uris,
/*-------------------------------------------------------------------------------------------*/
                                                    const std::vector<const std::string> &units,
                                                    double rate)
{
  auto signals =
    data::Recording::create_signalarray<HDF5::SignalArray, HDF5::Signal, HDF5::Clock>(uris, units, rate, nullptr) ;
  signals->m_data = m_file->create_signal(uris, units, nullptr, 0, 1.0, 0.0, rate, nullptr) ;
  return signals ;
  }

HDF5::SignalArray *HDF5::Recording::new_signalarray(const std::vector<const std::string> &uris,
/*-------------------------------------------------------------------------------------------*/
                                                    const std::vector<const std::string> &units,
                                                    HDF5::Clock *clock)
{
  auto signals =
    data::Recording::create_signalarray<HDF5::SignalArray, HDF5::Signal, HDF5::Clock>(uris, units, 0.0, clock) ;
  signals->m_data = m_file->create_signal(uris, units, nullptr, 0, 1.0, 0.0, 0.0, clock->m_data) ;
  return signals ;
  }
