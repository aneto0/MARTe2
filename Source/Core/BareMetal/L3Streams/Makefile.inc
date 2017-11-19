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

OBJSX=	BufferedStreamI.x \
		BufferedStreamIOBuffer.x \
		CharBuffer.x \
		DoubleBufferedStream.x \
		IOBuffer.x \
		IOBufferFloatPrint.x \
		IOBufferIntegerPrint.x \
		IOBufferStringPrint.x \
		IOBufferStreamPrint.x \
		IOBufferStructuredPrint.x \
		ToStringConversionFactory.x\
		IOBufferAnyTypeInfo.x \
		IOBufferAnyTypePrint.x \
		IOBufferStaticFunctions.x \
		SingleBufferedStream.x \
		StreamMemoryReference.x \
		StreamString.x\
		StreamStringIOBuffer.x
        
PACKAGE=Core/BareMetal
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults
include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I.
INCLUDES += -I../L0Types
INCLUDES += -I../L1Portability
INCLUDES += -I../L2Objects

LIBRARIES_STATIC=$(BUILD_DIR)/../L0Types/L0TypesB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/../L1Portability/L1PortabilityB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/../L2Objects/L2ObjectsB$(LIBEXT)


all: $(OBJS)    \
	        $(BUILD_DIR)/L3StreamsB$(LIBEXT) \
                $(BUILD_DIR)/test2$(EXEEXT) \
	        $(BUILD_DIR)/L3StreamsB.def 
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

