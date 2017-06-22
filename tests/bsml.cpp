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

#include <biosignalml/biosignalml.h>

#include <iostream>
#include <string>

// $ tests/test_bsml ../tests/sinewave.ttl http://demo.biosignalml.org/examples/sinewave

int main(int argc, char *argv[])
/*----------------------------*/
{

  std::cout << "Starting..." << std::endl ;

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " TURTLE_FILE RECORDING_URI" << std::endl ;
    exit(1) ;
    }

  std::string source(argv[1]) ; 
  auto g = rdf::Graph::create() ;
  g->parse_resource(source, rdf::Graph::Format::TURTLE) ;
  std::cout << g->serialise(rdf::Graph::Format::TURTLE) << std::endl ;

  std::string recording(argv[2]) ;
  auto rec = bsml::Recording::create(recording) ;
  rec->add_metadata<bsml::Recording>(g) ;

  rec = bsml::Recording::create_from_graph(rdf::URI(recording), g) ;

  std::cout << rec->to_string() << std::endl << std::endl ;
  std::cout << "  Label: "        << rec->label()  << std::endl
            << "  Format: "       << rec->format() << std::endl
            << "  Investigation: "<< rec->investigation() << std::endl
            << "  Timeline: "     << rec->timeline()->uri() << std::endl
            << "  Duration: "     << rec->duration()        << std::endl ;

  auto s = rec->get_signal("http://demo.biosignalml.org/examples/sinewave/signal/0") ;
  if (s) {
    std::cout << "  Label: "       << s->label()  << std::endl
              << "  Rate: "        << s->rate()   << std::endl
              << "  Units: "       << s->units()  << std::endl ;
    }
  else std::cout << "NULL" << std::endl ;

  std::cout << std::endl << "Signals:" << std::endl ;
  for (const auto &u: rec->get_signal_uris()) {
    auto signal = rec->get_signal(u) ;
//    std::cout << signal.to_string() << std::endl ;
    std::cout << "  Label: "       << signal->label()  << std::endl
              << "  Rate: "        << signal->rate()   << std::endl
              << "  Units: "       << signal->units()  << std::endl ;
    }

  std::cout << std::endl << "Annotations:" << std::endl ;
  for (const auto &u: rec->get_annotation_uris()) {
    auto note = rec->get_annotation(u) ;
    std::cout << "  Uri: "   << note->uri()           << std::endl ;
    std::cout << "  About: " << note->about()->uri()  << std::endl ;
    std::cout << "  Note: "  << note->comment()       << std::endl ;
    auto t = note->time() ;
    std::cout << "  Time: "  << (t ? t->start() : 0.1)  << std::endl ;
    }

  }
