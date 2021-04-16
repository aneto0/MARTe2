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

SPB    = L0Types.x L1Portability.x L2Objects.x L3Streams.x L4Configuration.x L4HttpService.x L4Logger.x L4Messages.x L5GAMs.x L6App.x

PACKAGE=GTest
ROOT_DIR=../../..
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)


LIBRARIES_STATIC=$(BUILD_DIR)/L0Types/L0TypesBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L1Portability/L1PortabilityBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L2Objects/L2ObjectsBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L3Streams/L3StreamsBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L4Configuration/L4ConfigurationBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L4HttpService/L4HttpServiceBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L4Logger/L4LoggerBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L4Messages/L4MessagesBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L5GAMs/L5GAMsBGTest$(LIBEXT)
LIBRARIES_STATIC+= $(BUILD_DIR)/L6App/L6AppBGTest$(LIBEXT)

all: $(OBJS)  $(SUBPROJ)  \
        $(BUILD_DIR)/MARTeBGTest$(LIBEXT) 
	echo  $(OBJS)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)


