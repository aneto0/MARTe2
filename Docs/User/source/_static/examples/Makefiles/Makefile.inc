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

#Named of the unit files to be compiled
OBJSX=ExFile2.x

#Create a folder named Examples in the Build directory
PACKAGE=Examples

#Location of the Build directory
ROOT_DIR=../../../../../..

#Location of the MakeDefaults directory.
#Note that the ROOT_DIR environment variable
#must have been exported before
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES += -I$(ROOT_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(ROOT_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES += -I$(ROOT_DIR)/Source/Core/FileSystem/L3Streams
INCLUDES += -I$(ROOT_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(ROOT_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(ROOT_DIR)/Source/Core/Scheduler/L4Messages
INCLUDES += -I$(ROOT_DIR)/Source/Core/Scheduler/L4StateMachine

#Generate an executable named ExFile1$(EXEEXT).
#Note that the file ExFile1.cpp has to exist and must
#not be declared in the OBJSX list.
all: $(OBJS) $(SUBPROJ)   \
        $(BUILD_DIR)/ExFile1$(EXEEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

