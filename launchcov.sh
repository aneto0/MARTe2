#/bin/sh
make -f Makefile.linux
make -f Makefile.cov

./Test/GTest/cov/MainGTest.ex < Test/GTest/cov/input

cd ../../../lcov/bin/

./geninfo ../../EFDA-MARTE-old/branches/MARTe2
./lcov --capture --directory ../../EFDA-MARTE-old/branches/MARTe2 --output-file coverage.info
./genhtml coverage.info --output-directory /tmp/html

cd -

