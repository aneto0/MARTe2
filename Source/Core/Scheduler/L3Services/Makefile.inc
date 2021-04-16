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

OBJSX=	EmbeddedServiceI.x \
    EmbeddedServiceMethodBinderI.x \
    EmbeddedThreadI.x \
    EmbeddedThread.x \
    ExecutionInfo.x \
    MultiThreadService.x \
    MultiClientEmbeddedThread.x \
    MultiClientService.x \
    SingleThreadService.x \
    ThreadsInformationQuery.x
        
PACKAGE=Core/Scheduler
ROOT_DIR=../../../..
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults
include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I.
INCLUDES += -I../../BareMetal/L0Types
INCLUDES += -I../../BareMetal/L1Portability
INCLUDES += -I../../BareMetal/L2Objects
INCLUDES += -I../../BareMetal/L3Streams
INCLUDES += -I../../BareMetal/L4Configuration
INCLUDES += -I../../BareMetal/L4Messages
INCLUDES += -I../../BareMetal/L5GAMs
INCLUDES += -I../../Scheduler/L1Portability
all: $(OBJS)    \
	        $(BUILD_DIR)/L3ServicesS$(LIBEXT) \
	        $(BUILD_DIR)/L3ServicesS.def
	echo  $(OBJS)

include depends.$(TARGET)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

