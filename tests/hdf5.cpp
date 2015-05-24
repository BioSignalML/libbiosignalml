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

#include "data/hdf5.h"
#include "biosignalml.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>


int main(int argc, char *argv[])
/*----------------------------*/
{

  bsml::HDF5::Recording hdf5("http://ex.org/recording", "test.h5") ;
  hdf5.set_description("testing 123...") ;

  const std::vector<const std::string> uris{"http://ex.org/1",
                                            "http://ex.org/2",
                                            "http://ex.org/3"
                                           } ;
  const std::vector<const std::string> units{"http://units.org/V",
                                             "http://units.org/mV",
                                             "http://units.org/mV"
                                           } ;



  auto *sig = hdf5.new_signal("http://ex.org/signal", units[1], 1000.0) ;



#define NPOINTS  1001
#define NSIGS       3

//  std::vector<double> times(NPOINTS) ;     // Times as vector<> v's double[]
// C++11  vector.data() is array[] of data elements

  double times[NPOINTS] ;
  for (int t = 0 ;  t < NPOINTS ;  ++t) {
    times[t] = (double)t/((double)(NPOINTS - 1)) ;
    }

  bsml::HDF5::Clock *clock = hdf5.new_clock("http://ex.org/clock",
                                            "http://units.org/Second",
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
