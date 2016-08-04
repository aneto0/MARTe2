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
# $Id: Makefile.inc 3 2012-01-15 16:26:07Z aneto $
#
#############################################################


OBJSX= BrokerITest.x \
       DataSourceITest.x\
       GAMDataSourceTest.x\
       GAMGroupTest.x\
       GAMSchedulerITest.x \
       GAMTest.x\
       GAMTestHelper.x\
       MemoryMapBrokerTest.x\
       MemoryMapInputBrokerTest.x\
       MemoryMapOutputBrokerTest.x\
       MemoryMapStatefulBrokerTest.x\
       MemoryMapStatefulInputBrokerTest.x\
       MemoryMapStatefulOutputBrokerTest.x\
       MemoryMapSynchronisedInputBrokerTest.x\
       RealTimeApplicationConfigurationBuilderTest.x\
       RealTimeStateTest.x\
       RealTimeThreadTest.x

#BrokerContainerTest.x \
		DataSourceContainerTest.x \
		DataSourceSignalRecordTest.x \
		DataSourceSignalTest.x \
		DataSourceSignalITest.x\
		DataSourceTest.x \
		GAMGenericSignalTest.x \
		GAMGroupTest.x \
		GAMSampledSignalTest.x \
		GAMSchedulerITest.x \
		GAMSchedulerRecordTest.x \
		GAMSignalsContainerTest.x \
		GAMSignalITest.x \
		GAMTest.x \
		MemoryMapDataSourceBrokerTest.x \
		MemoryMapInputReaderTest.x \
		MemoryMapOutputWriterTest.x \
		RealTimeApplicationTest.x \
		RealTimeStateTest.x \
		RealTimeThreadTest.x
		
#BasicGAMTest.x \
 		BasicGAMSchedulerTest.x \
		BasicRealTimeDataSourceDefTest.x \
		BasicRealTimeDataSourceInputReaderTest.x \
		BasicRealTimeDataSourceOutputWriterTest.x \
		GAMGroupTest.x \
		GAMITest.x \
		GAMSchedulerITest.x \
		GAMSchedulerRecordTest.x \
		RealTimeApplicationTest.x \
		RealTimeDataDefContainerTest.x \
		RealTimeDataSourceTest.x \
		RealTimeDataSourceBrokerTest.x \
		RealTimeDataSourceContainerTest.x \
		RealTimeDataSourceDefRecordTest.x \
		RealTimeGenericDataDefTest.x \
		RealTimeSampledDataDefTest.x \
		RealTimeStateTest.x \
		RealTimeThreadTest.x
			
PACKAGE=Core/BareMetal
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I../../../../Source/Core/BareMetal/L0Types
INCLUDES += -I../../../../Source/Core/BareMetal/L1Portability
INCLUDES += -I../../../../Source/Core/BareMetal/L2Objects
INCLUDES += -I../../../../Source/Core/BareMetal/L3Streams
INCLUDES += -I../../../../Source/Core/BareMetal/L4Configuration
INCLUDES += -I../../../../Source/Core/BareMetal/L5GAMs
INCLUDES += -I../../../../Source/Core/FileSystem/L1Portability
INCLUDES += -I../../../../Source/Core/Scheduler/L1Portability


all: $(OBJS) \
    $(BUILD_DIR)/L5GAMsTest$(LIBEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

