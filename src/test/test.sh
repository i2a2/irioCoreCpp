#!/bin/bash

failed_tests=()

if [ $# -eq 0 ]; then
    set -- test_bfp test_ut_irioCoreCpp test_ut_irioCore
fi

case "$1" in
--help|-h)
    echo "Usage: $0 [test1] [test2] ..."
    echo "Available tests:"
    echo " - test_bfp"
    echo " - test_ut_irioCoreCpp"
    echo " - test_ut_irioCore"
    echo " - test_irioCoreCpp"
    echo " - test_irioCore"
    echo ""
    echo "If no test specified, only the test that do not require specific hardware will be run:"
    echo " - test_bfp"
    echo " - test_ut_irioCoreCpp"
    echo " - test_ut_irioCore"
    exit 0
    ;;
esac

for executable in "$@"; do
    case $executable in
        test_bfp)
            cd ./c++/bfp
            if ! ./test_bfp; then
                echo "test_bfp failed"
                failed_tests+=("test_bfp")
            fi
            cd ../..
            ;;
        test_ut_irioCoreCpp)
            cd ./c++/unittests/irioCoreCpp
            if ! ./test_ut_irioCoreCpp; then
                echo "test_ut_irioCoreCpp failed"
                failed_tests+=("test_ut_irioCoreCpp")
            fi
            cd ../../..
            ;;
        test_ut_irioCore)
            cd ./c++/unittests/irioCore
            if ! ./test_ut_irioCore; then
                echo "test_ut_irioCore failed"
                failed_tests+=("test_ut_irioCore")
            fi
            cd ../../..
            ;;
        test_irioCoreCpp)
            cd ./c++/irioCoreCpp/
            if ! ./test_irioCoreCpp; then
                echo "test_irioCoreCpp failed"
                failed_tests+=("test_irioCoreCpp")
            fi
            cd ../..
            ;;
        test_irioCore)
            cd ./c++/irioCore
            if ! ./test_irioCore; then
                echo "test_irioCore failed"
                failed_tests+=("test_irioCore")
            fi
            cd ../..
            ;;
        *)
            echo "Invalid executable name: $executable"
            ;;
    esac
done

if [ ${#failed_tests[@]} -gt 0 ]; then
    echo "The following tests failed:"
    for executable in "${failed_tests[@]}"; do
        echo -e "\t$executable"
    done
    exit -1
fi
