#include "timing.h"


bsml::Clock::Clock(const std::string &uri, const std::string &units)
/*------------------------------------------------------------------*/
: bsml::Clock(uri)
{
  this->set_units(rdf::URI(units)) ;  // bsml::Units::get_units_uri(const std::string &u)
  }
