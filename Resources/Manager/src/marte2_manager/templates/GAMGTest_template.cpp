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

TEST(${class}, TestSetup) {
    ${src_class} test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(${class}, TestExecute) {
    ${src_class} test;
    ASSERT_TRUE(test.TestExecute());
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

