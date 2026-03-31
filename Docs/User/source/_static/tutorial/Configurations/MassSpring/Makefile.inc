#############################################################
#
# Copyright 2026 F4E | European Joint Undertaking for ITER 
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

#Named of the unit files to be compiled
OBJSX=RTApp-MassSpring-23-Sender.x \
	RTApp-MassSpring-24-Sender.x \
	RTApp-MassSpring-24-Sender-solution.x

#Location of the Build directory where the configuration file will be written to
BUILD_DIR?=.

#Location of the MakeDefaults directory. It is using defaults in the current project
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

FREE_UDP_PORTS := $(shell ./FindFreeUDPPort.sh)
FREE_UDP_PORT_0 := $(word 1, $(FREE_UDP_PORTS))
FREE_UDP_PORT_1 := $(word 2, $(FREE_UDP_PORTS))
FREE_UDP_PORT_2 := $(word 3, $(FREE_UDP_PORTS))
CFLAGS += -DTUTORIAL_UDP_PORT=$(FREE_UDP_PORT_0) -DTUTORIAL_UDP_PORT_1=$(FREE_UDP_PORT_1) -DTUTORIAL_UDP_PORT_2=$(FREE_UDP_PORT_2)

all: check-env $(OBJS) 
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

check-env:
ifndef MARTe2_DIR
    $(error MARTe2_DIR is undefined)
endif
ifndef TARGET
    $(error TARGET is undefined)
endif

