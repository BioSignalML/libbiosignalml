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

#include <biosignalml/data/hdf5.h>
#include <biosignalml/biosignalml.h>

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415726
#endif


int main(int argc, char *argv[])
/*----------------------------*/
{

  bsml::HDF5::Recording hdf5(rdf::URI("http://ex.org/recording"), "test.h5", true) ;
  hdf5.set_description("testing 123...") ;

  const std::vector<const std::string> uris{"1",
                                            "2",
                                            "3"
                                           } ;
  const std::vector<const rdf::URI> units{rdf::URI("http://units.org/V"),
                                          rdf::URI("http://units.org/mV"),
                                          rdf::URI("http://units.org/mV")
                                          } ;



  auto *sig = hdf5.new_signal("signal", units[1], 1000.0) ;



#define NPOINTS  1001
#define NSIGS       3

//  std::vector<double> times(NPOINTS) ;     // Times as vector<> v's double[]
// C++11  vector.data() is array[] of data elements

  double times[NPOINTS] ;
  for (int t = 0 ;  t < NPOINTS ;  ++t) {
    times[t] = (double)t/((double)(NPOINTS - 1)) ;
    }

  bsml::HDF5::Clock *clock = hdf5.new_clock("clock",
                                            rdf::URI("http://units.org/Second"),
                                            times, NPOINTS) ;
  clock->set_label("A common clock") ;

  auto signals = hdf5.new_signalarray(uris, units, clock) ;
  double data[NPOINTS*NSIGS] ;
  for (int t = 0 ;  t < NPOINTS ;  ++t) {
    for (int n = 0 ;  n < NSIGS ;  ++n) {
      double x = 2.0*M_PI*times[t]/times[NPOINTS-1] ;
      if      (n == 1) data[NSIGS*t + 1] = sin(x) ;
      else if (n == 2) data[NSIGS*t + 2] = cos(x) ;
      else             data[NSIGS*t + n] = times[t] ;
      }
    }
  signals->extend(data, NPOINTS*NSIGS) ;


//  std::cout << hdf5.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;


  hdf5.close() ;  // Should automatically update metadata...

  }
