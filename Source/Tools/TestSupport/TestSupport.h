/**
 * @file TestSupport.h
 * @brief Header file for class AnyType
 * @date 2 Jun 2019
 * @author Filippo Sartori
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef SOURCE_TEST_TESTSUPPORT_H_
#define SOURCE_TEST_TESTSUPPORT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
class TestInstance;
extern TestInstance *TestRoot;
extern bool TestSupportShowResults;


class TestInstance{
public:
	const char *testName;
	TestInstance();
	virtual ~TestInstance();
	virtual void Execute(bool &result)=0;
	TestInstance *nextTest;
};

void RunAllTests();

#define TEST(test_group_name,test_name)\
	class test_group_name ##  test_name ## TestInstance: public TestInstance{  \
	public:\
		test_group_name ##  test_name ## TestInstance(const char* testName);\
		virtual void Execute(bool &result);\
	}  test_group_name ##  test_name ## testInstance(#test_group_name "::" #test_name);\
	test_group_name ##  test_name ## TestInstance::test_group_name ##  test_name ## TestInstance(const char* testName){\
		this->testName = testName; \
		printf("added test %s\n",testName); \
	}\
	void test_group_name ##  test_name ## TestInstance::Execute(bool &result)



#define ASSERT(test_code,show,expect)\
			{ bool localRet = test_code;\
			if (show){\
				if (!expect){ \
					if (localRet){\
						printf("OK       :");\
					} else {\
						printf("**NO   :");\
					}\
				} else {\
					if (localRet){\
						printf("**OK=NO :");\
					} else {\
						printf("NO=OK  :");\
					}\
				}\
				printf ( #test_code "\n");\
			}\
			result = result & (localRet ^ expect); }

#define ASSERTI(test_code,show,expect)\
			{ int localRet = test_code;\
			if (show){\
				printf("%i ",localRet);\
				if (expect==0){ \
					if (localRet==0){\
						printf("OK     :");\
					} else {\
						printf("**NO   :");\
					}\
				} else {\
					if (localRet==expect){\
						printf("%i=OK   :",expect);\
					} else {\
						printf("**NO!=%i:",expect);\
					}\
				}\
				printf ( #test_code "\n");\
			}\
			result = result & (localRet != expect); }

#define ASSERT_TRUE(code_to_execute)  ASSERT((code_to_execute),TestSupportShowResults,false)
#define ASSERT_FALSE(code_to_execute) ASSERT((code_to_execute),TestSupportShowResults,true)
#define ASSERT_VALUE(code_to_execute,value) ASSERTI((code_to_execute),TestSupportShowResults,value)







#endif /* SOURCE_TEST_TESTSUPPORT_H_ */
