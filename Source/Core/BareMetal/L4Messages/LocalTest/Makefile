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

PACKAGE=Core/BareMetal/L4Messages
ROOT_DIR=../../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

OPTIMIZE=TestPseudoCode.x

include $(MAKEDEFAULTDIR)/MakeStdLibDefs

INCLUDES += -I../../L4Configuration
INCLUDES += -I../../L2Objects
INCLUDES += -I../../L3Streams
INCLUDES += -I../../L1Portability 
INCLUDES += -I../../L0Types 
INCLUDES += -I../VariableDescriptorLib 
INCLUDES += -I..

LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L0Types/*$(OBJEXT) 
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L1Portability/*$(OBJEXT) 
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/Scheduler/L1Portability/*$(OBJEXT) 
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/FileSystem/L1Portability/*$(OBJEXT) 
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L2Objects/*$(OBJEXT)
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L3Streams/*$(OBJEXT)
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L4Configuration/*$(OBJEXT)
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Core/BareMetal/L4Messages/*$(OBJEXT)
LIBRARIES += $(ROOT_DIR)/Build/$(TARGET)/Tools/TestSupport/*$(OBJEXT) 

OPTIM=-O4  -march=amdfam10 
#-mdump-tune-features
#-ftree-vectorize -fopt-info-vec-all

BUILD_PRODUCT = $(BUILD_DIR)/TestPseudoCode$(EXEEXT) 

include $(MAKEDEFAULTDIR)/MakeStdLibRules

