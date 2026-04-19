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
__date__ = '29/03/2026'

'''
Plot signals from csv
'''

import argparse
import csv
import matplotlib.pyplot as plt
import logging

logger = logging.getLogger(__name__)
# Create handlers
console_handler = logging.StreamHandler()

# Create formatters and add it to handlers
console_handler_format = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
console_handler.setFormatter(console_handler_format)

def parse_header(reader):
    ret = {
    }
    for field in reader.fieldnames:
        field_name = field.split(' ')[0]
        if field_name[0] == '#':
            field_name = field_name[1:]
        field_type = field.split(' ')[1].split('(')[1].split(')')[0]
        field_n_elems = int(field.split(' ')[1].split('[')[1].split(']')[0])
        if 'int' in field_type:
            field_cast = int
        else:
            field_cast = float
        ret[field_name] = {
            'id': field,
            'cast': field_cast,
            'type': field_type,
            'n_elems': field_n_elems
        }
    return ret

def parse_array(arr_str, field_cast):
    return [field_cast(x) for x in arr_str.strip("{}").split()]

def get_value(val_str, val_field):
    ret = None
    if val_field['n_elems'] > 1:
        ret = parse_array(val_str, val_field['cast'])
    else:
        ret = val_field['cast'](val_str)
    return ret

def plot_signals(time, signals):
    for name, values in signals.items():
        plt.plot(time, values, label=name)
        
    plt.xlabel('Time')
    plt.ylabel('Signal Value')
    plt.title('Signals from CSV')
    plt.legend()
    plt.grid()
    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'SSMGAM parameters for a mass spring model', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    log_options = ['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG']
    parser.add_argument('-l', '--log_level', type=str, help='Log level, one of: CRITICAL, ERROR, WARNING, INFO, DEBUG', default='INFO', choices=log_options)
    parser.add_argument('-f', '--filename', type=str, help='Filename to plot', required=True)
    parser.add_argument('-s', '--signals', nargs='+', help='Signals to plot', required=True)
    parser.add_argument('-t', '--time_signal_name', type=str, help='Name of the time signal', default='Time')
    parser.add_argument('-tm', '--time_multiplier', type=float, help='Time signal multiplier', default=1e-6)

    args = parser.parse_args()

    log_criticalities = [logging.CRITICAL, logging.ERROR, logging.WARNING, logging.INFO, logging.DEBUG]
    log_criticality = log_criticalities[log_options.index(args.log_level)]
    # Add handlers to the logger
    root_logger = logging.getLogger()
    root_logger.addHandler(console_handler)
    root_logger.setLevel(log_criticality)
    console_handler.setLevel(log_criticality)


    time_signal_id = None
    time = []
    signals = {}  

    ok = True
    with open(args.filename, 'r') as f:
        reader = csv.DictReader(f)
        header = parse_header(reader)

        for signal_name in args.signals:
            ok = signal_name in header
            if ok:
                signals[signal_name] = []
            else:
                logger.critical(f'Signal with name {signal_name} not available in the CSV')
                break
        if ok:
            ok = args.time_signal_name in header
            time_signal_id = header[args.time_signal_name]['id']
            if not ok:
                logger.critical(f'Time signal with name {args.time_signal_name} not available in the CSV')

        if ok:
            for row in reader:
                for signal_name in args.signals:
                    signal_id = header[signal_name]['id']
                    signals[signal_name].append(get_value(row[signal_id], header[signal_name]))

                time.append(get_value(row[time_signal_id], header[args.time_signal_name]))

    if ok:
        plot_signals(time, signals)
