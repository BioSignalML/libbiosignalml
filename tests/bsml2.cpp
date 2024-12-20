#include <biosignalml/biosignalml.h>

#include <iostream>
#include <string>


std::string rec_rdf = R"(@prefix dct: <http://purl.org/dc/terms/> .
@prefix prv: <http://purl.org/net/provenance/ns#> .
@prefix bsml: <http://www.biosignalml.org/ontologies/2011/04/biosignalml#> .
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
@prefix xsd: <http://www.w3.org/2001/XMLSchema#> .
@prefix cdt: <https://w3id.org/cdt/> .

<http://ex.org/recording>
	dct:description "testing 123..." ;
	dct:format "application/x-bsml+hdf5" ;
	a bsml:Recording .

<http://ex.org/recording/signal>
	bsml:rate 1000.0 ;
	bsml:recording <http://ex.org/recording> ;
	bsml:units "mV"^^cdt:ucumunit ;
	a bsml:Signal .
)" ;


int main(void)
/*----------*/
{

  std::string rec_uri = "http://ex.org/recording" ;

  auto rec = bsml::Recording::create(rdf::URI()) ;
  
  rec->set_uri(rdf::URI(rec_uri)) ;

  auto graph = rdf::Graph::create() ;

  graph->parse_string(rec_rdf, rdf::Graph::Format::TURTLE) ;

  rec->add_metadata<bsml::Recording>(graph) ;
  std::cout << "Opened " << rec->uri() << " (" << rec->description() << ")" << std::endl ;

  } ;
