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

OBJSX=          ClassRegistryDatabase.x \
		VariableDescriptor.x\
                AnyType.x \
		Reference.x \
		ReferenceContainer.x \
		ReferenceContainerNode.x \
                StructuredDataI.x \
		Object.x \
		IntrospectionTools.x \
		ClassMethodInterfaceMapper.x\
		ClassMethodCaller.x\
		ClassRegistryItem.x \
		ObjectRegistryDatabase.x \
		ReferenceContainerFilter.x \
		ReferenceContainerFilterReferences.x \
		ReferenceContainerFilterObjectName.x \
		test.x


include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES+=-I../L1Portability -I../L0Types

LIBRARIES_STATIC=$(BUILD_DIR)/../L0Types/L0TypesB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/../L1Portability/L1PortabilityB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/../L0Types/BalancedTreeNode.obj


all: $(OBJS) $(SUBPROJ)   \
      $(BUILD_DIR)/test$(EXEEXT) \
		$(BUILD_DIR)/L2ObjectsB$(LIBEXT) \
		$(BUILD_DIR)/L2ObjectsB.def
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

