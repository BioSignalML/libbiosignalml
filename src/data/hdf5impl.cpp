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

#include <stdlib.h>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <list>
#include <string.h>    // For memcpy() and strcmp()

#include <biosignalml/data/hdf5.h>
#include "hdf5impl.h"


/** New (HDF5 1.10 SWMR feature allows single writer, multiple readers... **/

using namespace bsml ;


// Declare C-style callbacks
extern "C" {
  static herr_t save_signal(hid_t, const char *, void *) ;
  static herr_t save_clock(hid_t, const char *, void *) ;
  } ;

// Data passed to callback
typedef struct {
  H5::H5File h5file ;
  void *listp ;
  } SaveInfo ;

static char *copystring(const std::string &s)
/*-----------------------------------------*/
{
  char *t = (char *)malloc(s.length() + 1) ;
  memcpy(t, s.c_str(), s.length() + 1) ;
  return t ;
  }


HDF5::Dataset::Dataset()
/*--------------------*/
: m_uri(""),
  m_dataset(H5::DataSet()),
  m_reference(0),
  m_index(-1)
{
  }



// Check that the HDF5 dataset (given by `dataref`) has the given `uri` and if
// an array, set `m_index` to the index of the uri in the array.
HDF5::Dataset::Dataset(const std::string &uri, const HDF5::DatasetRef &dataref)
/*---------------------------------------------------------------------------*/
: m_uri(uri),
  m_dataset(dataref.first),
  m_reference(dataref.second)
{
  int index = -1 ;
  H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
  try {
    H5::Attribute attr = m_dataset.openAttribute("uri") ;
    int nsignals = attr.getSpace().getSimpleExtentNpoints() ;
    if (nsignals == 1) {
      std::string ds_uri = "" ;
      attr.read(varstr, ds_uri) ;
      if (uri != ds_uri) throw HDF5::Exception("Dataset URI '" + ds_uri + "' should be '" + uri + "'") ;
      }
    else if (uri != "") {
      char **uris = (char **)calloc(nsignals, sizeof(char *)) ;
      attr.read(varstr, uris) ;
      int ds_index = -1 ;
      int n = 0 ;
      while (n < nsignals) {
        if (ds_index == -1 && strcmp(uri.c_str(), uris[n]) == 0) {
          m_uri = uri ;
          ds_index = n ;
          }
        free(uris[n]) ;
        ++n ;
        }
      free(uris) ;

      if (ds_index == -1) throw HDF5::Exception("Cannot find dataset for '" + uri + "'") ;
      else if (index < 0) index = ds_index ;
      else if (index != ds_index) throw HDF5::Exception("Corrupt dataset reference for '" + uri + "'") ;
      }
    }
  catch (H5::AttributeIException e) {
    if (index < 0) {
      H5::Attribute attr = m_dataset.createAttribute("uri", varstr, H5::DataSpace(H5S_SCALAR)) ;
      attr.write(varstr, uri) ;
      }
    }
  m_index = index ;
  }

HDF5::Dataset::~Dataset()
/*---------------------*/
{
  close() ;
  }


void HDF5::Dataset::close(void)
/*---------------------------*/
{
  if (m_index == -1) m_dataset.close() ;
  }


H5::DataSet HDF5::Dataset::get_dataset(void) const
/*----------------------------------------------*/
{
  return m_dataset ;
  }

hobj_ref_t HDF5::Dataset::get_reference(void) const
/*-----------------------------------------------*/
{
  return m_reference ;
  }


size_t HDF5::Dataset::size(void) const
/*----------------------------------*/
{
  if (m_dataset.getId() < 0) return 0 ;
  else {
    H5::DataSpace dspace = m_dataset.getSpace() ;
    hsize_t *shape = (hsize_t *)calloc(dspace.getSimpleExtentNdims(), sizeof(hsize_t)) ;
    dspace.getSimpleExtentDims(shape) ;
    hsize_t result = shape[0] ;
    free(shape) ;
    return result ;
    }
  }

std::string HDF5::Dataset::name(void) const
/*---------------------------------------*/
{
  int n = H5Iget_name(m_dataset.getId(), NULL, 0) ;
  if (n == 0) return std::string("") ;
  char *name = (char *)calloc(n+1, sizeof(char)) ;
  H5Iget_name(m_dataset.getId(), name, n+1) ;
  std::string result(name) ;
  free(name) ;
  return result ;
  }

int64_t HDF5::Dataset::clock_size(void)
/*-----------------------------------*/
{
  try {
    H5::Attribute attr = m_dataset.openAttribute("clock") ;
    hobj_ref_t ref ;
    attr.read(H5::PredType::STD_REF_OBJ, &ref) ;
    attr.close() ;
    H5::DataSet clk(H5Rdereference(H5Iget_file_id(m_dataset.getId()), H5R_OBJECT, &ref)) ;
    H5::DataSpace cspace = clk.getSpace() ;
    int cdims = cspace.getSimpleExtentNdims() ;
    hsize_t *cshape = (hsize_t *)calloc(cdims, sizeof(hsize_t)) ;
    cspace.getSimpleExtentDims(cshape) ;
    hsize_t result = cshape[0] ;
    free(cshape) ;
    return result ;
    }
  catch (H5::AttributeIException e) { }
  return -1 ;
  }


void HDF5::Dataset::extend(const double *data, intmax_t size, int nsignals)
/*-----------------------------------------------------------------------*/
{

  int64_t clocksize = this->clock_size() ;

  H5::DataSpace dspace = m_dataset.getSpace() ;
  int ndims = dspace.getSimpleExtentNdims() ;
  hsize_t
    *shape = (hsize_t *)calloc(ndims, sizeof(hsize_t)),
    *newshape = (hsize_t *)calloc(ndims, sizeof(hsize_t)),
    *count = (hsize_t *)calloc(ndims, sizeof(hsize_t)),
    *start = (hsize_t *)calloc(ndims, sizeof(hsize_t)) ;
  try {
    dspace.getSimpleExtentDims(shape) ;
    if (nsignals > 1) {         // compound dataset
      if (ndims != 2) throw HDF5::Exception("Compound dataset has wrong shape: " + m_uri) ;
      start[1] = 0 ;
      count[0] = size/nsignals ;
      newshape[1] = count[1] = shape[1] ;
      }
    else {                      // simple dataset
      count[0] = size ;
      for (int n = 1 ;  n < ndims ;  ++n) {
        start[n] = 0 ;
        count[0] /= shape[n] ;
        newshape[n] = count[n] = shape[n] ;
        }
      }
    start[0] = shape[0] ;
    newshape[0] = shape[0] + count[0] ;
    if (clocksize >= 0 && clocksize < newshape[0])
      throw HDF5::Exception("Clock for '" + m_uri + "' doesn't have sufficient times") ;
    m_dataset.extend(newshape) ;
    dspace = m_dataset.getSpace() ;
    dspace.selectHyperslab(H5S_SELECT_SET, count, start) ; // Starting at 'start' for 'count'
    H5::DataSpace mspace(ndims, count, count) ;
    m_dataset.write(data, H5::PredType::NATIVE_DOUBLE, mspace, dspace) ;
    }
  catch (H5::DataSetIException e) {
    throw HDF5::Exception("Cannot extend dataset '" + m_uri + "': " + e.getDetailMsg()) ;
    }
  free(shape) ;
  free(newshape) ;
  free(count) ;
  free(start) ;
  }


std::vector<double> HDF5::Dataset::read(size_t pos, intmax_t size)
/*--------------------------------------------------------------*/
{
  std::vector<double> points ;

  H5::DataSpace dspace = m_dataset.getSpace() ;
  int ndims = dspace.getSimpleExtentNdims() ;
  hsize_t
    *shape = (hsize_t *)calloc(ndims, sizeof(hsize_t)),
    *count = (hsize_t *)calloc(ndims, sizeof(hsize_t)),
    *start = (hsize_t *)calloc(ndims, sizeof(hsize_t)) ;
  try {
    dspace.getSimpleExtentDims(shape) ;
    if (size < 0 || (size + pos) > shape[0]) size = shape[0] - pos ;
    points.resize(size) ;
    start[0] = pos ;
    if (m_index >= 0) {         // compound dataset
      if (ndims != 2) throw HDF5::Exception("Compound dataset has wrong shape: " + m_uri) ;
      count[0] = size ;
      count[1] = 1 ;
      start[1] = m_index ;
      }
    else {                      // simple dataset
      count[0] = size ;
      for (int n = 1 ;  n < ndims ;  ++n) {
        size *= shape[n] ;
        count[n] = shape[n] ;
        start[n] = 0 ;
        }
      }
    dspace = m_dataset.getSpace() ;
    dspace.selectHyperslab(H5S_SELECT_SET, count, start) ;
    H5::DataSpace mspace(ndims, count, count) ;

    points = std::vector<double>(size) ;
    m_dataset.read((void *)points.data(), H5::PredType::NATIVE_DOUBLE, mspace, dspace) ;
    }
  catch (H5::DataSetIException e) {
    throw HDF5::Exception("Cannot read dataset '" + m_uri + "': " + e.getDetailMsg()) ;
    }
  free(shape) ;
  free(count) ;
  free(start) ;

  return points ;
  }


HDF5::File::File(H5::H5File h5file, const std::string &uri)
/*-------------------------------------------------------*/
: m_h5file(h5file), m_uri(uri), m_closed(false)
{
  }

HDF5::File::~File(void)
/*-------------------*/
{
  if (!m_closed) close() ;
  }

void HDF5::File::close(void)
/*------------------------*/
{
  m_h5file.close() ;
  m_closed = true ;
  }


const std::string HDF5::File::get_uri(void) const
/*---------------------------------------------*/
{
  return m_uri ;
  }


HDF5::File *HDF5::File::create(const std::string &uri, const std::string &fname, bool replace)
/*------------------------------------------------------------------------------------------*/
{
//Create a new HDF5 Recording file.
//
//:param uri: The URI of the Recording contained in the file.
//:param fname: The name of the file to create.
//:type fname: str
//:param replace: If True replace any existing file (default = False).
//:param replace: bool
  try {
#if !H5_DEBUG
    H5::Exception::dontPrint() ;
#endif
    H5::H5File h5file = H5::H5File(fname, replace ? H5F_ACC_TRUNC : H5F_ACC_EXCL) ;

    H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
    H5::DataSpace scalar(H5S_SCALAR) ;
    H5::Attribute attr ;

    H5::Group root = h5file.openGroup("/") ;
    H5::Group uris = h5file.createGroup("/uris") ;
    H5::Group rec = h5file.createGroup("/recording") ;
    h5file.createGroup("/recording/signal") ;
    
    attr = root.createAttribute("version", varstr, scalar) ;
    attr.write(varstr, BSML_H5_VERSION) ;
    attr.close() ;

    attr = rec.createAttribute("uri", varstr, scalar) ;
    attr.write(varstr, uri) ;
    attr.close() ;

    hobj_ref_t ref ;
    attr = uris.createAttribute(uri, H5::PredType::STD_REF_OBJ, scalar) ;
    h5file.reference(&ref, "/recording") ;
    attr.write(H5::PredType::STD_REF_OBJ, &ref) ;
    attr.close() ;

    h5file.flush(H5F_SCOPE_GLOBAL) ;
    return new HDF5::File(h5file, uri) ;
    }
  catch (H5::FileIException e) {
  // Need to remove any file... (only if replace ??)
    throw HDF5::IOError("Cannot create '" + fname + "': " + e.getDetailMsg()) ;
    }
  }


HDF5::File *HDF5::File::open(const std::string &fname, bool readonly)
/*-----------------------------------------------------------------*/
{
//Open an existing HDF5 Recording file.
//
//:param fname: The name of the file to open.
//:type fname: str
//:param readonly: If True don't allow updates (default = False).
//:param readonly: bool
  H5::H5File h5file ;
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  try {
    std::string fn = (!fname.compare(0, 7, "file://")) ? fname.substr(7) : fname ;
    h5file = H5::H5File(fn, readonly ? H5F_ACC_RDONLY : H5F_ACC_RDWR) ;
    }
  catch (H5::FileIException e) {
    throw HDF5::IOError("Cannot open '" + fname + "': " + e.getDetailMsg()) ;
    }

  H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
  H5::Attribute attr ;
  try {
    std::string version ;
    H5::Group root = h5file.openGroup("/") ;
    attr = root.openAttribute("version") ;
    attr.read(varstr, version) ;
    if (version.compare(0, 5, BSML_H5_VERSION, 0, 5))
      throw HDF5::Exception("Invalid BiosignalML HDF5 file: '" + fname + "'") ;
    size_t dot = version.find('.', 5) ;
    int h5major, major, minor ;
    if (!(dot != std::string::npos
       && (std::stringstream(version.substr(5, dot)) >> major)
       && (std::stringstream(version.substr(dot + 1)) >> minor)
       && (std::stringstream(BSML_H5_MAJOR) >> h5major)
       && major <= h5major))
      throw HDF5::Exception("File '" + fname + "' not compatible with version " + BSML_H5_VERSION) ;

    H5::Group rec, uris ;
    char *uri = nullptr ;

    uris = h5file.openGroup("/uris") ;
    rec = h5file.openGroup("/recording") ;
    h5file.openGroup("/recording/signal") ;

    attr = rec.openAttribute("uri") ;
    attr.read(varstr, &uri) ;
    attr.close() ;

    attr = uris.openAttribute(uri) ;
    hobj_ref_t uriref, recref ;
    attr.read(H5::PredType::STD_REF_OBJ, &uriref) ;
    h5file.reference(&recref, "/recording") ;
    attr.close() ;
    if (uriref != recref)
      throw HDF5::Exception("Internal error in BioSignalML HDF5 file: '" + fname + "'") ;
    return new HDF5::File(h5file, std::string(uri)) ;
    }
  catch (H5::Exception e) {
    throw HDF5::Exception("Invalid BioSignalML HDF5 file: '" + fname + "': " + e.getDetailMsg()) ;
    }
  }


HDF5::ClockData::Ptr HDF5::File::check_timing(double rate, const std::string &uri, size_t npoints)
/*----------------------------------------------------------------------------------------------*/
{
  HDF5::ClockData::Ptr dataset = nullptr ;
  if (rate != 0.0) {
    if (uri != "") throw HDF5::Exception("Only one of 'rate' or 'clock' can be specified") ;
    }
  else if (uri != "") {
    dataset = get_clock(uri) ;
    if (dataset == nullptr)
      throw HDF5::Exception("Clock doesn't exist") ;
    else if (dataset->size() < npoints)
      throw HDF5::Exception("Clock either doesn't have sufficient times") ;
    }
  else {
    throw HDF5::Exception("No timing information given") ;
    }
  return dataset ;
  }


HDF5::DatasetRef HDF5::File::create_dataset(const std::string &group,
/*-----------------------------------------------------------------*/
                 int rank, hsize_t *shape, hsize_t *maxshape, const double *data)
{
  H5Compression compression = BSML_H5_DEFAULT_COMPRESSION ;    /*** OPTION ***/
  H5::DataSpace dspace(rank, shape, maxshape) ;
  H5::DataType dtype = H5::PredType::IEEE_F64LE ;          /*** Create option ?? ***/
  if (dtype.getId() == 0) dtype = BSML_H5_DEFAULT_DATATYPE ;

  H5::Group grp ;
  try {
    grp = m_h5file.openGroup("/recording/" + group) ;
    }
  catch (H5::FileIException e) {
    grp = m_h5file.createGroup("/recording/" + group) ;
    }
  std::string dsetname = "/recording/" + group + "/" + std::to_string(grp.getNumObjs()) ;
  hsize_t *chunks = (hsize_t *)calloc(rank, sizeof(hsize_t)) ;
  H5::DataSet dset ;
  hobj_ref_t reference ;
  try {
    H5::DSetCreatPropList props ;
    chunks[0] = 4096 ;            /***** MAGIC NUMBER ****/
    int chunkbytes = chunks[0]*dtype.getSize() ;
    for (int n = 1 ;  n < rank ;  ++n) {
      chunkbytes *= shape[n] ;
      chunks[n] = shape[n] ;
      }
    while (chunkbytes < BSML_H5_CHUNK_BYTES) {
      chunks[0] *= 2 ;
      chunkbytes *= 2 ;
      }
    props.setChunk(rank, chunks) ;
    if      (compression == BSML_H5_COMPRESS_GZIP) props.setDeflate(4) ;
    else if (compression == BSML_H5_COMPRESS_SZIP) props.setSzip(H5_SZIP_NN_OPTION_MASK, 8) ;
    dset = m_h5file.createDataSet(dsetname, dtype, dspace, props) ;
    if (data != nullptr) dset.write(data, H5::PredType::NATIVE_DOUBLE) ;
    m_h5file.reference(&reference, dsetname) ;
    }
  catch (H5::FileIException e) {
    e.printError() ;
    throw HDF5::Exception("Cannot create '" + group + "' dataset: " + e.getDetailMsg()) ;
    }
  free(chunks) ;
  return HDF5::DatasetRef(dset, reference) ;
  }


void HDF5::File::set_signal_attributes(const H5::DataSet &dset, double gain, double offset,
/*---------------------------------------------------------------------------------------*/
 double rate, const std::string &timeunits, const HDF5::ClockData::Ptr &clock)
{
  H5::Attribute attr ;
  H5::DataSpace scalar(H5S_SCALAR) ;

  if (gain != 1.0) {
    attr = dset.createAttribute("gain", H5::PredType::IEEE_F64LE, scalar) ;
    attr.write(H5::PredType::NATIVE_DOUBLE, &gain) ;
    attr.close() ;
    }
  if (offset != 0.0) {
    attr = dset.createAttribute("offset", H5::PredType::IEEE_F64LE, scalar) ;
    attr.write(H5::PredType::NATIVE_DOUBLE, &offset) ;
    attr.close() ;
    }

  if (rate != 0.0) {
    attr = dset.createAttribute("rate", H5::PredType::IEEE_F64LE, scalar) ;
    attr.write(H5::PredType::NATIVE_DOUBLE, &rate) ;
    attr.close() ;
    }
  else if (clock != nullptr) {
    attr = dset.createAttribute("clock", H5::PredType::STD_REF_OBJ, scalar) ;
    hobj_ref_t ref = clock->get_reference() ;
    attr.write(H5::PredType::STD_REF_OBJ, &ref) ;
    attr.close() ;
    }
  if (timeunits != "") {
    H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
    attr = dset.createAttribute("timeunits", varstr, scalar) ;
    attr.write(varstr, timeunits) ;
    attr.close() ;
    }
  }


HDF5::SignalData::Ptr HDF5::File::create_signal(const std::string &uri, const std::string &units,
/*---------------------------------------------------------------------------------------------*/
 const double *data, size_t datasize, std::vector<hsize_t> datashape,
 double gain, double offset, double rate, HDF5::ClockData::Ptr clock)
{
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  H5::Attribute attr ;
  H5::Group urigroup = m_h5file.openGroup("/uris") ;
  try {
    attr = urigroup.openAttribute(uri) ;
    attr.close() ;
    throw HDF5::Exception("Signal's URI '" + uri + "' already specified") ;
    }
  catch (H5::AttributeIException e) { }

  size_t npoints = 0 ;
  int rank = datashape.size() + 1 ;
  hsize_t *shape = (hsize_t *)calloc(rank, sizeof(hsize_t)) ;
  hsize_t *maxshape = (hsize_t *)calloc(rank, sizeof(hsize_t)) ;
  maxshape[0] = H5S_UNLIMITED ;
  int elsize = 1 ;
  if (rank > 1) {           // simple dataset, shape of data point given
    for (int n = 0 ;  n < (rank - 1) ;  ++n) {
      int count = datashape.at(n) ;
      maxshape[n + 1] = shape[n + 1] = count ;
      elsize *= count ;
      }
    npoints = (data != nullptr) ? datasize/elsize : 0 ;
    }
  else if (data != nullptr) {  // simple dataset, data determines shape
    npoints = datasize ;
    }
  shape[0] = npoints ;
  HDF5::DatasetRef sigdata = create_dataset("signal", rank, shape, maxshape, data) ;
  free(maxshape) ;
  free(shape) ;
  H5::DataSet dset = sigdata.first ;

  H5::DataSpace scalar(H5S_SCALAR) ;
  H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
  hobj_ref_t reference = sigdata.second ;

  try {
    attr = urigroup.createAttribute(uri, H5::PredType::STD_REF_OBJ, scalar) ;
    attr.write(H5::PredType::STD_REF_OBJ, &reference) ;
    attr.close() ;

    attr = dset.createAttribute("uri", varstr, scalar) ;
    attr.write(varstr, uri) ;
    attr.close() ;

    attr = dset.createAttribute("units", varstr, scalar) ;
    attr.write(varstr, units) ;
    attr.close() ;

    set_signal_attributes(dset, gain, offset, rate, "", clock) ;
    }
  catch (H5::AttributeIException e) {
    throw HDF5::Exception("Cannot set signal's attributes: " + e.getDetailMsg()) ;
    }

  m_h5file.flush(H5F_SCOPE_GLOBAL) ;
  return std::make_shared<HDF5::SignalData>(uri, sigdata) ;
  }


HDF5::SignalData::Ptr HDF5::File::create_signal(const std::vector<std::string> &uris,
/*---------------------------------------------------------------------------------*/
 const std::vector<std::string> &units, const double *data, size_t datasize,
 double gain, double offset, double rate, HDF5::ClockData::Ptr clock)
{
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  if (uris.size() != units.size()) throw HDF5::Exception("'uri' and 'units' have different sizes") ;
  int nsignals = uris.size() ;

  if (nsignals == 1) {
    return
      create_signal(uris[0], units[0], data, datasize, std::vector<hsize_t>(), gain, offset, rate, clock) ;
    }
  H5::Attribute attr ;
  H5::Group urigroup = m_h5file.openGroup("/uris") ;
  int n ;

  for (n = 0 ;  n < nsignals ;  ++n) {
    try {
      attr = urigroup.openAttribute(uris[n]) ;
      attr.close() ;
      throw HDF5::Exception("Signal's URI '" + uris[n] + "' already specified") ;
      }
    catch (H5::AttributeIException e) { }
    }
  size_t npoints = (data != nullptr) ? datasize/nsignals : 0 ;
  hsize_t maxshape[2] = { H5S_UNLIMITED, (hsize_t)nsignals } ;
  hsize_t shape[2]    = { npoints,       (hsize_t)nsignals } ;

  HDF5::DatasetRef sigdata = create_dataset("signal", 2, shape, maxshape, data) ;
  H5::DataSet dset = sigdata.first ;

  H5::DataSpace scalar(H5S_SCALAR) ;
  H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
  hobj_ref_t reference = sigdata.second ;
  const char **values= (const char **)calloc(nsignals, sizeof(char *)) ;
  hsize_t dims[1] ;
  dims[0] = nsignals ;
  H5::DataSpace attrspace(1, dims, dims) ;

  try {
    for (n = 0 ;  n < nsignals ;  ++n) {
      values[n] = copystring(uris[n]) ;
      attr = urigroup.createAttribute(uris[n], H5::PredType::STD_REF_OBJ, scalar) ;
      attr.write(H5::PredType::STD_REF_OBJ, &reference) ;
      attr.close() ;
      }
    attr = dset.createAttribute("uri", varstr, attrspace) ;
    attr.write(varstr, values) ;
    attr.close() ;
    attr = dset.createAttribute("units", varstr, attrspace) ;
    for (n = 0 ;  n < nsignals ;  ++n) {
      free((void *)values[n]) ;    // Has URI from above
      values[n] = copystring(units[n]) ;
      }
    attr.write(varstr, values) ;
    attr.close() ;

    set_signal_attributes(dset, gain, offset, rate, "", clock) ;
    }
  catch (H5::AttributeIException e) {
    throw HDF5::Exception("Cannot set signal's attributes: " + e.getDetailMsg()) ;
    }

  for (n = 0 ;  n < nsignals ;  ++n)
    if (values[n]) free((void *)values[n]) ;
  free(values) ;

  m_h5file.flush(H5F_SCOPE_GLOBAL) ;
  return std::make_shared<HDF5::SignalData>("", sigdata) ;
  }


HDF5::ClockData::Ptr HDF5::File::create_clock(const std::string &uri, const std::string &units,
/*-------------------------------------------------------------------------------------------*/
                                          double rate, const double *data, size_t datasize)
{
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  H5::Attribute attr ;
  H5::Group urigroup = m_h5file.openGroup("/uris") ;
  try {
    attr = urigroup.openAttribute(uri) ;
    attr.close() ;
    throw HDF5::Exception("Clock's URI '" + uri + "' already specified") ;
    }
  catch (H5::AttributeIException e) { }

  if (datasize > 0 && rate != 0.0)
    throw HDF5::Exception("A Clock cannot have both 'times' and a 'rate'") ;
  
  hsize_t maxshape[] = { H5S_UNLIMITED } ;
  hsize_t shape[]    = { datasize } ;
  HDF5::DatasetRef clkdata = create_dataset("clock", 1, shape, maxshape, data) ;
  H5::DataSet dset = clkdata.first ;

  H5::DataSpace scalar(H5S_SCALAR) ;
  hobj_ref_t reference = clkdata.second ;

  try {
    attr = urigroup.createAttribute(uri, H5::PredType::STD_REF_OBJ, scalar) ;
    attr.write(H5::PredType::STD_REF_OBJ, &reference) ;
    attr.close() ;

    H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;

  // This could go into HDF5::ClockData's constructor...
    attr = dset.createAttribute("units", varstr, scalar) ;
    attr.write(varstr, units) ;
    attr.close() ;
    if (rate != 0.0) {
      attr = dset.createAttribute("rate", H5::PredType::IEEE_F64LE, scalar) ;
      attr.write(H5::PredType::NATIVE_DOUBLE, &rate) ;
      attr.close() ;
      }
    }
  catch (H5::AttributeIException e) {
    throw HDF5::Exception("Cannot set clock's attributes: " + e.getDetailMsg()) ;
    }

  m_h5file.flush(H5F_SCOPE_GLOBAL) ;
  return std::make_shared<HDF5::ClockData>(uri, clkdata) ;
  }

HDF5::ClockData::Ptr HDF5::File::create_clock(const std::string &uri, const std::string &units,
/*-------------------------------------------------------------------------------------------*/
                                          const double *data, size_t datasize)
{
  return create_clock(uri, units, 0.0, data, datasize) ;
  }


HDF5::DatasetRef HDF5::File::get_dataref(const std::string &uri, const std::string &prefix)
/*---------------------------------------------------------------------------------------*/
{
  try {
    H5::Group uris = m_h5file.openGroup("/uris") ;
    H5::Attribute attr = uris.openAttribute(uri) ;
    hobj_ref_t ref ;
    attr.read(H5::PredType::STD_REF_OBJ, &ref) ;
    attr.close() ;

    H5::DataSet dset = H5::DataSet(m_h5file, &ref) ;
    hid_t id = dset.getId() ;

    char buf[8] ;                                         // HDF5 bug, must be at least 2 long
    int len = H5Rget_name(id, H5R_OBJECT, &ref, buf, 8) ; // HDF5 bug, cannot have NULL name
    char *name = (char *)malloc(len + 1) ;
    H5Rget_name(id, H5R_OBJECT, &ref, name, len + 1) ;

    bool matched = (prefix.compare(0, std::string::npos, name, prefix.size()) == 0) ;
    free(name) ;
    if (matched) return HDF5::DatasetRef(dset, ref) ;
    }
  catch (H5::AttributeIException e) { }

  return HDF5::DatasetRef() ;
  }



HDF5::SignalData::Ptr HDF5::File::get_signal(const std::string &uri)
/*----------------------------------------------------------------*/
{
//Find a signal from its URI.
//
//:param uri: The URI of the signal to get.
//:return: A :class:`HDF5::SignalData` containing the signal, or None if
//         the URI is unknown or the dataset is not that for a signal.
  HDF5::DatasetRef dataref = get_dataref(uri, "/recording/signal/") ;
  if (!dataref.valid()) throw HDF5::Exception("Cannot find signal: " + uri) ;
  return HDF5::SignalData::get_signal(uri, dataref) ;
  }


static herr_t save_signal(hid_t id, const char *name, void *op_data)
/*---------------------------------------------------------------*/
{
  SaveInfo *info = (SaveInfo *)op_data ;
  std::list<HDF5::SignalData::Ptr> &sig
   = reinterpret_cast<std::list<HDF5::SignalData::Ptr> &>(info->listp) ;
  try {
    H5::DataSet dset = H5::DataSet(info->h5file.openDataSet(name)) ;

    hobj_ref_t ref ;
    info->h5file.reference(&ref, name) ;
    HDF5::DatasetRef dataref = HDF5::DatasetRef(dset, ref) ;
    H5::Attribute attr = dset.openAttribute("uri") ;
    int nsignals = attr.getSpace().getSimpleExtentNpoints() ;
    H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
    if (nsignals == 1) {
      std::string uri ;
      attr.read(varstr, uri) ;
      sig.push_back(HDF5::SignalData::get_signal(uri, dataref)) ;
      }
    else if (nsignals > 1) {
      char *uris[nsignals] ;
      attr.read(varstr, uris) ;
      int n = 0 ;
      while (n < nsignals) {
        sig.push_back(HDF5::SignalData::get_signal(std::string(uris[n]), dataref)) ;
        free(uris[n]) ;
        ++n ;
        }
      }
    }
  catch (H5::FileIException e) { }
  return 0 ;
  }


std::list<HDF5::SignalData::Ptr> HDF5::File::get_signals(void)
/*----------------------------------------------------------*/
{
//Return all signals in the recording.
//
//:rtype: list of :class:`HDF5::SignalData`
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  std::list<HDF5::SignalData::Ptr> signals ;
  const std::string name = "/recording/signal" ;
  m_h5file.iterateElems(name, nullptr, save_signal, (void *)&signals) ;
  return signals ;
  }


HDF5::ClockData::Ptr HDF5::File::get_clock(const std::string &uri)
/*--------------------------------------------------------------*/
{
//Find a clock dataset from its URI.
//
//:param uri: The URI of the clock dataset to get.
//:return: A :class:`HDF5::ClockData` or None if the URI is unknown or
//         the dataset is not that for a clock.
  HDF5::DatasetRef dataref = get_dataref(uri, "/recording/clock/") ;
  if (!dataref.valid()) throw HDF5::Exception("Cannot find clock: " + uri) ;
  return HDF5::ClockData::get_clock(uri, dataref) ;
  }


static herr_t save_clock(hid_t id, const char *name, void *op_data)
/*---------------------------------------------------------------*/
{
  SaveInfo *info = (SaveInfo *)op_data ;
  std::list<HDF5::ClockData::Ptr> clk
   = reinterpret_cast<std::list<HDF5::ClockData::Ptr> &>(info->listp) ;
  try {
    H5::DataSet dset = H5::DataSet(info->h5file.openDataSet(name)) ;

    hobj_ref_t ref ;
    info->h5file.reference(&ref, name) ;
    H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
    H5::Attribute attr = dset.openAttribute("uri") ;
    std::string uri ;
    attr.read(varstr, uri) ;
    clk.push_back(HDF5::ClockData::get_clock(uri, HDF5::DatasetRef(dset, ref))) ;
    }
  catch (H5::FileIException e) { }
  return 0 ;
  }


std::list<HDF5::ClockData::Ptr> HDF5::File::get_clocks(void)
/*--------------------------------------------------------*/
{
//Return all clocks in the recording.
//
//:rtype: list of :class:`HDF5::ClockData`
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  std::list<HDF5::ClockData::Ptr> clocks ;
  const std::string name = "/recording/clock" ;
  m_h5file.iterateElems(name, nullptr, save_clock, (void *)&clocks) ;
  return clocks ;
  }


void HDF5::File::store_metadata(const std::string &metadata, const std::string &mimetype)
/*-------------------------------------------------------------------------------------*/
{
//Store metadata in the HDF5 recording.
//
//:param metadata: RDF serialised as a string.
//:type metadata: str or unicode
//:param mimetype: A mimetype string for the RDF format used.
//
//NOT C++:  Metadata is encoded as UTF-8 when stored. ****
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  try {
    m_h5file.unlink("/metadata") ;
    }
  catch (H5::FileIException e) { }
  H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
  H5::DataSpace scalar(H5S_SCALAR) ;
  H5::DataSet md = m_h5file.createDataSet("/metadata", varstr, scalar) ;
  md.write(metadata, varstr, scalar) ;
  H5::Attribute attr = md.createAttribute("mimetype", varstr, scalar) ;
  attr.write(varstr, mimetype) ;
  md.close() ;
  m_h5file.flush(H5F_SCOPE_GLOBAL) ;
  }


std::pair<std::string, std::string> HDF5::File::get_metadata(void)
/*--------------------------------------------------------------*/
{
//:return: A 2-tuple of retrieved metadata and mimetype, or
//         (None, None) if the recording has no '/metadata' dataset.
#if !H5_DEBUG
  H5::Exception::dontPrint() ;
#endif
  try {
    H5::DataSet md = m_h5file.openDataSet("/metadata") ;
    H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
    H5::DataSpace scalar(H5S_SCALAR) ;
    std::string metadata ;
    md.read(metadata, varstr, scalar) ;
    H5::Attribute attr = md.openAttribute("mimetype") ;
    std::string mimetype ;
    attr.read(varstr, mimetype) ;
    return std::make_pair(std::string(metadata), std::string(mimetype)) ;
    }
  catch (H5::FileIException e) { }
  return std::make_pair("", "") ;
  }



HDF5::ClockData::ClockData()
/*------------------------*/
: HDF5::Dataset()
{
  }

HDF5::ClockData::ClockData(const std::string &uri, const HDF5::DatasetRef &ds)
/*--------------------------------------------------------------------------*/
: HDF5::Dataset(uri, ds)
{
  }

HDF5::ClockData::Ptr HDF5::ClockData::get_clock(const std::string &uri, const HDF5::DatasetRef &dataref)
/*----------------------------------------------------------------------------------------------------*/
{
  H5::DataSet dset = dataref.first ;
  H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;
  H5::Attribute attr = dset.openAttribute("units") ;
  std::string units ;
  attr.read(varstr, units) ;

  double rate = 0.0 ;
  try {
    attr = dset.openAttribute("rate") ;
    attr.read(H5::PredType::NATIVE_DOUBLE, &rate) ;
    attr.close() ;
    }
  catch (H5::AttributeIException e) { }

  if (rate != 0.0) return std::make_shared<HDF5::ClockData>(uri, dataref) ;

// Actually reading data points needs to be a separate method... 
//  size_t size = dset.getSpace().getSimpleExtentNpoints() ;
//  std::vector<double> times(size) ;
//  dset.read((void *)(times.data()), H5DataTypes(&rate).mtype) ;

  return std::make_shared<HDF5::ClockData>(uri, dataref) ;
  }




HDF5::SignalData::SignalData()
/*==========================*/
: HDF5::Dataset()
{
  }


HDF5::SignalData::SignalData(const std::string &uri, const HDF5::DatasetRef &ds)
/*----------------------------------------------------------------------------*/
: HDF5::Dataset(uri, ds)
{
  }


HDF5::SignalData::Ptr HDF5::SignalData::get_signal(const std::string &uri, const HDF5::DatasetRef &dataref)
/*-------------------------------------------------------------------------------------------------------*/
{
  H5::DataSet dset = dataref.first ;
  H5::StrType varstr(H5::PredType::C_S1, H5T_VARIABLE) ;

  std::string units ;
  double rate = 0.0 ;

  try {
    H5::Attribute attr = dset.openAttribute("units") ;
    attr.read(varstr, units) ;
    attr.close() ;

    attr = dset.openAttribute("rate") ;
    attr.read(H5::PredType::NATIVE_DOUBLE, &rate) ;
    attr.close() ;
    }
  catch (H5::AttributeIException e) { }
  // Could have a `clock` attribute instead of `rate`.

  if (rate != 0.0) return std::make_shared<HDF5::SignalData>(uri, dataref) ;
  return std::make_shared<HDF5::SignalData>(uri, dataref) ;
  }


int HDF5::SignalData::signal_count(void)
/*------------------------------------*/
{
  H5::DataSpace uspace = m_dataset.openAttribute("uri").getSpace() ;
  int udims = uspace.getSimpleExtentNdims() ;
  if (udims == 0) return 1 ;  // SCALAR
  else {
    if (udims != 1) throw HDF5::Exception("Dataset's 'uri' attribute has wrong shape: " + m_uri) ;
    return uspace.getSimpleExtentNpoints() ;
    }
  }
