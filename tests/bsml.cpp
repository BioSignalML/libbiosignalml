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

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " TURTLE_FILE RECORDING_URI" << std::endl ;
    exit(1) ;
    }

  std::string source(argv[1]) ; 
  rdf::Graph g ;
  g.parse_resource(source, rdf::Graph::Format::TURTLE) ;
  std::cout << g.serialise(rdf::Graph::Format::TURTLE) << std::endl ;

  std::string recording(argv[2]) ;
  bsml::Recording rec(recording, g) ;

  std::cout << rec.to_string() << std::endl << std::endl ;
  std::cout << "  Label: "        << rec.label()  << std::endl
            << "  Format: "       << rec.format() << std::endl
            << "  Investigation: "<< rec.investigation() << std::endl
            << "  Timeline: "     << rec.timeline()->uri() << std::endl
            << "  Duration: "     << rec.duration()        << std::endl ;


  auto s = rec.get_signal("http://demo.biosignalml.org/examples/sinewave/signal/0") ;
  if (s) {
    std::cout << "  Label: "       << s->label()  << std::endl
              << "  Rate: "        << s->rate()   << std::endl
              << "  Units: "       << s->units()  << std::endl ;
    }
  else std::cout << "NULL" << std::endl ;

  std::cout << std::endl << "Signals:" << std::endl ;
  for (const auto &signal: rec.get_signals()) {
//    std::cout << signal.to_string() << std::endl ;
    std::cout << "  Label: "       << signal->label()  << std::endl
              << "  Rate: "        << signal->rate()   << std::endl
              << "  Units: "       << signal->units()  << std::endl ;
    }


  std::cout << std::endl << "Annotations:" << std::endl ;
  for (const auto &note: rec.get_annotations()) {
    std::cout << "  Note: "        << note->comment()       << std::endl
              << "  Time: "        << note->time().start() << std::endl ;
    }

  }
