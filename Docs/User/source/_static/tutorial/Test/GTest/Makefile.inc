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
# Date:   16/04/2026                                            
# Author: cabrian                                          
#                                                            
#############################################################

SPB =  

OBJSX =  

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

ROOT_DIR = ../..

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

LIBRARIES   += -L$(MARTe2_DIR)/Build/$(TARGET)/Core/ -lMARTe2
LIBRARIES   += $(MARTe2_DIR)/Lib/gtest-1.7.0/libgtest.a $(MARTe2_DIR)/Lib/gtest-1.7.0/libgtest_main.a

INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Lib/gtest-1.7.0/include

LIBRARIES_STATIC+=$(shell find $(ROOT_DIR)/Build/$(TARGET) -name "*.a")

all: $(OBJS) check-env\
	$(BUILD_DIR)/MainGTest$(EXEEXT)
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

check-env:
ifndef MARTe2_DIR
    $(error MARTe2_DIR is undefined)
endif
ifndef MARTe2_Components_DIR
    $(error MARTe2_Components_DIR is undefined)
endif

