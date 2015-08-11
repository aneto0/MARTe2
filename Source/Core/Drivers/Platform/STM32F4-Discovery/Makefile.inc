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

PACKAGE=Core/Drivers
ROOT_DIR=../../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

OBJSX=ClockP.x \
	USBComunicationP.x \
	VCPDescription.x 
       
include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES+= -I../../../L0Portability
INCLUDES+= -I../../../L1Objects
INCLUDES+= -I../..
INCLUDES+= -I.
all: $(OBJS)    \
                $(BUILD_DIR)/DriversP$(LIBEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

