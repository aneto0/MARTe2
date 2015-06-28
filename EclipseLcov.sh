#!/bin/bash
LCOV_DIR=~/bin/lcov/bin/
OUTPUT_DIR=Build

#Build with coverage enabled
make -f Makefile.cov clean
make -f Makefile.cov

#Run baseline coverage
$LCOV_DIR/lcov --capture --initial --directory . --no-external --output-file $OUTPUT_DIR/MARTe2.coverage.info.initial

#Execute the tests
Test/GTest/cov/MainGTest.ex

#Create test coverage data file
$LCOV_DIR/lcov --capture --directory . --no-external --output-file $OUTPUT_DIR/MARTe2.coverage.info.tests

#Combine baseline and test coverage data
$LCOV_DIR/lcov --add-tracefile $OUTPUT_DIR/MARTe2.coverage.info.initial --add-tracefile $OUTPUT_DIR/MARTe2.coverage.info.tests --output-file $OUTPUT_DIR/MARTe2.coverage.info.1

#Remove false positives
$LCOV_DIR/lcov --remove $OUTPUT_DIR/MARTe2.coverage.info.1 "/Test*" --output-file $OUTPUT_DIR/MARTe2.coverage.info.2
$LCOV_DIR/lcov --remove $OUTPUT_DIR/MARTe2.coverage.info.2 "*gtest*" --output-file $OUTPUT_DIR/MARTe2.coverage.info

#Generate the html
$LCOV_DIR/genhtml $OUTPUT_DIR/MARTe2.coverage.info --output-directory $OUTPUT_DIR/cov_html
