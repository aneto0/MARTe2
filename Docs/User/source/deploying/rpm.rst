.. date: 18/05/2021
   author: Andre' Neto
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.

RPM
===

The MARTe2 core and MARTe2 components are packaged as `RPM files <https://vcis.f4e.europa.eu/dist/marte2/>`_. They can be installed individually or by installing the target specific YUM release RPM (look for RPM files named ``release``).

Building the RPM from sources
-----------------------------

The MARTe2 core and components RPM files can be built by issuing the command ``make -f Makefile.rpmbuild``.


Project specific RPM
--------------------

A project specific RPM can be built using the rpmbuild MakeDefaults :vcisgl:`definitions <MARTe2/tree/master/MakeDefaults/MakeStdLibDefs.rpmbuild>` and :vcisgl:`rules <MARTe2/tree/master/MakeDefaults/MakeStdLibRules.rpmbuild>`.

The following is an example of a project specific RPM Makefile:

.. code-block:: makefile
 
  TARGET=rpmbuild
  ROOT_DIR=.
  
  FOLDERSX?=Source.x Makefile.x86-linux.x Makefile.inc.x Startup.x Configurations.x
  
  MARTe2_MAKEDEFAULT_DIR?=$(MARTe2_DIR)/MakeDefaults
  
  #If not set try to get the project version from a git tag (in the format vx.y, i.e. assuming it starts with v)
  M2_RPM_VERSION?=$(shell git tag | sort -V | tail -1 | cut -c2-)
  M2_RPM_NAME?=SUPITA
  M2_RPM_ID?=supita
  M2_RPM_REQUIRES=marte2-core marte2-components marte2-components-ccs
  M2_RPM_SPEC_FILE_PATH=$(MARTe2_DIR)/Resources/RPM/
  
  #If not specified, the MakeDefaults behaviour is to get the latest tag from git
  #M2_RPM_VERSION=dev$(shell git rev-parse --short HEAD)
  
  #Project specific folders that are to be installed. It really has to be escaped with a space.
  M2_RPM_OTHER_FOLDERS=Startup\ Configurations
  #Get current CODAC version
  CODAC_CCS=`codac-version | cut -c 1,3`
  #If not set it will default to standard OS (e.g. el7)
  M2_RPM_CUSTOM_DIST="ccs$(CODAC_CCS)"
  
  include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)
  
  all: $(OBJS) $(BUILD_DIR)/$(M2_RPM_ID)-$(M2_RPM_VERSION).rpm check-env
  	echo  $(OBJS)
  
  check-env:
  ifndef MARTe2_DIR
      $(error MARTe2_DIR is undefined)
  endif
  ifndef MARTe2_Components_DIR
      $(error MARTe2_Components_DIR is undefined)
  endif
  
  include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)
  
