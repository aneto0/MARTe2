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
__date__ = '07/01/2023'

##
# imports
##
from pyparsing import *
import logging

logger = logging.getLogger(__name__)

class StandardPrinter(object):
    """Converts a python element dict into a MARTe2 configuration string.
    See https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2/-/blob/master/Source/Core/BareMetal/L3Streams/StandardPrinter.h
    All values are cast to string."""

    def __init__(self):
        pass

    def print(self, cfg, ident = 2, level = 0):
        ret = ''
        for k in cfg:
            ret += ' ' * (ident * level)
            val = cfg[k]
            if (isinstance(val, dict)):
                ret += f'{k} = '
                ret += '{\n'
                ret += self.print(val, ident, level + 1)
                ret += ' ' * (ident * level)
                ret += '}\n'
            elif (isinstance(val, list)):
                ret += f'{k} = '
                ret += '{'
                ret += self._print_list(val)
                ret += '}\n'
            else:
                ret += self._print_scalar(k, val)
                ret += '\n'
        return ret

    def _print_list(self, l):
        ret = ''
        for i, v in enumerate(l):
            if (isinstance(v, list)):
                ret += '{'
                ret += self._print_list(v)
                ret += '}'
            else:
                if (i != 0):
                    ret += ' '
                ret += f'\"{v}\"'
        return ret

    def _print_scalar(self, k, v):
        return f'{k} = \"{v}\"'

