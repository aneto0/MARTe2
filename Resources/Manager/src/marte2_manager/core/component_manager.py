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
__date__ = '12/04/2026'

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

class ComponentManager(BaseManager):

    """
    Manage the creation of components.
    """
    def __init__(self, app_def_data, resources_files, template_global_vars, project_name=None, project_path=None):
        super().__init__(app_def_data, resources_files, template_global_vars, project_name, project_path)

    def create_component(self, comp_type, comp_name, comp_namespace):
        """
        Create a new component of the given type.
        """
        self._comp_type = comp_type
        self._comp_name = comp_name
        self._comp_namespace = comp_namespace
        ok = self._add_component_folders()
        if ok:
            ok = self._add_makefiles()
        if ok:
            ok = self._patch_parent_makefiles()
        if ok:
            ok = self._add_component_files()
        return ok
    
    def _add_component_folders(self):
        src_name = self._app_def_data['folders']['src']['name']
        test_name = self._app_def_data['folders']['test']['name']
        comp_type_name = self._app_def_data['components'][self._comp_type]['name']
        comp_folder_name = self._comp_name
        folders = {
            self._project_name: {
                src_name: {
                    comp_type_name: {
                        comp_folder_name: {}
                    }
                },
                test_name: {
                    comp_type_name: {
                        comp_folder_name: {}
                    }
                }
            }
        }
        return self._create_folders(self._project_path, folders, only_leafs=True)

    def _add_component_files(self):
        comp_type_name = self._app_def_data['components'][self._comp_type]['name']
        src_file_template_name = self._app_def_data['components'][self._comp_type]['src_template_name'] + '.cpp'
        h_file_template_name = self._app_def_data['components'][self._comp_type]['src_template_name'] + '.h'
        test_src_file_template_name = self._app_def_data['components'][self._comp_type]['test_template_name'] + '.cpp'
        test_h_file_template_name = self._app_def_data['components'][self._comp_type]['test_template_name'] + '.h'
        gtest_src_file_template_name = self._app_def_data['components'][self._comp_type]['gtest_template_name'] + '.cpp'

        src_folder_name = self._app_def_data['folders']['src']['name']
        test_folder_name = self._app_def_data['folders']['test']['name']
        template_cpp_vars = copy.deepcopy(self._template_cpp_vars)
        template_h_vars = copy.deepcopy(self._template_h_vars)

        template_cpp_vars['file'] = f'{self._comp_name}.cpp'
        template_cpp_vars['class'] = self._comp_name
        template_cpp_vars['namespace'] = self._comp_namespace

        template_h_vars['file'] = f'{self._comp_name}.h'
        template_h_vars['class'] = self._comp_name
        template_h_vars['namespace'] = self._comp_namespace
        template_h_vars['header_guard'] = f'_{self._comp_namespace.upper()}_{self._comp_name.upper()}_H_' 

        ok = self._copy_template_text_file(src_file_template_name, Path(self._project_path) / self._project_name / src_folder_name / comp_type_name / self._comp_name, template_cpp_vars, target_name=f'{self._comp_name}.cpp')
        if ok:
            ok = self._copy_template_text_file(h_file_template_name, Path(self._project_path) / self._project_name / src_folder_name / comp_type_name / self._comp_name, template_h_vars, target_name=f'{self._comp_name}.h')
        if ok:
            template_cpp_vars['file'] = f'{self._comp_name}Test.cpp'
            template_cpp_vars['class'] = f'{self._comp_name}Test'
            ok = self._copy_template_text_file(test_src_file_template_name, Path(self._project_path) / self._project_name / test_folder_name / comp_type_name / self._comp_name, template_cpp_vars, target_name=f'{self._comp_name}Test.cpp')
        if ok:
            template_h_vars['file'] = f'{self._comp_name}Test.h'
            template_h_vars['class'] = f'{self._comp_name}Test'
            template_h_vars['src_class'] = f'{self._comp_name}'
            template_h_vars['header_guard'] = f'_{self._comp_namespace.upper()}_{self._comp_name.upper()}_TEST_H_' 
            ok = self._copy_template_text_file(test_h_file_template_name, Path(self._project_path) / self._project_name / test_folder_name / comp_type_name / self._comp_name, template_h_vars, target_name=f'{self._comp_name}Test.h')

        if ok:
            template_cpp_vars['file'] = f'{self._comp_name}GTest.cpp'
            template_cpp_vars['class'] = f'{self._comp_name}GTest'
            template_cpp_vars['src_class'] = f'{self._comp_name}Test'
            ok = self._copy_template_text_file(gtest_src_file_template_name, Path(self._project_path) / self._project_name / test_folder_name / comp_type_name / self._comp_name, template_cpp_vars, target_name=f'{self._comp_name}GTest.cpp')

        return ok

    def _add_makefiles(self):
        comp_type_name = self._app_def_data['components'][self._comp_type]['name']
        src_folder_name = self._app_def_data['folders']['src']['name']
        test_folder_name = self._app_def_data['folders']['test']['name']
        template_make_vars = copy.deepcopy(self._template_make_vars)

        template_make_vars['root_dir'] = '../../../'
        template_make_vars['package'] = f'PACKAGE = {comp_type_name}'
        template_make_vars['objsx'] = f'{self._comp_name}.x'
        template_make_vars['includes'] = self._get_marte2_includes() 
        template_make_vars['target_1'] = f'all: $(OBJS) $(SUBPROJ) check-env \\\n\t$(BUILD_DIR)/{self._comp_name}$(LIBEXT) \\\n\t$(BUILD_DIR)/{self._comp_name}$(DLLEXT)\n\techo  $(OBJS)' 

        for makefile_name in ['Makefile.inc', 'Makefile.gcc', 'Makefile.cov']:
            ok = self._copy_template_text_file(makefile_name, Path(self._project_path) / self._project_name / src_folder_name / comp_type_name / self._comp_name, template_make_vars)
            if not ok:
                break

        template_make_vars['package'] = f'PACKAGE = {comp_type_name}'
        template_make_vars['objsx'] = f'{self._comp_name}Test.x\\\n\t{self._comp_name}GTest.x'
        template_make_vars['includes'] = self._get_marte2_includes() 
        template_make_vars['includes'] += f'\nINCLUDES += -I$(ROOT_DIR)/{src_folder_name}/{comp_type_name}/{self._comp_name}' 
        template_make_vars['includes'] += '\nINCLUDES += -I$(MARTe2_DIR)/Lib/gtest-1.7.0/include'
        template_make_vars['target_1'] = f'all: $(OBJS) $(SUBPROJ) check-env \\\n\t$(BUILD_DIR)/{self._comp_name}Test$(LIBEXT)\n\techo  $(OBJS)' 

        for makefile_name in ['Makefile.inc', 'Makefile.gcc', 'Makefile.cov']:
            if not ok:
                break
            ok = self._copy_template_text_file(makefile_name, Path(self._project_path) / self._project_name / test_folder_name / comp_type_name / self._comp_name, template_make_vars)

        return ok

    def _patch_parent_makefiles(self):
        comp_type_name = self._app_def_data['components'][self._comp_type]['name']
        src_folder_name = self._app_def_data['folders']['src']['name']
        test_folder_name = self._app_def_data['folders']['test']['name']

        makefile_path = Path(self._project_path) / self._project_name / src_folder_name / comp_type_name / 'Makefile.inc'
        ok = self._patch_makefile_assignment(makefile_path, 'SPB', f'{self._comp_name}.x')
        if ok:
            makefile_path = Path(self._project_path) / self._project_name / test_folder_name / comp_type_name / 'Makefile.inc'
            ok = self._patch_makefile_assignment(makefile_path, 'SPB', f'{self._comp_name}.x')
        return ok

    def _patch_makefile_assignment(self, makefile_path, var_name, var_value):
        ok = True
        try:
            with open(makefile_path, 'r') as f:
                makefile_text = f.read()
            pattern = re.compile(r"(^SPB\s*=\s*)(.*?)(?=^\S.*=|\Z)", re.MULTILINE | re.DOTALL,)
            match = pattern.search(makefile_text)
            if match:
                prefix = match.group(1)
                body = match.group(2)
                # Collect current entries, removing line continuations and extra spaces
                prefix = prefix.replace('\\\n\t', ' ').strip()
                existing_entries = body.replace('\\\n\t', ' ').strip()
                entries = [item for item in existing_entries.split() if item]
                logger.debug(f'In {makefile_path}, current entries for {var_name}: {entries}')
                if var_value not in entries:
                    entries.append(var_value)
                new_content = f'{prefix} {entries[0]} \\'
                for entry in entries[1:]:
                    new_content += f'\n\t{entry} \\'
                new_content = new_content[:-1] # Remove the last \
            else:
                new_content = f'{prefix} = {var_value}'
            new_content += '\n\n'
            start, end = match.span()
            new_makefile_text = makefile_text[:start] + new_content + makefile_text[end:]
            with open(makefile_path, 'w') as f:
                f.write(new_makefile_text)
        except Exception as e:
            logger.error(f'Failed to patch makefile {makefile_path}: {e}')
            ok = False
        return ok

    def _get_marte2_includes(self):
        includes = []

        includes.append('INCLUDES += -I.')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4HttpService')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Logger')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L6App')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L3Streams')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L4HttpService')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L4LoggerService')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L6App')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4LoggerService')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4Messages')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4StateMachine')
        includes.append('INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs')

        return '\n'.join(includes)

    def _add_component_templates(self, comp_folder_name):
        ok = True
        return ok
