#!/usr/bin/env python3
__copyright__ = '''
    Copyright 2020 F4E | European Joint Undertaking for ITER and
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
__author__ = 'Giuseppe Avon'
__date__ = '09/07/2021'

##
# Notes
##

##
# Constants
##
BEGINDELIMITER = "---TESTRESULTBEGIN---"
STATSDELIMITER = "---STATSDELIMITER---"
ENDDELIMITER = "---TESTRESULTEND---"

STATEMACHINE_WAITINGSTART = 0
STATEMACHINE_WAITINGSTATS = 1
STATEMACHINE_WAITINGEND = 2

##
# Imports
##
from collections import defaultdict
import getopt
import sys
import xml.etree.cElementTree as ET

##
# Class to model a single Test Result entity
##
class TestResult:
    isValid = False
    suiteName = ''
    caseName = ''
    caseResult = False
    caseNumber = 0
    caseTotal = 0

    @staticmethod
    def fromLine(line):
        cls = TestResult()
        validProgress = False
        validNames = False
        validResult = False

        tempData = line.split(" ")
        if(len(tempData) == 3):
            tempNumTotal = tempData[0][tempData[0].find("[") + 1 : tempData[0].find("]")]
            tempA = tempNumTotal.split("/")
            if(len(tempA) == 2):
                cls.caseNumber = tempA[0]
                cls.caseTotal = tempA[1]
                validProgress = True

            tempB = tempData[1].split(".")
            if(len(tempB) == 2):
                cls.suiteName = tempB[0][1:len(tempB[0]) - 1]
                cls.caseName = tempB[1][1:len(tempB[1]) - 1]
                validNames = True

            tempC = tempData[2]
            if tempC[0:4] == "PASS":
                cls.caseResult = True
                validResult = True
            elif tempC[0:4] == "FAIL":
                cls.caseResult = False
                validResult = True
            else:
                validResult = False

        cls.isValid = validProgress and validNames and validResult

        return cls
    
    def __str__(self):
        return self.suiteName + " " + self.caseName

##
# Class to model the entire parser
##
class PortableTestResultsParser:
    
    parserState = STATEMACHINE_WAITINGSTART
    isContentComplete = False
    resultsList = defaultdict(list)

    def __init__(self):
        self.parserState = STATEMACHINE_WAITINGSTART
        self.isContentComplete = False

    def pushLine(self, line):
        if self.parserState == STATEMACHINE_WAITINGSTART:
            if line[0:len(BEGINDELIMITER)] == BEGINDELIMITER:
                self.parserState = STATEMACHINE_WAITINGSTATS
        elif self.parserState == STATEMACHINE_WAITINGSTATS:
            if line[0:len(STATSDELIMITER)] == STATSDELIMITER:
                self.parserState = STATEMACHINE_WAITINGEND
            else:
                result = TestResult.fromLine(line)
                if result.isValid:
                    self.resultsList[result.suiteName].append(result)
                else:
                    print('Found unparsable line "' + line + '"') 
        elif self.parserState == STATEMACHINE_WAITINGEND:
            if line[0:len(ENDDELIMITER)] == ENDDELIMITER:
                self.isContentComplete = True
                self.parserState = STATEMACHINE_WAITINGSTART

def usage():
    print('Results parser usage')
    print('PortableTestResultsParser.py -i <inputfile> -o <outputfile>')
    print('Or, alternatively')
    print('PortableTestResultsParser.py --inputfile <inputfile> --outputfile <outputfile>')

def toJUnitXML(inputFile, outputFile):
    with open(inputFile, 'r') as fp:
        line = fp.readline()
        lineCnt = 1
        parser = PortableTestResultsParser()

        while line:
            parser.pushLine(line)
            line = fp.readline()
            lineCnt += 1

        suitesTests = 0
        suitesFailures = 0

        suiteFailures = 0
        suitesFailures = 0
        
        if parser.isContentComplete:
            root = ET.Element("testsuites", tests=str(suitesTests), failures=str(suitesFailures), disabled="0", errors="0", timestamp="0", time="0", name="AllTests")
            suiteTests = 0
            suiteFailures = 0
            for key,value in parser.resultsList.iteritems():
                suite = ET.SubElement(root, "testsuite", name=key, tests=str(suiteTests), failures=str(suiteFailures), disabled="0", errors="0", time="0")
                for i in range(len(parser.resultsList[key])):
                    suiteTests = suiteTests + 1
                    suitesTests = suitesTests + 1
                    testcase = ET.SubElement(suite, "testcase", name=value[i].caseName, classname=key, status="run", time="0")
                    if not value[i].caseResult:
                        suiteFailures = suiteFailures + 1
                        suitesFailures = suitesFailures + 1
                        failureMessage = "Failure in method call " + value[i].caseName
                        failure = ET.SubElement(testcase, "failure", message=str(failureMessage))
                        failure.text = str(failureMessage)
                    print(parser.resultsList[key][i])
                suite.attrib['tests'] = str(suiteTests)
                suite.attrib['failures'] = str(suiteFailures)
                root.attrib['tests'] = str(suitesTests)
                root.attrib['failures'] = str(suitesFailures)
            tree = ET.ElementTree(root)
            tree.write(outputFile, encoding='utf8', method='xml')

##
# Entrypoint
##
def main(argv):
    inputFile = ''
    outputFile = ''

    try:
        opts, args = getopt.getopt(argv, "hi:o:", ["help", "inputfile=", "outputfile="])
    except getopt.GetoptError as err:
        print (err)
        usage()
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"): 
            usage()
            sys.exit()
        elif opt in ("-i", "--inputfile"):
            inputFile = arg
        elif opt in ("-o", "--outputfile"):
            outputFile = arg        

    if not inputFile or not outputFile:
        usage()
    else:
        toJUnitXML(inputFile, outputFile)

if __name__ == "__main__":
    main(sys.argv[1:])
