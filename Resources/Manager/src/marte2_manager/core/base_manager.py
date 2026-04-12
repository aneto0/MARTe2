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

from pathlib import Path
from string import Template

###
# Globals
###
logger = logging.getLogger(__name__)

###
# Classes
###

class BaseManager(object):

    """
    Base manager class.
    """
    def __init__(self, app_def_data, resources_files, template_global_vars, project_name=None, project_path=None):
        self._app_def_data = app_def_data 
        self._resources_files = resources_files
        self._template_global_vars = template_global_vars
        self._project_name = project_name
        self._project_path = project_path
        self._template_make_vars = {
            'header_text': '\n'.join(self._app_def_data['makefiles']['header']['text']),
            'spb': ' ',
            'objsx': ' ',
            'package': ' ',
            'root_dir': '.',
            'includes': '',
            'libraries': '',
            'libraries_static': '',
            'libraries_static_cov': '',
            'file': '',
            'class': '',
            'target_1': '',
            'target_2': '',
            'target_3': '',
            'target_4': ''
        }
        self._template_cpp_vars = {
            'header_text': '\n'.join(self._app_def_data['src']['.cpp']['header']['text']),
            'file': '',
            'class': '',
            'namespace': ''
        }
        self._template_h_vars = {
            'header_text': '\n'.join(self._app_def_data['src']['.h']['header']['text']),
            'file': '',
            'class': '',
            'namespace': '',
            'header_guard': ''
        }

    def _copy_template_text_file(self, template_name, destination_path, template_vars, target_name = None):
        ok = True
        try:
            if target_name is None:
                target_name = template_name
            destination_path = destination_path / target_name
            template_f = self._resources_files / 'templates' / template_name
            template_content = template_f.read_text(encoding="utf-8")
            template_content = self._expand_template(template_content, template_vars)
            template_content = self._expand_template(template_content, template_vars) # Expand twice to allow for nested variables. Do not remove!
            template_content = self._expand_template(template_content, self._template_global_vars) # Expand global variables at the end to allow for their use in the templates
            template_content = self._remove_extra_blank_lines(template_content)
            destination_path.write_text(template_content, encoding="utf-8")
        except Exception as e:
            logger.error(f"Failed to copy template {template_name} to {destination_path}: {e}")
            ok = False
        return ok

    def _expand_template(self, template_content, template_vars):
        template = Template(template_content)
        return template.safe_substitute(template_vars)

    def _remove_extra_blank_lines(self, text):
        return re.sub(r"\n\s*\n+", "\n\n", text)

    def _create_folders(self, base_path, folders, only_leafs=False):
        """
        Create the folders in the given path.
        """
        ok = True
        for folder_name in folders:
            if not ok:
                break
            folder_path = os.path.join(base_path, folder_name)
            try:
                create_folder = True
                if only_leafs:
                    subfolders = folders[folder_name]
                    if isinstance(subfolders, dict) and len(subfolders) > 0:
                        create_folder = False
                if create_folder:
                    ret = os.makedirs(folder_path, exist_ok = False)
                    logger.debug(f"Created folder {folder_path}")
            except FileExistsError:
                logger.critical(f"Folder {folder_path} already exists.")
                ok = False
            if ok:
                subfolders = folders[folder_name]
                ok = self._create_folders(folder_path, subfolders, only_leafs)

        return ok

    def _get_base_folder_structure(self): 
        folders = {self._project_name: {}}
        folders[self._project_name][self._app_def_data['folders']['src']['name']] = {
            self._app_def_data['components']['gams']['name']: {},
            self._app_def_data['components']['datasources']['name']: {},
            self._app_def_data['components']['interfaces']['name']: {}
        }
        folders[self._project_name][self._app_def_data['folders']['test']['name']] = {
            self._app_def_data['components']['gams']['name']: {},
            self._app_def_data['components']['datasources']['name']: {},
            self._app_def_data['components']['interfaces']['name']: {},
            'GTest': {}
        }
        folders[self._project_name][self._app_def_data['folders']['configurations']['name']] = {
        }
        folders[self._project_name][self._app_def_data['folders']['docs']['name']] = {
        }
        folders[self._project_name][self._app_def_data['folders']['startup']['name']] = {
        }
        return folders

    def _validate_folders(self, base_path, folders):
        """
        Validate that the folders in the given path match the expected structure.
        """
        ok = True
        for folder_name in folders:
            if not ok:
                break
            folder_path = os.path.join(base_path, folder_name)
            if not os.path.isdir(folder_path):
                logger.critical(f"Folder {folder_path} does not exist.")
                ok = False
            if ok:
                subfolders = folders[folder_name]
                ok = self._validate_folders(folder_path, subfolders)

        return ok