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
__date__ = '09/04/2026'

'''
Modify signals over OPCUA
'''
import argparse
import logging
import opcua
import sys
import time

from opcua import Client, ua

logger = logging.getLogger(__name__)
# Create handlers
console_handler = logging.StreamHandler()

# Create formatters and add it to handlers
console_handler_format = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
console_handler.setFormatter(console_handler_format)

def get_node(node, node_name):
    ret = None

    for child in node.get_children():
        try:
            name = child.get_display_name().Text
            if name == node_name:
                ret = child
                break
        except:
            ret = None

    return ret

def get_node_from_dot_name(o_node, dot_name):

    dot_name_arr = dot_name.split('.')
    node = o_node
    for dot_name_i in dot_name_arr:
        node = get_node(node, dot_name_i)
        if node is None:
            break

    return node


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Browse signal from the mass spring model over OPCUA', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    log_options = ['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG']
    parser.add_argument('-l', '--log_level', type=str, help='Log level, one of: CRITICAL, ERROR, WARNING, INFO, DEBUG', default='INFO', choices=log_options)
    parser.add_argument('-a', '--address', type=str, help='OPCUA address to connect to', default='localhost')
    parser.add_argument('-p', '--port', type=int, help='OPCUA port to connect to', required=True)
    parser.add_argument('-s', '--signal_name', type=str, help='OPCUA port to connect to', required=True)
    parser.add_argument('-v', '--signal_value', type=float, help='OPCUA port to connect to', required=True)
    parser.add_argument("--print_time_stamps", action="store_true")


    args = parser.parse_args()

    log_criticalities = [logging.CRITICAL, logging.ERROR, logging.WARNING, logging.INFO, logging.DEBUG]
    log_criticality = log_criticalities[log_options.index(args.log_level)]
    # Add handlers to the logger
    root_logger = logging.getLogger()
    root_logger.addHandler(console_handler)
    root_logger.setLevel(log_criticality)
    console_handler.setLevel(log_criticality)

    logging.getLogger('opcua').setLevel(logging.CRITICAL)
    url = f'opc.tcp://{args.address}:{args.port}'
    logger.info(f'Connecting to OPCUA server at: {url}')

    client = Client(url)
    client.connect()

    objects = client.get_objects_node()
    node = get_node_from_dot_name(objects, args.signal_name)
    if node is not None:
        dv = node.get_data_value()
        variant_type = dv.Value.VariantType
        if variant_type == ua.VariantType.Double:
            value = ua.Variant(args.signal_value, variant_type)
            node.set_value(value)
        elif variant_type == ua.VariantType.Int32 or variant_type == ua.VariantType.UInt32:
            value = ua.Variant(int(args.signal_value), variant_type)
            node.set_value(value)
        else:
            logger.critical(f'Invalid type: {variant_type}')
    else:
        logger.critical(f'Signal with name {args.signal_name} could not be found in the server')
    client.disconnect()

