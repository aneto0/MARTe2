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
SPB    = L0Types.x L1Portability.x L2Objects.x L3Streams.x L4Configuration.x L4Logger.x L4Messages.x L4HttpService.x L5GAMs.x L6App.x
PACKAGE=Core
MARTe2_MAKEDEFAULT_DIR?=../../../MakeDefaults

ROOT_DIR=../../..
include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

LIBRARIES_STATIC=$(BUILD_DIR)/L0Types/L0TypesB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L1Portability/L1PortabilityB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L2Objects/L2ObjectsB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L3Streams/L3StreamsB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L4Configuration/L4ConfigurationB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L4Logger/L4LoggerB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L4Messages/L4MessagesB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L4HttpService/L4HttpServiceB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L5GAMs/L5GAMsB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L6App/L6AppB$(LIBEXT)

all: $(OBJS) $(SUBPROJ) \
		$(BUILD_DIR)/MARTeCoreB$(LIBEXT) \
		$(BUILD_DIR)/MARTeCoreB.def 
	echo  $(OBJS)



include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

