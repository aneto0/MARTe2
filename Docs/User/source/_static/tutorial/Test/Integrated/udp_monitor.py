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
__date__ = '30/03/2026'

'''
Monitor signals received using UDP
'''
import argparse
import logging
import socket
import struct
import sys

logger = logging.getLogger(__name__)
# Create handlers
console_handler = logging.StreamHandler()

# Create formatters and add it to handlers
console_handler_format = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
console_handler.setFormatter(console_handler_format)

signals = {
    "1" : [
        ("Thread1CycleTime", "I", 1),
        ("Thread1CycleTimeAverage", "I", 1),
        ("Thread1CycleTimeStdDev", "I", 1),
        ("Thread1CycleTimeMax", "I", 1),
        ("Thread1CycleTimeMin", "I", 1),
        ("Thread1CycleTimeHistogram", "I", 11),
        ("Thread1FreeTimeHistogram", "I", 11),
        ("GAMsExecutionTime", "I", 1),
    ],
    "2" : [
        ("ReferencePosition", "d", 1)
    ]
}

def listen_and_print(signals, udp_ip, udp_port):

    # Build struct format (little endian)
    fmt = "<"
    for _, t, n in signals:
        fmt += t * n

    packet_size = struct.calcsize(fmt)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((udp_ip, udp_port))

    logger.info(f"Listening on UDP port {udp_port} (expecting {packet_size} bytes)")

    while True:
        data, _ = sock.recvfrom(4096)

        if len(data) < packet_size:
            print("Incomplete packet")
            continue

        values = struct.unpack(fmt, data[:packet_size])

        idx = 0
        output_lines = []

        for name, t, n in signals:
            if n == 1:
                value = values[idx]
                idx += 1
            else:
                value = values[idx:idx+n]
                idx += n

            output_lines.append(f"{name:35}: {value}")

        # overwrite screen
        sys.stdout.write("\033[H\033[J")  # clear screen
        sys.stdout.write("\n".join(output_lines))
        sys.stdout.flush()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Plot signals from the mass spring model', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    log_options = ['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG']
    parser.add_argument('-l', '--log_level', type=str, help='Log level, one of: CRITICAL, ERROR, WARNING, INFO, DEBUG', default='INFO', choices=log_options)
    parser.add_argument('-p', '--port', type=int, help='UDP port to listen', required=True)
    parser.add_argument('-i', '--ip', type=str, help='UDP IP to listen', default='0.0.0.0')
    parser.add_argument('-s', '--signals_id', type=str, help='ID of the expected signal struct', default='1')

    args = parser.parse_args()

    log_criticalities = [logging.CRITICAL, logging.ERROR, logging.WARNING, logging.INFO, logging.DEBUG]
    log_criticality = log_criticalities[log_options.index(args.log_level)]
    # Add handlers to the logger
    root_logger = logging.getLogger()
    root_logger.addHandler(console_handler)
    root_logger.setLevel(log_criticality)
    console_handler.setLevel(log_criticality)

    signals_struct = signals[args.signals_id]
    listen_and_print(signals_struct, args.ip, args.port)

