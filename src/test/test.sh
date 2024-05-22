#!/bin/bash

output_files=()

if [ $# -eq 0 ]; then
    set -- test-suites/BFP.xml test-suites/irioCoreUnit.xml test-suites/irioCoreCppUnit.xml
fi

case "$1" in
--help|-h)
    echo "Usage: $0 [test1] [test2] ..."
    echo "Available tests:"
    echo " - test-suites/irioCoreAll.xml"
    echo " - test-suites/BFP.xml"
    echo " - test-suites/irioCoreUnit.xml"
    echo " - test-suites/irioCoreCppUnit.xml"
    echo " - test-suites/irioCoreFunctional.xml"
    echo " - test-suites/irioCoreCppFunctional.xml"
    echo ""
    echo "If no test specified, only the test that do not require specific hardware will be run:"
    echo " - test-suites/BFP.xml"
    echo " - test-suites/irioCoreUnit.xml"
    echo " - test-suites/irioCoreCppUnit.xml"
    exit 0
    ;;
esac

mkdir -p test-results
for executable in "$@"; do
    ./automate_GT.py -i $executable -o test-results/$(basename $executable .xml).xml -S
    output_files+=("test-results/$(basename $executable .xml).xml")
done

for f in $output_files; do
    if grep -q FAIL $f; then
        exit 1
    fi
done