#ifndef BSML_OBJECT_H
#define BSML_OBJECT_H

#include "aobject.h"
#include "ontology.h"

#include <string>

using namespace rdf ;


namespace bsml {

  class Object : public AObject::AObject
  /*----------------------------------*/
  {
    A_OBJECT

    // Generic attributes all resources have:
    PROPERTY_STRING(label, RDFS::label)
    PROPERTY_STRING(comment, RDFS::comment)
    PROPERTY_STRING(description, DCT::description)
    PROPERTY_NODE(precededBy, PRV::precededBy)
    PROPERTY_URI(creator, DCT::creator)
    PROPERTY_DATETIME(created, DCT::created)
    //, XSD.dateTime,  utils::datetime_to_isoformat, utils::isoformat_to_datetime)

   public:
    Object() { }
    Object(const std::string &uri) ;

    } ;

  } ;

#endif
