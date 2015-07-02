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

OBJSX=  EndianityTest.x \
        ThreadsTest.x \
        SleepTest.x \
        ProcessorTest.x \
        AtomicTest.x \
<<<<<<< HEAD
        ProcessorTypeTest.x \
	EventSemTest.x \
	MutexTest.x \
	BasicConsoleTest.x \
	HighResolutionTimerTest.x \
	MemoryTest.x \
	MemoryIntegrationTest.x \
	FastPollingMutexTest.x \
	StringHelperTest.x
=======
		ProcessorTypeTest.x \
		EventSemTest.x \
		MutexTest.x \
		BasicConsoleTest.x \
		HighResolutionTimerTest.x \
		MemoryTest.x \
		MemoryIntegrationTest.x \
		FastPollingMutexIntegrationTest.x \
		FastPollingMutexTest.x \
		StringHelperTest.x 
>>>>>>> 02fb75a8e699553ae2aefbdedfbb2c3e801e1593

PACKAGE=Core
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I../../../Source/Core/L0Portability
all: $(OBJS) \
                $(BUILD_DIR)/L0PortabilityTest$(LIBEXT) 
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

