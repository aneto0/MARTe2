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
__date__ = '22/03/2026'

'''
Helper to generate configuration of SSMGAM for a mass spring damper system
'''

import argparse
import logging
import numpy as np

logger = logging.getLogger(__name__)
# Create handlers
console_handler = logging.StreamHandler()

# Create formatters and add it to handlers
console_handler_format = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
console_handler.setFormatter(console_handler_format)

def mass_spring_damper_state_space_numpy(m, c, k, x0, v0, Ts):
    if m <= 0:
        raise ValueError("Mass must be greater than 0.")
    if Ts <= 0:
        raise ValueError("Sampling time Ts must be greater than 0.")

    A = np.array([
        [0.0, 1.0],
        [-k / m, -c / m]
    ])

    B = np.array([
        [0.0],
        [1.0 / m]
    ])

    C = np.array([
        [1.0, 0.0],
        [0.0, 1.0]
    ])

    D = np.array([
        [0.0],
        [0.0]
    ])

    x_init = np.array([[x0], [v0]])

    # Tustin discretization
    I = np.eye(A.shape[0])
    Ad = np.linalg.solve(I - 0.5 * Ts * A, I + 0.5 * Ts * A)
    Bd = np.linalg.solve(I - 0.5 * Ts * A, Ts * B)
    Cd = C.copy()
    Dd = D.copy()

    return {
        "A": A,
        "B": B,
        "C": C,
        "D": D,
        "Ad": Ad,
        "Bd": Bd,
        "Cd": Cd,
        "Dd": Dd,
        "x0": x_init
    }

def msd_pid_gains(m, c, k, wn, zeta, alpha):
    """
    Compute continuous-time PID gains for a mass-spring-damper system
    using simple pole placement.
    """
    p3 = alpha * wn

    Kd = m * (2.0 * zeta * wn + p3) - c
    Kp = m * (wn**2 + 2.0 * zeta * wn * p3) - k
    Ki = m * (wn**2) * p3

    return Kp, Ki, Kd

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'SSMGAM parameters for a mass spring model', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    log_options = ['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG']
    parser.add_argument('-l', '--log_level', type=str, help='Log level, one of: CRITICAL, ERROR, WARNING, INFO, DEBUG', default='INFO', choices=log_options)
    parser.add_argument('-m', '--mass', type=float, help='Mass in kg', default=1.0)
    parser.add_argument('-c', '--friction_coefficient', type=float, help='Friction coefficient (N/(m/s))', default=0.5)
    parser.add_argument('-k', '--spring_coefficient', type=float, help='Spring coefficient (N/m)', default=10.0)
    parser.add_argument('-x0', '--initial_position', type=float, help='Initial position (m)', default=0.0)
    parser.add_argument('-v0', '--initial_velocity', type=float, help='Initial velocity (m/s)', default=0.0)
    parser.add_argument('-ts', '--sampling_time', type=float, help='Sampling time in s', default=0.01)
    parser.add_argument('-os', '--output_spaces', type=int, help='Number of spaces to format the output', default=12)
    parser.add_argument('-wn', '--closed_loop_freq', type=float, help='Desired closed loop natural frequency (rad/s)', default=2.0)
    parser.add_argument('-z', '--dumping_ratio', type=float, help='Desired closed loop dumping ratio', default=0.7)
    parser.add_argument('-a', '--pole_3_mult', type=float, help='Closed loop extra pole location factor', default=5.0)

    args = parser.parse_args()

    log_criticalities = [logging.CRITICAL, logging.ERROR, logging.WARNING, logging.INFO, logging.DEBUG]
    log_criticality = log_criticalities[log_options.index(args.log_level)]
    # Add handlers to the logger
    root_logger = logging.getLogger()
    root_logger.addHandler(console_handler)
    root_logger.setLevel(log_criticality)
    console_handler.setLevel(log_criticality)

    ret = mass_spring_damper_state_space_numpy(m=args.mass, c=args.friction_coefficient, k=args.spring_coefficient, x0=args.initial_position, v0=args.initial_velocity, Ts=args.sampling_time)

    A_mat = ret['Ad']
    B_mat = ret['Bd']
    C_mat = ret['Cd']
    D_mat = ret['Dd']
    state_matrix_str = '{{' + str(A_mat[0][0]) + ' ' + str(A_mat[0][1]) + '} {' + str(A_mat[1][0]) + ' ' + str(A_mat[1][1]) + '}}'
    input_matrix_str = '{{' + str(B_mat[0][0]) + '} {' + str(B_mat[1][0]) + '}}'
    output_matrix_str = '{{' + str(C_mat[0][0]) + ' ' + str(C_mat[0][1]) + '} {' + str(C_mat[1][0]) + ' ' + str(C_mat[1][1]) + '}}'
    feedthrough_matrix_str = '{{' + str(D_mat[0][0]) + '} {' + str(D_mat[1][0]) + '}}'

    print('SSMGAM')
    print(f'{" " * args.output_spaces}StateMatrix = {state_matrix_str}')
    print(f'{" " * args.output_spaces}InputMatrix = {input_matrix_str}')
    print(f'{" " * args.output_spaces}OutputMatrix = {output_matrix_str}')
    print(f'{" " * args.output_spaces}FeedthroughMatrix= {feedthrough_matrix_str}')
 
    Kp, Ki, Kd = msd_pid_gains(args.mass, args.friction_coefficient, args.spring_coefficient, wn=args.closed_loop_freq, zeta=args.dumping_ratio, alpha=args.pole_3_mult)
    print('PIDGAM')
    print(f'{" " * args.output_spaces}Kp = {Kp}')
    print(f'{" " * args.output_spaces}Ki = {Ki}')
    print(f'{" " * args.output_spaces}Kd = {Kd}')
