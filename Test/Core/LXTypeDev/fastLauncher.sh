#/bin/sh

make -f Makefile.linux

cd ../../GTest/

make -f Makefile.linux clean
make -f Makefile.linux

cd -
cd ../../../Build/linux/GTest

./MainGTest.ex --gtest_filter=* < input
