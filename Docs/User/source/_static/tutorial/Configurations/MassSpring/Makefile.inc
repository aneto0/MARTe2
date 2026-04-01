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
	RTApp-MassSpring-24-Sender-solution.x \
	RTApp-MassSpring-24-Receiver.x \
	RTApp-MassSpring-25-Receiver.x \
	RTApp-MassSpring-25-Receiver-solution.x \
	RTApp-MassSpring-26-Publisher.x \
	RTApp-MassSpring-27-Publisher.x \
	RTApp-MassSpring-27-Publisher-solution.x \
	RTApp-MassSpring-28-Publisher.x \
	RTApp-MassSpring-28-Subscriber.x \
	RTApp-MassSpring-29-Subscriber.x \
	RTApp-MassSpring-29-Subscriber-solution.x \
	RTApp-MassSpring-30.x \
	RTApp-MassSpring-31.x \
	RTApp-MassSpring-31-solution.x \
	RTApp-MassSpring-32.x \
	RTApp-MassSpring-33.x \
	RTApp-MassSpring-33-solution.x \
	RTApp-MassSpring-36.x 

#Location of the Build directory where the configuration file will be written to
BUILD_DIR?=.

#Location of the MakeDefaults directory. It is using defaults in the current project
MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

FREE_UDP_PORTS := $(shell ./FindFreeUDPPort.sh)
FREE_UDP_PORT_0 := $(word 1, $(FREE_UDP_PORTS))
FREE_UDP_PORT_1 := $(word 2, $(FREE_UDP_PORTS))
FREE_UDP_PORT_2 := $(word 3, $(FREE_UDP_PORTS))
FREE_UDP_PORT_3 := $(word 4, $(FREE_UDP_PORTS))
FREE_UDP_PORT_4 := $(word 5, $(FREE_UDP_PORTS))
FREE_UDP_PORT_5 := $(word 6, $(FREE_UDP_PORTS))
FREE_UDP_PORT_6 := $(word 7, $(FREE_UDP_PORTS))
FREE_UDP_PORT_7 := $(word 8, $(FREE_UDP_PORTS))
FREE_UDP_PORT_8 := $(word 9, $(FREE_UDP_PORTS))
FREE_UDP_PORT_9 := $(word 10, $(FREE_UDP_PORTS))
CFLAGS += -DTUTORIAL_UDP_PORT=$(FREE_UDP_PORT_0) -DTUTORIAL_UDP_PORT_1=$(FREE_UDP_PORT_1) -DTUTORIAL_UDP_PORT_2=$(FREE_UDP_PORT_2) -DTUTORIAL_UDP_PORT_3=$(FREE_UDP_PORT_3) -DTUTORIAL_UDP_PORT_4=$(FREE_UDP_PORT_4) -DTUTORIAL_UDP_PORT_5=$(FREE_UDP_PORT_5) -DTUTORIAL_UDP_PORT_6=$(FREE_UDP_PORT_6) -DTUTORIAL_UDP_PORT_7=$(FREE_UDP_PORT_7) -DTUTORIAL_UDP_PORT_8=$(FREE_UDP_PORT_8) -DTUTORIAL_UDP_PORT_9=$(FREE_UDP_PORT_9)


FREE_TOPIC_NAMES := $(shell ./GenerateTopicNames.sh)
FREE_TOPIC_NAME_0 := $(word 1, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_1 := $(word 2, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_2 := $(word 3, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_3 := $(word 4, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_4 := $(word 5, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_5 := $(word 6, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_6 := $(word 7, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_7 := $(word 8, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_8 := $(word 9, $(FREE_TOPIC_NAMES))
FREE_TOPIC_NAME_9 := $(word 10, $(FREE_TOPIC_NAMES))
CFLAGS += -DTUTORIAL_TOPIC_NAME_0=$(FREE_TOPIC_NAME_0) -DTUTORIAL_TOPIC_NAME_1=$(FREE_TOPIC_NAME_1) -DTUTORIAL_TOPIC_NAME_2=$(FREE_TOPIC_NAME_2) -DTUTORIAL_TOPIC_NAME_3=$(FREE_TOPIC_NAME_3) -DTUTORIAL_TOPIC_NAME_4=$(FREE_TOPIC_NAME_4) -DTUTORIAL_TOPIC_NAME_5=$(FREE_TOPIC_NAME_5) -DTUTORIAL_TOPIC_NAME_6=$(FREE_TOPIC_NAME_6) -DTUTORIAL_TOPIC_NAME_7=$(FREE_TOPIC_NAME_7) -DTUTORIAL_TOPIC_NAME_8=$(FREE_TOPIC_NAME_8) -DTUTORIAL_TOPIC_NAME_9=$(FREE_TOPIC_NAME_9)

USERNAME_UPPER := $(shell echo $(USER) | tr '[:lower:]' '[:upper:]')
CFLAGS += -DUSERNAME=$(USERNAME_UPPER)

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

