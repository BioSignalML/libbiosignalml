#include "formats/hdf5.h"


using namespace bsml ;


HDF5::Clock::Clock(const std::string &uri, const std::string &units)
/*----------------------------------------------------------------*/
: HDF5::Clock(uri)
{
  this->set_units(rdf::URI(units)) ;
  }


HDF5::Signal::Signal(const std::string &uri, const std::string &units, HDF5::Clock *clock)
/*--------------------------------------------------------------------------------------*/
: HDF5::Signal(uri)
{
  this->set_units(rdf::URI(units)) ;  // bsml::Units::get_units_uri(const std::string &u)
  if (clock != nullptr) this->set_clock(clock->uri()) ;
  }



HDF5::Recording::Recording(const std::string &uri, const std::string &filename)
/*---------------------------------------------------------------------------*/
: HDF5::Recording(uri)
{
  closed = true ;
  }


//HDF5::Recording::~Recording()  // Destructor is generated...
/*-------------------------------*/
//{
//  this->close() ;
//  }


void HDF5::Recording::close(void)
/*-----------------------------*/
{
  if (!closed) {
    closed = true ;
    }
  }


HDF5::Clock *HDF5::Recording::new_clock(const std::string &uri, const std::string &units, double *times)
/*----------------------------------------------------------------------------------------------------*/
{
  return bsml::Recording::new_clock<HDF5::Clock>(uri, units) ;
  }


HDF5::Signal *HDF5::Recording::new_signal(const std::string &uri, const std::string &units)
/*---------------------------------------------------------------------------------------*/
{
  return bsml::Recording::new_signal<HDF5::Signal>(uri, units) ;
  }

HDF5::SignalVector HDF5::Recording::new_signal(const std::vector<const std::string> &uris,
/*--------------------------------------------------------------------------------------*/
                                                           const std::vector<const std::string> &units,
                                                           HDF5::Clock *clock)
{
  return bsml::Recording::create_signalvector<HDF5::SignalVector, HDF5::Signal, HDF5::Clock>(uris, units, clock) ;
  }

