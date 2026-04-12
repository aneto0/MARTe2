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
import argparse
import getpass
import json
import logging
import sys

from datetime import datetime
from importlib import resources
from marte2_manager.core.component_manager import ComponentManager
from marte2_manager.core.project_manager import ProjectManager

###
# Globals
###
logger = logging.getLogger(__name__)

# Create handlers
console_handler = logging.StreamHandler()

# Create formatters and add it to handlers
console_handler_format = logging.Formatter('%(asctime)s - %(filename)s:%(lineno)d - %(levelname)s - %(message)s')
console_handler.setFormatter(console_handler_format)

# Add handlers to the logger
root_logger = logging.getLogger()
root_logger.addHandler(console_handler)

def main():

    parser = argparse.ArgumentParser(description = 'Rename all the elements and packages under a  given package or element', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    logOptions = ['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG']
    parser.add_argument('-l', '--loglevel', type=str, help='Log level, one of: CRITICAL, ERROR, WARNING, INFO, DEBUG', default='INFO', choices=logOptions)
    parser.add_argument('--author', type=str, help='Author name to use in the templates', default=None, required=False)
    parser.add_argument('--app_def', type=str, help='Path to the application project definition file. If not defined, a default application project definition file will be used', default=None, required=False)
    parser.add_argument('--project_name', type=str, help='Name of the project to create', required=True)
    parser.add_argument('--project_path', type=str, help='Path where the project should be created', required=True)

    command_parsers = parser.add_subparsers(dest='command', help='Sub-commands')
    new_project_parser = command_parsers.add_parser('new', help='Create a new project')

    add_parser = command_parsers.add_parser('add', help='Create a new component in the project')
    add_parser.add_argument('--cpt_type', type=str, help='Type of the component to create', choices=['gams', 'datasources', 'interfaces'], required=True)
    add_parser.add_argument('--cpt_name', type=str, help='Name of the component to create', required=True)
    add_parser.add_argument('--cpt_namespace', type=str, help='Namespace of the component to create', required=True)

    args = parser.parse_args()

    log_criticalities = [logging.CRITICAL, logging.ERROR, logging.WARNING, logging.INFO, logging.DEBUG]
    log_criticality = log_criticalities[logOptions.index(args.loglevel)]
    root_logger.setLevel(log_criticality)
    console_handler.setLevel(log_criticality)

    ok = True

    app_def = args.app_def
    if app_def is None:
        logger.info(f'Using default application project definition file')
        f = resources.files('marte2_manager') / 'resources' / 'default_app_def.json'
        app_def_data = json.loads(f.read_text(encoding='utf-8'))
    else:
        try:
            with open(app_def) as f:
                app_def_data = json.load(f)
        except Exception as e:
            logger.error(f'Failed to load application project definition file: {e}')
            ok = False

    if ok:
        author = args.author
        if author is None:
            author = getpass.getuser()
        template_global_vars = {
            'date': datetime.now().strftime("%d/%m/%Y"),
            'author': author
        }

    if ok:
        project_manager = ProjectManager(app_def_data, resources.files('marte2_manager'), template_global_vars, args.project_name, args.project_path)
        if args.command == 'new':
            ok = project_manager.create_project()
        elif args.command == 'add':
            ok = project_manager.validate_project()
            if ok:
                component_manager = ComponentManager(app_def_data, resources.files('marte2_manager'), template_global_vars, args.project_name, args.project_path)
                ok = component_manager.create_component(args.cpt_type, args.cpt_name, args.cpt_namespace)
            if not ok:
                logger.error(f'Project validation failed. Cannot add component.')

    ret = 0
    if not ok:
        ret = 1

    sys.exit(ret)

if __name__ == '__main__':
    main()