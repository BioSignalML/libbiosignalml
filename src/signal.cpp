

#include "signal.h"


bsml::Signal::Signal(const std::string &uri, const std::string &units, bsml::Clock *clock)
/*--------------------------------------------------------------------------------------*/
: bsml::Signal(uri)
{
  this->set_units(rdf::URI(units)) ;  // bsml::Units::get_units_uri(const std::string &u)
  if (clock != nullptr) this->set_clock(clock->uri()) ;
  }
