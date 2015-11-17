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

#include <biosignalml/annotation.h>
#include <biosignalml/segment.h>

using namespace bsml ;


Annotation::Annotation(const rdf::URI &uri, Object::Ptr about, const std::string &text,
/*-----------------------------------------------------------------------------------*/
                       const std::set<rdf::Node> &tags, const Annotation::Ptr &predecessor)
: Annotation(uri)
{
  set_about(about) ;
  if (text != "") set_comment(text) ;
  for (auto const &t : tags) add_tags(t) ;

  if (predecessor.is_valid()) set_precededBy(predecessor.uri()) ;
  }

TemporalEntity::Ptr Annotation::time(void) const
/*--------------------------------------------*/
{
  if (about() && about()->rdf_type() == BSML::Segment)
    return std::static_pointer_cast<Segment>(about())->time() ;
  return annotation_time() ;  // This may not be valid...
  }
