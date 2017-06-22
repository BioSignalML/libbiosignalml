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

#include <typeinfo>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#ifndef WIN32
#include <cxxabi.h>

// todo: javadoc this properly
const char* demangle(const char* name)
{
    static char buf[1024];
    size_t size = sizeof(buf);
    int status;
    // todo:
    char* res = abi::__cxa_demangle (name,
                                 buf,
                                 &size,
                                 &status);
    buf[sizeof(buf) - 1] = 0; // I'd hope __cxa_demangle does this when the name is huge, but just in case.
    return res;
  }
#endif


#ifndef M_PI
#define M_PI 3.1415726
#endif


//#define NPOINTS  1001
#define NPOINTS     5
#define NSIGS       3


void test_read(bsml::HDF5::Signal::Ptr signal, double start, double duration,
/*-------------------------------------------------------------------------*/
               size_t s_index, size_t e_index)
{
  std::cout << std::endl << "TEST " << start << " to " << (start+duration) << std::endl ;

  auto p = bsml::Interval::create(rdf::URI(), start, duration) ;
  auto d = signal->read(p) ;

  if (d->size() > 0) {
    assert(d->time(0) == (double)s_index/((double)(NPOINTS - 1))) ;
    assert(d->time(d->size()-1) == (double)e_index/((double)(NPOINTS - 1))) ;
    }
  else assert(s_index > e_index) ;

  for (auto n = 0 ;  n < d->size() ;  ++n) {
    auto p = d->point(n) ;
    std::cout << "  DATA: " << p.time() << " " << p.value() << std::endl ;
    }
  }


int main(int argc, char *argv[])
/*----------------------------*/
{

  auto hdf5 = bsml::HDF5::Recording(rdf::URI("http://ex.org/recording"), "test.h5", true) ;
  hdf5.set_description("testing 123...") ;

  auto sig = hdf5.new_signal("signal", rdf::URI("http://units.org/mV"), 1000.0) ;
  double points[10] = { 0, 1, 2, 3, 4, 4, 3, 2, 1, 0 } ;
  sig->extend(points, 10) ;

  hdf5.set_duration(10/1000.0) ;

  const std::vector<std::string> uris{"1", "2", "3" } ;
  const std::vector<rdf::URI> units{rdf::URI("http://units.org/V"),
                                    rdf::URI("http://units.org/mV"),
                                    rdf::URI("http://units.org/mV")
                                    } ;


//  std::vector<double> times(NPOINTS) ;     // Times as vector<> v's double[]
// C++11  vector.data() is array[] of data elements

  double times[NPOINTS] ;
  for (int t = 0 ;  t < NPOINTS ;  ++t) {
    times[t] = (double)t/((double)(NPOINTS - 1)) ;
    }

  auto clock = hdf5.new_clock("clock", rdf::URI("http://units.org/Second"), times, NPOINTS) ;
  clock->set_label("A common clock") ;

  hdf5.set_duration(1.0) ;

  auto signals = hdf5.new_signalarray(uris, units, clock) ;
  double data[NPOINTS*NSIGS] ;
  for (int t = 0 ;  t < NPOINTS ;  ++t) {
    for (int n = 0 ;  n < NSIGS ;  ++n) {
//      double x = 2.0*M_PI*times[t]/times[NPOINTS-1] ;
//      if      (n == 1) data[NSIGS*t + 1] = sin(x) ;
//      else if (n == 2) data[NSIGS*t + 2] = cos(x) ;
//      else             data[NSIGS*t + n] = times[t] ;
      data[NSIGS*t + n] = t + 100*n ;
      }
    }
  signals->extend(data, NPOINTS*NSIGS) ;

//  std::cout << "STORED: " << hdf5.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;
  hdf5.close() ;  // Should automatically update metadata...


  try {

    auto hdf5 = bsml::HDF5::Recording("test.h5", true) ;  // Open for reading, readonly
    std::cout << "Opened " << hdf5.uri() << " (" << hdf5.description() << ")" << std::endl ;

// Prefixes are duplicated...  (serd bug ??)
    std::cout << "READ: " << hdf5.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;

//    for (auto const &s : hdf5.signal_set()) {
//      std::cout << "Signal: " << std::hex << (long)s.get() << " " << s->uri() << " " << demangle(typeid(s.get()).name()) << std::endl ;
//      }

/****

  TIDY UPS...

  1. Signal.clock() to return Clock object and not clock URI

  2. Signal.time(n) == Signal.clock().time(n)

  3. Read sequential from last read

  4. Return number of points read

  5. read(length) instead of read(pos) -- what about required params?
       -- read(), read(length) and read(pos, length)
       -- first two read from last position
       
  6. Signals might not have a 'rate' attribute but instead have 'clock'.
  
   
  DONE:
   
  1. Auto convert from rdf::URI to std::string (explicit operator)
        

****/

    auto s = hdf5.get_signal("http://ex.org/recording/2") ;  // Returns nullptr if not found
//    auto c = s->clock() ;
//    auto s = hdf5.get_signal("signal") ;    // Should fail with exception if unknown
    // We want s to be a HDF5::Signal but 'unopened', and have `s->read()`
    // to actually get data.
    std::cout << "Signal: " << s->uri() << std::endl ;

//std::cout << "Sx: "       << std::type_index(typeid(s.get())).name()
//          << std::endl ;

    if (s) {
//      auto d = s->read(0) ;    // (0), (0, 100)  (0.01, 1.0)  // start, len ??
//      auto d = s->read(1, 2) ;
//    auto t = c->read(2) ;

      test_read(s, 0.0,  1.0,  0, 4) ;  // Should get points [0, 1, 2, 3, 4], interval [0.0, 1.0]
      test_read(s, 0.25, 0.25, 1, 2) ;  // Should get points [1, 2], interval [0.25, 0.5]
      test_read(s, 0.25, 0.2,  1, 1) ;  // Should get point [1], interval [0.25, 0.25]
      test_read(s, 0.2,  0.25, 1, 1) ;  // Should get point [1], interval [0.25, 0.25]
      test_read(s, 0.45, 0.25, 2, 2) ;  // Should get point [2], interval [0.5, 0.5]
      test_read(s, 0.45, 0.05, 2, 2) ;  // Should get point [2], interval [0.5, 0.5]
      test_read(s, 0.45, 0.01, 3, 2) ;  // Should get empty result set
      test_read(s, 1.05, 0.01, 3, 2) ;  // Should get empty result set
      test_read(s, -0.5, 1.0,  0, 2) ;  //
      test_read(s, -0.5, 0.5,  0, 0) ;  //
      test_read(s, -0.2, 0.1,  1, 0) ;  //

//      for (auto const &x : d->data()) std::cout << "  DATA: " << x << std::endl ;
//      for (auto const &t : d->times()) std::cout << "  TIME: " << t << std::endl ;

//      s = hdf5.get_signal("http://ex.org/recording/1") ;
//      d = s->read() ;    // (0), (0, 100)  (0.01, 1.0)
      }

    hdf5.close() ;

    }
  catch (bsml::HDF5::Exception e) {   // bsml::Exception  ???

    std::cout << "EXCEPTION: " << e.what() << std::endl ;

    }
  }
