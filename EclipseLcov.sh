#!/bin/bash
OUTPUT_DIR=Build

#Clear previous results
rm -f $OUTPUT_DIR/MARTe2.coverage.*

#Build with coverage enabled
make -f Makefile.cov clean
make -f Makefile.cov

#Run baseline coverage
lcov --capture --initial --directory . --no-external --output-file $OUTPUT_DIR/MARTe2.coverage.info.initial

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

#Create test coverage data file
lcov --capture --directory . --no-external --output-file $OUTPUT_DIR/MARTe2.coverage.info.tests

#Combine baseline and test coverage data
lcov --add-tracefile $OUTPUT_DIR/MARTe2.coverage.info.initial --add-tracefile $OUTPUT_DIR/MARTe2.coverage.info.tests --output-file $OUTPUT_DIR/MARTe2.coverage.info.1

#Remove false positives
lcov --remove $OUTPUT_DIR/MARTe2.coverage.info.1 "/Test*" --output-file $OUTPUT_DIR/MARTe2.coverage.info.2
lcov --remove $OUTPUT_DIR/MARTe2.coverage.info.2 "*_Gen.cpp" --output-file $OUTPUT_DIR/MARTe2.coverage.info.3
lcov --remove $OUTPUT_DIR/MARTe2.coverage.info.3 "*gtest*" --output-file $OUTPUT_DIR/MARTe2.coverage.info

#Generate the html
genhtml $OUTPUT_DIR/MARTe2.coverage.info --output-directory $OUTPUT_DIR/cov_html

#Generate the text output
lcov -l $OUTPUT_DIR/MARTe2.coverage.info > $OUTPUT_DIR/MARTe2.coverage.out

make -f Makefile.cov clean_gen
