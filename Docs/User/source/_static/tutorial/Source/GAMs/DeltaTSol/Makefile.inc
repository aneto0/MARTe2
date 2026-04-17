#############################################################
#                                                            
# Copyright 2026 EFDA | European Joint Undertaking for ITER  
# and the Development of Fusion Energy ('Fusion for Energy') 
#                                                            
# Licensed under the EUPL, Version 1.1 or - as soon they     
# will be approved by the European Commission - subsequent   
# versions of the EUPL (the 'Licence');                      
# You may not use this work except in compliance with the    
# Licence.                                                   
# You may obtain a copy of the Licence at:                   
#                                                            
# https://eupl.eu/                                           
#                                                            
# Unless required by applicable law or agreed to in          
# writing, software distributed under the Licence is         
# distributed on an 'AS IS' basis,                           
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either       
# express or implied.                                        
# See the Licence for the specific language governing        
# permissions and limitations under the Licence.             
#                                                            
# Date:   17/04/2026                                            
# Author: cabrian                                          
#                                                            
#############################################################

SPB =  

OBJSX = DeltaTSol.x

PACKAGE = GAMs

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

ROOT_DIR = ../../../

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I.
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4HttpService
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Logger
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L6App
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L4HttpService
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L4LoggerService
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L6App
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4LoggerService
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4StateMachine
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs

all: $(OBJS) $(SUBPROJ) check-env \
	$(BUILD_DIR)/DeltaTSol$(LIBEXT) \
	$(BUILD_DIR)/DeltaTSol$(DLLEXT)
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

check-env:
ifndef MARTe2_DIR
    $(error MARTe2_DIR is undefined)
endif
ifndef MARTe2_Components_DIR
    $(error MARTe2_Components_DIR is undefined)
endif

