#!/bin/bash

failed_tests=()

cd ./c++/bfp
if ! ./test_bfp; then
    echo "test_bfp failed"
    failed_tests+=("test_bfp")
fi

cd ../unittests/irioCoreCpp
if ! ./test_ut_irioCoreCpp; then
    echo "test_ut_irioCoreCpp failed"
    failed_tests+=("test_ut_irioCoreCpp")
fi

cd ../irioCore
if ! ./test_ut_irioCore; then
    echo "test_ut_irioCore failed"
    failed_tests+=("test_ut_irioCore")
fi

cd ../../irioCoreCpp/
# if ! ./test_irioCoreCpp; then
#     echo "test_irioCoreCpp failed"
#     failed_tests+=("test_irioCoreCpp")
# fi

cd ../irioCore
# if ! ./test_irioCore; then
#     echo "test_irioCore failed"
#     failed_tests+=("test_irioCore")
# fi

if [ ${#failed_tests[@]} -gt 0 ]; then
    echo "The following tests failed:"
    for executable in "${failed_tests[@]}"; do
        echo "$executable"
    done
fi
