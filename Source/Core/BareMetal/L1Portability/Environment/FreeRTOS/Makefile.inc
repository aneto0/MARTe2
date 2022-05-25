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

PACKAGE=Core/BareMetal/L1Portability
ROOT_DIR=../../../../../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

OBJSX=  BasicConsole.x \
        ErrorManagement_Gen.x \
    HardwareI.x \
    HighResolutionTimerCalibrator.x \
    LoadableLibrary.x  \
    MemoryCheck_Gen.x  \
    MemoryOperationsHelper_CLIB_Gen.x \
    Sleep.x \
    StandardHeap.x \
    StringHelperExtras_Gen.x \
    StringHelper_CLIB_Gen.x \
    TimeStamp.x
#HighResolutionTimerCalibrator.x 
    

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES+=-I../../
INCLUDES+=-I../../../L0Types

all: $(OBJS) $(CPPS) \
    $(BUILD_DIR)/L1Portability$(LIBEXT)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
