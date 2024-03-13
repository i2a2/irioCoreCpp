#!/bin/bash

cd ./c++/bfp
./test_bfp


cd ../unittests/irio_v2_cpp
./test_ut_iriov2cpp

cd ../irio_v2_cpp_compatibility
./test_ut_iriov2cpp_compatibility

cd ../../irio_v2_cpp/
./test_iriov2cpp

