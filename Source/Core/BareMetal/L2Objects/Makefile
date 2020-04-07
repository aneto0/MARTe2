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
PACKAGE=Core/BareMetal
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

# to allow horizontal naviagtion in the tree. Generates subprojects associated to the HTEARS
SPBLT= $(patsubst %,%.x,$(foreach toCheck,$(foreach hPath, $(HTEARS), $(subst BareMetal,$(hPath),$(CURDIR))),$(wildcard $(toCheck))))

include $(MAKEDEFAULTDIR)/MakeStdLibDefs

INCLUDES+=-I. -I../L1Portability -I../L0Types -IVariableDescriptorLib

LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L0Types/*$(OBJEXT) 
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L1Portability/*$(OBJEXT) 
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/Scheduler/L1Portability/*$(OBJEXT) 
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/FileSystem/L1Portability/*$(OBJEXT) 


include $(MAKEDEFAULTDIR)/MakeStdLibRules

