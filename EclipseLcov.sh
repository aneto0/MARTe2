#!/bin/bash
OUTPUT_DIR=Build

#Build with coverage enabled
make -f Makefile.cov clean
make -f Makefile.cov

#Run baseline coverage
lcov --capture --initial --directory . --no-external --output-file $OUTPUT_DIR/MARTe2.coverage.info.initial

#Execute the tests
Test/GTest/cov/MainGTest.ex --gtest_filter=BareMetal*
Test/GTest/cov/MainGTest.ex --gtest_filter=FileSystem*
Test/GTest/cov/MainGTest.ex --gtest_filter=Scheduler*:-Scheduler*L5GAMs*
Test/GTest/cov/MainGTest.ex --gtest_filter=Scheduler*L5GAMs*:-Scheduler*CircularBufferThreadInputDataSourceGTest*
Test/GTest/cov/MainGTest.ex --gtest_filter=Scheduler*CircularBufferThreadInputDataSourceGTest*

#Create test coverage data file
lcov --capture --directory . --no-external --output-file $OUTPUT_DIR/MARTe2.coverage.info.tests

#Combine baseline and test coverage data
lcov --add-tracefile $OUTPUT_DIR/MARTe2.coverage.info.initial --add-tracefile $OUTPUT_DIR/MARTe2.coverage.info.tests --output-file $OUTPUT_DIR/MARTe2.coverage.info.1

#Remove false positives
lcov --remove $OUTPUT_DIR/MARTe2.coverage.info.1 "/Test*" --output-file $OUTPUT_DIR/MARTe2.coverage.info.2
lcov --remove $OUTPUT_DIR/MARTe2.coverage.info.2 "*gtest*" --output-file $OUTPUT_DIR/MARTe2.coverage.info

#Generate the html
genhtml $OUTPUT_DIR/MARTe2.coverage.info --output-directory $OUTPUT_DIR/cov_html

make -f Makefile.cov clean_gen
