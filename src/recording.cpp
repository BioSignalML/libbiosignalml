#include "recording.h"


bsml::Recording::Recording(const std::string &uri)
/*----------------------------------------------*/
: AObject::AObject(uri), m_format(""), m_source(), m_subject()
{
  }


