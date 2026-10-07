#!/bin/bash
OUTPUT_DIR=Build

#Clear previous results
rm -f $OUTPUT_DIR/MARTe2.coverage.*

#Build with coverage enabled
make -f Makefile.cov clean
make -f Makefile.cov

if [ $# -gt 0 ]; then
    for filter in $@; do
        echo "Testing: $filter"
        Test/GTest/cov/MainGTest.ex --gtest_filter=$filter
    done
else
    #Execute the tests
    Test/GTest/cov/MainGTest.ex --gtest_filter=BareMetal*
    Test/GTest/cov/MainGTest.ex --gtest_filter=FileSystem*
    Test/GTest/cov/MainGTest.ex --gtest_filter=Scheduler*
fi

#Generate the html
mkdir -p $OUTPUT_DIR/cov_html
gcovr --gcov-ignore-errors=source_not_found --html-details $OUTPUT_DIR/cov_html/coverage.html Source/

#Generate the text output
gcovr --gcov-ignore-errors=source_not_found --txt $OUTPUT_DIR/coverage.txt Source/

#make -f Makefile.cov clean_gen
