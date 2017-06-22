/******************************************************************************
 *                                                                            *
 *  BioSignalML Management in C++                                             *
 *                                                                            *
 *  Copyright (c) 2010-2012  David Brooks                                     *
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

#include <iostream>
#include <vector>

#include <biosignalml/data/hdf5.h>
#include "data/hdf5impl.h"


using namespace bsml ;


int main(void)
{
  try {
    const std::string uri = "Test URI" ;
    const std::string file = "t2.h5" ;
    auto h = HDF5::File::create(uri, file, true) ;

    double testdata[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 } ;
    std::vector<hsize_t> dshape(2) ;
    dshape[0] = 2 ;
    dshape[1] = 3 ;
// ??? std::cout << h.createSignal<int>("signal 1", "mV", NULL, NULL, 1.0, 0.0, 1000.0) << std::endl ;
    auto s = h->create_signal("signal 2", "mV", testdata, 6, dshape, 1.0, 0.0, 2000.0) ;
//    std::cout << s.name() << std::endl ;
//    testdata[0] += 1.0 ;
//    s->extend(testdata) ;
//    testdata[0] += 1.0 ;
//    s->extend(testdata) ;
//    testdata[0] += 1.0 ;
//    s->extend(testdata) ;
//    testdata[0] += 1.0 ;
//    s->extend(testdata) ;
   
//    h.createSignal<int>("signal 2", "mV") ;
    
//    h.createSignal<int>("signal 3", "mV", testdata, 360.0) ;

    auto c = h->create_clock("clock1", "s", testdata, 6) ;
//    std::cout << c.name() << std::endl ;
//    c->extend(testdata) ;
//    h->store_metadata("metadata", "format") ;
    h->close() ;

    delete h ;

//    HDF5::file r = HDF5::File::open("t2.h5") ;
//    std::cout << r.get_uri() << std::endl ;
//    r.close() ;
    }
  catch (HDF5::Exception e) {
    std::cerr << "EXCEPTION: " << e.what() << "\n" ;
    }
  }

