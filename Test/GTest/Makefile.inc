#############################################################
#
# Copyright 2015 F4E | European Joint Undertaking for ITER 
#  and the Development of Fusion Energy ('Fusion for Energy')
# 
# Licensed under the EUPL, Version 1.1 or - as soon they 
# will be approved by the European Commission - subsequent  
# versions of the EUPL (the "Licence"); 
# You may not use this work except in compliance with the 
# Licence. 
# You may obtain a copy of the Licence at: 
#  
# http://ec.europa.eu/idabc/eupl
#
# Unless required by applicable law or agreed to in 
# writing, software distributed under the Licence is 
# distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. 
# See the Licence for the specific language governing 
# permissions and limitations under the Licence. 
#
#############################################################

OBJSX=
SPB = BareMetal.x Scheduler.x FileSystem.x

PACKAGE=
ROOT_DIR=../..
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

ifeq ($(MARTe2_TEST_ENVIRONMENT), GTest)
INCLUDES += -I$(ROOT_DIR)/Lib/gtest-1.7.0/include
OBJSX += MARTe2UTest.x
LIBRARIES += ../../Lib/gtest-1.7.0/libgtest.a ../../Lib/gtest-1.7.0/libgtest_main.a 
else
LIBRARIES += ../../Build/$(TARGET)/$(MARTe2_TEST_ENVIRONMENT)/MARTe2UTest$(OBJEXT)
LIBRARIES += ../../Build/$(TARGET)/$(MARTe2_TEST_ENVIRONMENT)/MARTe2UTestImpl$(OBJEXT)
endif

INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L6App
INCLUDES += -I$(ROOT_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES += -I$(ROOT_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(ROOT_DIR)/Test/

all: $(OBJS) $(SUBPROJ)   \
        $(BUILD_DIR)/MainGTest$(EXEEXT)
	echo  $(OBJS)


include depends.$(TARGET)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

