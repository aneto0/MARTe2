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

PACKAGE=Core/BareMetal/L2Objects
ROOT_DIR=../../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults


OBJSX=  	VariableDescriptorLib.x VD_Variable.x Dimension.x \
                DataDimension.x ArrayDimension.x PointerToArrayDimension.x \
                VectorDimension.x MatrixDimension.x ZTADimension.x \
                VariableCloner.x

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES+=-I.. -I../../L1Portability -I../../L0Types

#LIBRARIES_STATIC=$(BUILD_DIR)/../../L0Types/L0TypesB$(LIBEXT)
#LIBRARIES_STATIC+=$(BUILD_DIR)/../../L1Portability/L1PortabilityB$(LIBEXT)

all: $(OBJS) $(SUBPROJ)   \
		$(BUILD_DIR)/L2VDLib$(LIBEXT) 
	echo  $(OBJS)


include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

