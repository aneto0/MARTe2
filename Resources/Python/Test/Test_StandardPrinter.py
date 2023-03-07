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
__date__ = '22/01/2023'

##
# imports
##
import unittest
from StandardPrinter import StandardPrinter
from StandardParser import StandardParser

class TestStandardPrinter(unittest.TestCase):

    def setUp(self):
        self.printer = StandardPrinter()
        self.parser = StandardParser()

    def tearDown(self):
        pass

    def test_scalar(self):
        d = {}
        d['A'] = 1
        d['+A#$3'] = ' 32@#'
        o_str = self.printer.print(d)
        ret = self.parser.parse_string(o_str)
        self.assertTrue(ret['A'] == '1')
        self.assertTrue(ret['+A#$3'] == ' 32@#')

    def test_list(self):
        d = {}
        d['A'] = [1, 2, 3]
        d['B'] = [[1,2], [0]]
        o_str = self.printer.print(d)
        ret = self.parser.parse_string(o_str)
        self.assertTrue(ret['A'][0] == '1')
        self.assertTrue(ret['A'][1] == '2')
        self.assertTrue(ret['A'][2] == '3')
        self.assertTrue(ret['B'][0][0] == '1')
        self.assertTrue(ret['B'][0][1] == '2')

    def test_dict(self):
        d = {}
        d['A'] = {}
        d['A']['C'] = [[1,2,3], [3,4]]
        d['A']['D'] = {}
        d['A']['D']['E'] = 5
        d['A']['B'] = 4
        o_str = self.printer.print(d)
        e_str =  'A = {\n'
        e_str += '  C = {{"1" "2" "3"}{"3" "4"}}\n'
        e_str += '  D = {\n'
        e_str += '    E = "5"\n'
        e_str += '  }\n'
        e_str += '  B = "4"\n'
        e_str += '}\n'
        self.assertTrue(o_str == e_str)
        o_str = self.printer.print(d, ident = 4)
        e_str =  'A = {\n'
        e_str += '    C = {{"1" "2" "3"}{"3" "4"}}\n'
        e_str += '    D = {\n'
        e_str += '        E = "5"\n'
        e_str += '    }\n'
        e_str += '    B = "4"\n'
        e_str += '}\n'
        self.assertTrue(o_str == e_str)

    def test_cfg_file(self):
        ret = {}
        with open('test_file.cfg') as f:
            ret = self.parser.parse_file(f)
        o_str = self.printer.print(ret)
        with open('/tmp/out.cfg', 'w') as f:
            f.write(o_str)
        ret = self.parser.parse_string(o_str)
        self.assertTrue('PlasmaCurrentsS1_ReadTime;\n' in ret['$PlasmaCurrent2kHz']['+Functions']['+ElapsedTimes']['Expression'])

if __name__ == "__main__":
    unittest.main()

