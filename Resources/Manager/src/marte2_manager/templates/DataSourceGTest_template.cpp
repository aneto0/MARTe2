${header_text}

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "${src_class}.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace ${namespace} {

TEST(${class}, TestInitialise) {
    ${src_class} test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(${class}, TestSetConfiguredDatabase) {
    ${src_class} test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(${class}, TestSynchronise) {
    ${src_class} test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(${class}, TestAllocateMemory) {
    ${src_class} test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(${class}, TestGetSignalMemoryBuffer) {
    ${src_class} test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(${class}, TestGetBrokerName) {
    ${src_class} test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(${class}, TestPrepareNextState) {
    ${src_class} test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

