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

PACKAGE=Core/Scheduler/L1Portability
ROOT_DIR=$(MARTe2_DIR)
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults


OBJSX=  EventSem.x \
        MutexSem.x \
        Processor.x \
        Threads.x \
        ThreadInformation_Gen.x 

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

INCLUDES+=-I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/FileSystem/L3Streams
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/FileSystem/L6App
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L4Logger
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L6App
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability

all: $(OBJS)    $(CPPS) \
                $(BUILD_DIR)/L1Portability$(LIBEXT)
	echo  $(OBJS) $(CPPS)

include depends.$(TARGET)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

