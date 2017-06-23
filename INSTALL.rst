Dependencies
============


HDF5
----

Use CMake build from https://www.hdfgroup.org/HDF5/release/cmakebuild.html


Windows
~~~~~~~

Build with: ::

  ctest -S HDF5config.cmake,BUILD_GENERATOR=VS201364,INSTALLDIR=C:\usr\local ^
        -C Release -V -O hdf518.log

Then run installer and set `HDF5_DIR` environment variable to location of
installed CMake files. e.g: ::

  set HDF5_DIR=c:/Program Files/HDF_Group/HDF5/1.8.16/cmake


macOS and Linux
~~~~~~~~~~~~~~~

Build with: ::

  curl https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-1.10.1/src/CMake-hdf5-1.10.1.tar.gz | tar xz
  cd CMake-hdf5-1.10.1
  ctest -S HDF5config.cmake,BUILD_GENERATOR=Unix,INSTALLDIR=/usr/local -C Release -V -O build.log

  cd build
  # Linux
  sudo ./HDF5-1.10.1-Linux.sh --prefix=/usr/local
  # macOS
  sudo ./HDF5-1.10.1-Darwin.sh --prefix=/usr/local

and after installation: ::

  export HDF5_DIR=/usr/local/HDF_Group/HDF5/1.10.1/share/cmake

The `szip` and `zlib` static libraries and CMake files must be manually copied
if HDF5 is then installed by running `make install` from the build directory;
they though are in the created `.tar.gz` archives.
