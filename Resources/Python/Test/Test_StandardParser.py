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
from StandardParser import StandardParser

class TestStandardParser(unittest.TestCase):

    def setUp(self):
        self.parser = StandardParser()

    def tearDown(self):
        pass

    def test_scalar(self):
        cfg = '  A       =         1   '
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'] == '1' )
        cfg = '  B       =         "C D "  '
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['B'] == "C D ")
        cfg = ' +_1A:34FB       =         "  ABCDED 923 !#@#"'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['+_1A:34FB'] == '  ABCDED 923 !#@#')
        cfg = ' $A="ABCDED"'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['$A'] == 'ABCDED')
        cfg = ' $A=A1234'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['$A'] == 'A1234')
        cfg = ' $A=12A34Z'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['$A'] == '12A34Z')
        cfg = ' $A=0xFF'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['$A'] == '0xFF')

    def test_scalar_malformed(self):
        cfg = '  B       ='
        #ret = self.parser.parse_string(cfg)
        #self.assertTrue(len(ret) == 0)

    def test_multiline_string(self):
        cfg = '''Expression = "TCR_VSR_Sync_I_ElapsedReadTime = TCR_VSR_Sync_I_ReadTime;
                 TCR_VSR_Sync_I_ElapsedExecTime = TCR_VSR_Sync_I_ExecTime - TCR_VSR_Sync_I_ReadTime;"'''
        ret = self.parser.parse_string(cfg)
        mstr = ret['Expression']
        self.assertTrue('TCR_VSR_Sync_I_ReadTime;\n' in mstr)
        self.assertTrue('TCR_VSR_Sync_I_ElapsedExecTime' in mstr)

    def test_nodes(self):
        cfg= '''/*A comment*/
                //Another comment
                A = {
                Class = "ReferenceContainer"
                +1A:34F = {
                    ABCDED = 3.4
                }
            }'''

        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A']['+1A:34F']['ABCDED'] == '3.4')

    def test_arrays(self):
        cfg = 'A = {"A" "B" 3}'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'][0] == 'A')
        self.assertTrue(ret['A'][1] == 'B')
        self.assertTrue(ret['A'][2] == '3')

        cfg = 'A = {"A", "B", 3,"4"}'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'][0] == 'A')
        self.assertTrue(ret['A'][1] == 'B')
        self.assertTrue(ret['A'][2] == '3')
        self.assertTrue(ret['A'][3] == '4')
    
        """
        cfg = 'A = {{2} {3 -3} {"A" "B" 3} {0}}'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'][0][0] == '2')
        self.assertTrue(ret['A'][1][0] == '3')
        self.assertTrue(ret['A'][1][1] == '-3')
        self.assertTrue(ret['A'][2][0] == 'A')
        self.assertTrue(ret['A'][2][1] == 'B')
        self.assertTrue(ret['A'][2][2] == '3')
        self.assertTrue(ret['A'][3][0] == '0')
        """

        cfg = 'A = {3, -3}'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'][0] == '3')
        self.assertTrue(ret['A'][1] == '-3')

        cfg = 'A = {{2}, {3, -3}, {"A", "B", 3}, {0}}'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'][0][0] == '2')
        self.assertTrue(ret['A'][1][0] == '3')
        self.assertTrue(ret['A'][1][1] == '-3')
        self.assertTrue(ret['A'][2][0] == 'A')
        self.assertTrue(ret['A'][2][1] == 'B')
        self.assertTrue(ret['A'][2][2] == '3')
        self.assertTrue(ret['A'][3][0] == '0')

        cfg = 'A = {{{1 2} /*Ignore me*/ {3 4}} {{5} {"A" "B"} {"C"}}}//Ignore me!'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'][0][0][0] == '1')
        self.assertTrue(ret['A'][0][0][1] == '2')
        self.assertTrue(ret['A'][0][1][0] == '3')
        self.assertTrue(ret['A'][0][1][1] == '4')
        self.assertTrue(ret['A'][1][0][0] == '5')
        self.assertTrue(ret['A'][1][1][0] == 'A')
        self.assertTrue(ret['A'][1][1][1] == 'B')
        self.assertTrue(ret['A'][1][2][0] == 'C')

        cfg = 'A = {{{1, 2}, /*Ignore me*/ {3, 4}}, {{5}, {"A", "B"}, {"C"}}}//Ignore me!'
        ret = self.parser.parse_string(cfg)
        self.assertTrue(ret['A'][0][0][0] == '1')
        self.assertTrue(ret['A'][0][0][1] == '2')
        self.assertTrue(ret['A'][0][1][0] == '3')
        self.assertTrue(ret['A'][0][1][1] == '4')
        self.assertTrue(ret['A'][1][0][0] == '5')
        self.assertTrue(ret['A'][1][1][0] == 'A')
        self.assertTrue(ret['A'][1][1][1] == 'B')
        self.assertTrue(ret['A'][1][2][0] == 'C')


    def test_cfg_file(self):
        ret = {}
        with open('test_file.cfg') as f:
            ret = self.parser.parse_file(f)
        self.assertTrue('PlasmaCurrentsS1_ReadTime;\n' in ret['$PlasmaCurrent2kHz']['+Functions']['+ElapsedTimes']['Expression'])

if __name__ == "__main__":
    unittest.main()

