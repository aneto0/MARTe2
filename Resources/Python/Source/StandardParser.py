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

class StandardParser(object):
    """MARTe2 configuration python StandardParser implementation.
    See https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2/-/blob/master/Source/Core/BareMetal/L4Configuration/StandardParser.h
    All values are converted to string to avoid issues with unquoted strings vs hexadecimal and other special numbers."""

    def __init__(self):
        LBRACE, RBRACE, EQUAL = map(Suppress, '{}=')
        field = Word(printables, excludeChars='={}')
        field.addParseAction(tokenMap(str.rstrip), tokenMap(str.lstrip)) #Remove any spaces from the field

        string = QuotedString(quoteChar='"', multiline=True)
        string_no_quotes = Word(printables, excludeChars='{}=/*,')

        #The code below was givign issues when comparing hex numbers against unquoted strings.
        #number = pyparsing_common.number()
        #hex_number = pyparsing_common.hex_integer()
        #scalar_value = (number | hex_number | string)
        scalar_value = (string | string_no_quotes)

        arr_list = delimitedList(scalar_value)
        arr_value = Group(LBRACE + arr_list + RBRACE)

        arr_list_spaces = OneOrMore(scalar_value)
        arr_value_spaces = Group(LBRACE + arr_list_spaces + RBRACE)

        mat_value = Group(LBRACE + delimitedList(arr_value) + RBRACE)
        mat_value_spaces = Group(LBRACE + OneOrMore(arr_value_spaces) + RBRACE)

        cub_value = Group(LBRACE + delimitedList(mat_value) + RBRACE)
        cub_value_spaces = Group(LBRACE + OneOrMore(mat_value_spaces) + RBRACE)
        variable_value = scalar_value | arr_value | mat_value | cub_value | arr_value_spaces | mat_value_spaces | cub_value_spaces

        node = Forward()

        member_field_eq_var = Group(field + EQUAL + variable_value)
        member_field_eq_node = Group(field + EQUAL + LBRACE + node + RBRACE)
        members_def = member_field_eq_var | member_field_eq_node 

        node <<= Dict(OneOrMore(members_def))

        node.setParseAction(lambda t: t)

        self.parser = OneOrMore(Group(Dict(members_def))).ignore(cStyleComment).ignore(dblSlashComment)

    def parse_string(self, cfg):
        ret = {}
        try:
            #ret_dicts = self.parser.parseString(cfg, parseAll=True) #parseAll=True was important when trying to detect invalid numbers
            ret_dicts = self.parser.parseString(cfg)
            for ret_dict in ret_dicts:
                ret.update(ret_dict.asDict())
        except Exception as e:
            logger.critical(f'Failed to parse cfg: {e}')
        return ret

    def parse_file(self, cfg):
        ret = {}
        try:
            ret_dicts = self.parser.parseFile(cfg)
            for ret_dict in ret_dicts:
                ret.update(ret_dict.asDict())
        except Exception as e:
            logger.critical(f'Failed to parse cfg: {e}')
        return ret
