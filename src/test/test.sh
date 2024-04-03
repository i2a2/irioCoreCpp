#!/bin/bash

cd ./c++/bfp
./test_bfp

cd ../unittests/irioCoreCpp
./test_ut_irioCoreCpp

cd ../irioCore
./test_ut_irioCore

cd ../../irioCoreCpp/
./test_irioCoreCpp

cd ../irioCore
./test_irioCore