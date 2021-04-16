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

SPB = L1Portability.x L3Streams.x L4HttpService.x L4LoggerService.x L6App.x

PACKAGE=GTest
ROOT_DIR=../../..
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

LIBRARIES_STATIC = $(BUILD_DIR)/L1Portability/L1PortabilityFGTest$(LIBEXT)
LIBRARIES_STATIC += $(BUILD_DIR)/L3Streams/L3StreamsFGTest$(LIBEXT)
LIBRARIES_STATIC += $(BUILD_DIR)/L4HttpService/L4HttpServiceFGTest$(LIBEXT)
LIBRARIES_STATIC += $(BUILD_DIR)/L4LoggerService/L4LoggerServiceFGTest$(LIBEXT)
LIBRARIES_STATIC += $(BUILD_DIR)/L6App/L6AppFGTest$(LIBEXT)

all: $(OBJS) $(SUBPROJ)   \
        $(BUILD_DIR)/MARTeFGTest$(LIBEXT) 
	echo  $(OBJS)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)
