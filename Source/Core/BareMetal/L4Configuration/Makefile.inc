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

OBJSX=\
		TokenInfo.x \
		Token.x \
		FloatToInteger.x \
		IntegerToFloat.x \
		StringToFloat.x \
		StringToInteger.x \
		FromStringConversionFactory.x \
		NumericConversionFactory.x \


#		LexicalAnalyzer.x \
#		TypeConversion.x \
#		ValidateBasicType.x\
#		AnyObject.x \
#		AnyTypeCreator.x \
#		StandardParser.x \
#		XMLParser.x \
#		JsonParser.x \
#		ParserI.x \
#		ConfigurationDatabase.x

        
PACKAGE=Core/BareMetal
ROOT_DIR=../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults
include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I.
INCLUDES += -I../L0Types
INCLUDES += -I../L1Portability
INCLUDES += -I../L2Objects
INCLUDES += -I../L3Streams

all: $(OBJS)    \
	        $(BUILD_DIR)/L4ConfigurationB$(LIBEXT) \
	        $(BUILD_DIR)/L4ConfigurationB.def \
                $(BUILD_DIR)/test2$(EXEEXT) 
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

