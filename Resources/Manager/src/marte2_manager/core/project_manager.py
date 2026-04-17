#!/usr/bin/python3
__copyright__ = '''
    Copyright 2022 F4E | European Joint Undertaking for ITER and
    the Development of Fusion Energy ('Fusion for Energy').
    Licensed under the EUPL, Version 1.1 or - as soon they will be approved
    by the European Commission - subsequent versions of the EUPL (the "Licence")
    You may not use this work except in compliance with the Licence.
    You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl

    Unless required by applicable law or agreed to in writing,
    software distributed under the Licence is distributed on an "AS IS"
    basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
    or implied. See the Licence permissions and limitations under the Licence.
'''
__license__ = 'EUPL'
__author__ = 'Andre Neto'
__date__ = '10/04/2026'

###
# Imports
###
import copy
import logging
import os
import re

from marte2_manager.core.base_manager import BaseManager
from pathlib import Path
from string import Template

###
# Globals
###
logger = logging.getLogger(__name__)

###
# Classes
###

class ProjectManager(BaseManager):

    """
    Manage the creation of projects.
    """
    def __init__(self, app_def_data, resources_files, template_global_vars, project_name=None, project_path=None):
        super().__init__(app_def_data, resources_files, template_global_vars, project_name, project_path)

    def create_project(self):
        """
        Create a new project with the given name and path.
        """
        ok = True
        logger.info(f"Creating project {self._project_name} at {self._project_path}")

        folders = self._get_base_folder_structure()
        ok = self._create_folders(self._project_path, folders)
        if ok:
            ok = self._add_templates()
        return ok

    def validate_project(self):
        folders = self._get_base_folder_structure()
        return self._validate_folders(self._project_path, folders)

    def _add_templates(self):
        ok = self._add_makefiles()
        if ok:
            ok = self._add_gtest()
        if ok:
            ok = self._add_marte_app_sh()
        return ok

    def _add_makefiles(self):
        template_make_vars = copy.deepcopy(self._template_make_vars)
        src_folder_name = self._app_def_data['folders']['src']['name']
        test_folder_name = self._app_def_data['folders']['test']['name']
        gam_folder_name = self._app_def_data['components']['gams']['name']
        datasources_folder_name = self._app_def_data['components']['datasources']['name']
        interfaces_folder_name = self._app_def_data['components']['interfaces']['name']

        spb = []
        parent_folder_name = src_folder_name
        for folder_name in [gam_folder_name, datasources_folder_name, interfaces_folder_name]:
            spb.append(f"\t{parent_folder_name}/{folder_name}.x\\") 
        template_make_vars['spb'] = '\n'.join(spb)
        
        spbt = []
        parent_folder_name = test_folder_name
        for folder_name in [gam_folder_name, datasources_folder_name, interfaces_folder_name]:
            spbt.append(f"\t{parent_folder_name}/{folder_name}.x\\") 
        spbt.append(f"\t{test_folder_name}/GTest.x") 
        template_make_vars['spbt'] = '\n'.join(spbt)

        template_make_vars['target_1'] = 'all: $(OBJS) core test check-env\n\techo  $(OBJS)' 
        template_make_vars['target_2'] = 'core: $(SUBPROJ)\n\techo  $(SUBPROJ)' 
        template_make_vars['target_3'] = 'test: $(SUBPROJT)\n\techo  $(SUBPROJT)' 

        for makefile_name in ['Makefile.inc', 'Makefile.gcc', 'Makefile.cov']:
            ok = self._copy_template_text_file(makefile_name, Path(self._project_path) / self._project_name, template_make_vars)
            if not ok:
                break
        template_make_vars['spb'] = ''
        template_make_vars['spbt'] = ''
        template_make_vars['objsx'] = ''
        template_make_vars['root_dir'] = '../../'
        for parent_folder_name in [src_folder_name, test_folder_name]:
            if not ok:
                break
            for folder_name in [gam_folder_name, datasources_folder_name, interfaces_folder_name]:
                if not ok:
                    break
                for makefile_name in ['Makefile.inc', 'Makefile.gcc', 'Makefile.cov']:
                    ok = self._copy_template_text_file(makefile_name, Path(self._project_path) / self._project_name / parent_folder_name / folder_name, template_make_vars)
                    if not ok:
                        break
        return ok

    def _add_gtest(self):
        template_cpp_vars = copy.deepcopy(self._template_cpp_vars)
        src_folder_name = self._app_def_data['folders']['src']['name']
        test_folder_name = self._app_def_data['folders']['test']['name']

        template_cpp_vars['file'] = 'MainGTest.cpp'
        template_cpp_vars['class'] = 'MainGTest'
        ok = self._copy_template_text_file('MainGTest.cpp', Path(self._project_path) / self._project_name / test_folder_name / 'GTest', template_cpp_vars)

        if ok:
            template_make_vars = copy.deepcopy(self._template_make_vars)
            template_make_vars['root_dir'] = '../..'
            template_make_vars['target_1'] = 'all: $(OBJS) check-env\\\n\t$(BUILD_DIR)/MainGTest$(EXEEXT)\n\techo  $(OBJS)' 
            includes = []
            includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types')
            includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability')
            includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects')
            includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams')
            includes.append('INCLUDES += -I$(MARTe2_DIR)/Lib/gtest-1.7.0/include')
            template_make_vars['includes'] = '\n'.join(includes)

            libraries = []
            libraries.append('LIBRARIES   += -L$(MARTe2_DIR)/Build/$(TARGET)/Core/ -lMARTe2')
            libraries.append('LIBRARIES   += $(MARTe2_DIR)/Lib/gtest-1.7.0/libgtest.a $(MARTe2_DIR)/Lib/gtest-1.7.0/libgtest_main.a')
            template_make_vars['libraries'] = '\n'.join(libraries)

            libraries_static = 'LIBRARIES_STATIC+=$(shell find $(ROOT_DIR)/Build/$(TARGET) -name "*.a")'
            template_make_vars['libraries_static'] = libraries_static
            ok = self._copy_template_text_file('Makefile.inc', Path(self._project_path) / self._project_name / test_folder_name / 'GTest', template_make_vars)
        if ok:
            libraries_static_cov = []
            libraries_static_cov.append(f'LIBRARIES_STATIC  = $(shell find $(ROOT_DIR)/{src_folder_name}/ -name "*.a")')
            libraries_static_cov.append(f'LIBRARIES_STATIC += $(shell find $(ROOT_DIR)/{test_folder_name}/ -name "*.a")')
            template_make_vars['libraries_static_cov'] = '\n'.join(libraries_static_cov)

            libraries_cov = []
            libraries_cov.append('LIBRARIES  = -L$(MARTe2_DIR)/Build/x86-linux/Core/ -lMARTe2')
            libraries_cov.append('LIBRARIES += -lpthread')
            libraries_cov.append('LIBRARIES += $(MARTe2_DIR)/Lib/gtest-1.7.0/libgtest.a')
            template_make_vars['libraries_cov'] = '\n'.join(libraries_cov)

            ok = self._copy_template_text_file('Makefile.cov', Path(self._project_path) / self._project_name / test_folder_name / 'GTest', template_make_vars)

        if ok:
            ok = self._copy_template_text_file('Makefile.gcc', Path(self._project_path) / self._project_name / test_folder_name / 'GTest', template_make_vars)
        return ok


    def _add_marte_app_sh(self):
        template_marte_app_sh_vars = copy.deepcopy(self._template_marte_app_sh_vars)
        startup_folder_name = self._app_def_data['folders']['startup']['name']
        ok = self._copy_template_text_file('MARTeApp.sh', Path(self._project_path) / self._project_name / startup_folder_name, template_marte_app_sh_vars)
        return ok
