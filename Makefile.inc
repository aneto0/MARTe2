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
#Subprojects w.r.t. the main directory only (important to allow setting SPBM as an export variable - inside this Makefile, i.e. it cannot be in MakeDefaults).
#If SPB is directly exported as an environment variable it will also be evaluated as part of the subprojects SPB, thus potentially overriding its value.
#Main target and test subprojects.
SPBM?=Source/Core.x Source/App.x
SPBMT?=Test/Core/BareMetal.x Test/Core/Scheduler.x Test/Core/FileSystem.x Test.x Docs/User/source/_static/examples/Core.x
#This really has to be defined locally.
SUBPROJMAIN=$(SPBM:%.x=%.spb)
SUBPROJMAINTEST=$(SPBMT:%.x=%.spb)
SUBPROJMAINCLEAN=$(SPBM:%.x=%.spc)
SUBPROJMAINTESTCLEAN=$(SPBMT:%.x=%.spc)

MARTe2_MAKEDEFAULT_DIR?=MakeDefaults

ROOT_DIR=.
include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

all: $(OBJS) gtest core test
	echo  $(OBJS)

core: $(SUBPROJMAIN)
	echo  $(SUBPROJMAIN)

gtest:
ifeq ($(TARGET), x86-linux)
	make -C Lib/gtest-1.7.0 -f Makefile.$(TARGET)
else ifeq ($(TARGET), cov)
	make -C Lib/gtest-1.7.0 -f Makefile.$(TARGET)
endif

test: $(SUBPROJMAINTEST)
	echo  $(SUBPROJMAINTEST)

clean:: $(SUBPROJMAINCLEAN) $(SUBPROJMAINTESTCLEAN) clean_wipe_old

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

