#include "model.h"

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
  g.parseResource(source, rdf::Graph::Format::TURTLE) ;
  std::cout << g.serialise(rdf::Graph::Format::TURTLE) << std::endl ;

  std::string recording(argv[2]) ;
  bsml::Recording rec(recording, g) ;

  std::cout << rec.to_string() << std::endl ;
  std::cout << "  Label: "        << rec.label()  << std::endl
            << "  Format: "       << rec.format() << std::endl
            << "  Investigation: "<< rec.investigation() << std::endl
            << "  Timeline: "     << rec.timeline()->uri() << std::endl
            << "  Duration: "     << rec.duration().to_string() << std::endl ;

  std::cout << std::endl << "Signals:" << std::endl ;
  for (const auto &signal: rec.signals()) {
//    std::cout << signal.to_string() << std::endl ;
    std::cout << "  Label: "       << signal->label()  << std::endl
              << "  Rate: "        << signal->rate()   << std::endl
              << "  Units: "       << signal->units()  << std::endl ;
    }


  std::cout << std::endl << "Annotations:" << std::endl ;
  for (const auto &note: rec.annotations()) {
    std::cout << "  Note: "        << note->comment()  << std::endl
              << "  Time: "        << note->time()->start().to_string() << std::endl ;
    }

  }
