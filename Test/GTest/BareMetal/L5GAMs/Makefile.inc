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


OBJSX=  BrokerIGTest.x\
        DataSourceIGTest.x\
        ExecutableIGTest.x\
        GAMBareSchedulerGTest.x\
        GAMDataSourceGTest.x\
        GAMGroupGTest.x\
        GAMGTest.x\
        GAMSchedulerIGTest.x\
        MemoryDataSourceIGTest.x\
        MemoryMapBrokerGTest.x\
        MemoryMapInputBrokerGTest.x\
        MemoryMapInterpolatedInputBrokerGTest.x\
        MemoryMapMultiBufferBrokerGTest.x\
        MemoryMapMultiBufferInputBrokerGTest.x\
        MemoryMapMultiBufferOutputBrokerGTest.x\
        MemoryMapOutputBrokerGTest.x\
        MemoryMapSynchronisedInputBrokerGTest.x\
        MemoryMapSynchronisedMultiBufferInputBrokerGTest.x\
        MemoryMapSynchronisedMultiBufferOutputBrokerGTest.x\
        MemoryMapSynchronisedOutputBrokerGTest.x\
        RealTimeApplicationGTest.x\
        RealTimeApplicationConfigurationBuilderGTest.x\
        RealTimeStateGTest.x\
        RealTimeThreadGTest.x\
        TimingDataSourceGTest.x

PACKAGE=GTest/BareMetal
ROOT_DIR=../../../..
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)
include $(ROOT_DIR)/Test/$(MARTe2_TEST_ENVIRONMENT)/Makefile.utest

INCLUDES += -I../../..
INCLUDES += -I../../../Core/BareMetal/L0Types
INCLUDES += -I../../../Core/BareMetal/L1Portability
INCLUDES += -I../../../Core/BareMetal/L2Objects
INCLUDES += -I../../../Core/BareMetal/L3Streams
INCLUDES += -I../../../Core/BareMetal/L4Configuration
INCLUDES += -I../../../Core/BareMetal/L4Messages
INCLUDES += -I../../../Core/BareMetal/L5GAMs
INCLUDES += -I../../../Core/Scheduler/L1Portability
INCLUDES += -I../../../Core/FileSystem/L1Portability
INCLUDES += -I../../../../Source/Core/BareMetal/L0Types
INCLUDES += -I../../../../Source/Core/BareMetal/L1Portability
INCLUDES += -I../../../../Source/Core/BareMetal/L2Objects
INCLUDES += -I../../../../Source/Core/BareMetal/L3Streams
INCLUDES += -I../../../../Source/Core/BareMetal/L4Configuration
INCLUDES += -I../../../../Source/Core/BareMetal/L4Messages
INCLUDES += -I../../../../Source/Core/BareMetal/L5GAMs
INCLUDES += -I../../../../Source/Core/Scheduler/L1Portability
INCLUDES += -I../../../../Source/Core/FileSystem/L1Portability



all: $(OBJS)    \
        $(BUILD_DIR)/L5GAMsBGTest$(LIBEXT) 
	echo  $(OBJS)

include depends.$(TARGET)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

