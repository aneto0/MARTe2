.. date: 24/03/2018
   author: Andre' Neto
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.


New parsers
===========

SLK Parser
----------

The MARTe parsers are developed using the `slk tool <http://www.slkpg.org/download.html>`_. In order to add a new language the steps described below must be followed.

Lexical Analyzer
----------------

The MARTe2 lexical analyser recognizes five type of tokens:

- STRING: any token beginning with a non-number character or enclosed by double quotes.
- NUMBER: any token which could represent a number.
- TERMINAL: one of the terminal characters in the terminals list provided by the user.
- EOF: end of the stream.
- ERROR: none of the previous ones.

The analyser allows to configure the separator and terminal characters and the comment patterns (which depend on the language to be parsed). 
For more informations about the MARTe2 lexical analyser see the documentation of the :vcisdoxygencl:`LexicalAnalyzer` class.

Grammar file
------------

The file containing the parser grammar must be provided by the user as an input for the slk tool. 
It is a file with the extension ``*.ll`` and must be written using the slk language and respecting the token detailed above in order to use the MARTe2 lexical analyser. 

The available MARTe supported grammars are available `here <https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2/tree/master/Docs/Assets/Snippets/Parsing>`_.

Adding a new grammar
--------------------

The MARTe2 class :vcisdoxygencl:`ParserI` provides the implementation of all the required callbacks that are required to construct a new :vcisdoxygencl:`StructuredDataI` from a given configuration file. 

A parser for a new configuration language can be implemented following these steps:

1. Write the parser grammar file (grammar_file_name.ll) using the same token types defined in the MARTe2 :vcisdoxygencl:`LexicalAnalyzer` class and the callbacks implemented in the :vcisdoxygencl:`ParserI` class.
2. Create your parser class copying from an existent parser (:vcisdoxygencl:`StandardParser`, :vcisdoxygencl:`XMLParser`, :vcisdoxygencl:`JSonParser`).
3. Type on the console ``slk -C++ grammar_file_name.ll`` to generate the parser files.
4. Open the generated file ``SlkParse.cpp`` and copy the arrays at the beginning, in the source file of your parser class (the array named ``Parse[]`` should be renamed to ``ParseArray[]`` to avoid conflicts with the function ``ParserI::Parse()``.
5. Copy the constants defined in ``SlkParse.cpp`` (just after the arrays) in the ``Constants[]`` array maintaining the same order: ``{ START_SYMBOL, END_OF_SLK_INPUT_, START_STATE, START_CONFLICT,END_CONFLICT, START_ACTION, END_ACTION, TOTAL_CONFLICTS }``
6. Open the generated file ``SlkString.cpp`` and copy the array ``Terminal_name[]`` in the source file of your parser class.
7. Open the generated file ``SlkTable.cpp`` and copy the content of the function ``initialize_table()`` inside your parser constructor changing the namespace ``SlkAction`` with the name of your parser class.
8. Make a ParserGrammar constant structure with the separators, terminals and comment patterns of the language to be parsed and use it to initialize the parent class :vcisdoxygencl:`ParserI` in your parser constructor.
9. All the other functions can be copied without any modification from any other existent parser class.
