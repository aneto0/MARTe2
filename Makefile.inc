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
SPB=
SPBC=Source/Core.x Source/App.x

SPBT=Test/Core/BareMetal.x Test/Core/Scheduler.x Test/Core/FileSystem.x Test/GTest.x Docs/User/source/_static/examples/Core.x
SUBPROJCORE = $(SPBC:%.x=%.spb)
SUBPROJTEST = $(SPBT:%.x=%.spb)
SUBPROJCOREC = $(SPBC:%.x=%.spc)
SUBPROJTESTC = $(SPBT:%.x=%.spc)

MARTe2_MAKEDEFAULT_DIR?=MakeDefaults

ROOT_DIR=.
include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

all: $(OBJS) gtest core test
	echo  $(OBJS)

core: $(SUBPROJCORE)
	echo  $(SUBPROJCORE)

gtest:
	make -C Lib/gtest-1.7.0 -f Makefile.$(TARGET)

test: $(SUBPROJTEST)
	echo  $(SUBPROJTEST)

clean: $(SUBPROJCOREC) $(SUBPROJTESTC)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

