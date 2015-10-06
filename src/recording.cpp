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

#include <biosignalml/recording.h>


bsml::Clock::Pointer bsml::Recording::get_clock(const std::string &uri)
/*-------------------------------------------------------------------*/
{
  return get_object<bsml::Clock>(uri, this->clock_set()) ;
  }


bsml::Signal::Pointer bsml::Recording::get_signal(const std::string &uri)
/*---------------------------------------------------------------------*/
{
  return get_object<bsml::Signal>(uri, this->signal_set()) ;
  }
