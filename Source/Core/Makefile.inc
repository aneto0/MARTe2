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
OBJSX=
SPB    = BareMetal.x Scheduler.x FileSystem.x
PACKAGE=
MARTe2_MAKEDEFAULT_DIR?=../../MakeDefaults

ROOT_DIR=../..
include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -IBareMetal/L0Types
INCLUDES += -IBareMetal/L1Portability
INCLUDES += -IBareMetal/L2Objects
INCLUDES += -IBareMetal/L3Streams
INCLUDES += -IBareMetal/L4Configuration
INCLUDES += -IBareMetal/L4Logger
INCLUDES += -IBareMetal/L4Messages
INCLUDES += -IBareMetal/L5GAMs
INCLUDES += -IBareMetal/L6App
INCLUDES += -IFileSystem/L1Portability
INCLUDES += -IFileSystem/L3Streams
INCLUDES += -IFileSystem/L4LoggerService
INCLUDES += -IFileSystem/L6App
INCLUDES += -IScheduler/L1Portability
INCLUDES += -IScheduler/L3Services
INCLUDES += -IScheduler/L4LoggerService
INCLUDES += -IScheduler/L4Messages
INCLUDES += -IScheduler/L4StateMachine
INCLUDES += -IScheduler/L5GAMs

all: $(OBJS) $(SUBPROJ) \
		$(BUILD_DIR)/MARTe2$(DLLEXT) \
		$(BUILD_DIR)/MARTe2$(LIBEXT)
	echo  $(OBJS)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)
