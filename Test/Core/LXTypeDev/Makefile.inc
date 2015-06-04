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

OBJSX= IntegerToStreamTest.x \
	FloatToStreamTest.x \
	BitSetToIntegerTest.x \
	DoubleIntegerTest.x \
	BufferedStreamTest.x \
	StreamStringTest.x \
	StreamMemoryReferenceTest.x \
	ErrorManagementTest.x \
	StreamableTest.x
		
PACKAGE=Core
ROOT_DIR=../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I../../../Source/Core/L0Portability
INCLUDES += -I../../../Source/Core/L1CLib
INCLUDES += -I../../../Source/Core/LXTypeDev
INCLUDES += -I../L0Portability

all: $(OBJS) \
                $(BUILD_DIR)/LXTypeDevTest$(LIBEXT)                 
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

