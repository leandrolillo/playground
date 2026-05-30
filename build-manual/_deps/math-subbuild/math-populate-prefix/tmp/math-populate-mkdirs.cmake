# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-src")
  file(MAKE_DIRECTORY "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-src")
endif()
file(MAKE_DIRECTORY
  "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-build"
  "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-subbuild/math-populate-prefix"
  "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-subbuild/math-populate-prefix/tmp"
  "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-subbuild/math-populate-prefix/src/math-populate-stamp"
  "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-subbuild/math-populate-prefix/src"
  "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-subbuild/math-populate-prefix/src/math-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-subbuild/math-populate-prefix/src/math-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/tmp/workspace/leandrolillo/playground/build-manual/_deps/math-subbuild/math-populate-prefix/src/math-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
