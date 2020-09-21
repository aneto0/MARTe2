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
__author__ = 'Andre Neto'
__date__ = '24/07/2020'

##
# Notes
# - All the python ctags wrappers and llvm/clang had several limitations that I did not manage to address
#   Thus I have decided to call ctag from the command line and parse it myself
##

##
# Imports
##
import argparse
import datetime
import git
import glob
import logging
import os
import subprocess
import unidecode

# Create a custom logger
logger = logging.getLogger('{0}'.format(__name__))

# Create handlers
consoleHandler = logging.StreamHandler()

# Create formatters and add it to handlers
consoleHandlerFormat = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
consoleHandler.setFormatter(consoleHandlerFormat)

# Add handlers to the logger
logger.addHandler(consoleHandler)


def CheckLint(lintIgnoredFiles):
    #Get all .cpp
    cppFilesFullPath = [f for f in glob.glob('Source/**/*.cpp', recursive=True)]
    cppFiles = []
    for f in cppFilesFullPath:
        cppFiles.append(f.split('/')[-1])

    LINT_FILE_LOCATION = 'MakeDefaults/Lint/marte_flint_files.lnt'
    lintFileList = open(LINT_FILE_LOCATION).read()

    #Get all .h
    for f in cppFiles:
        logger.debug('Checking if file {0} is listed for linting'.format(f))
        if (f not in lintIgnoredFiles):
            if (f not in lintFileList):
                logger.critical('File {0} is not being linted!'.format(f))

def CheckFunctionalTest(f, ignorePatterns):
    TAGS_FILE_LOCATION = '/tmp/tags'
    #Get all the function names
    os.system('ctags -f {0} --c++-kinds=+p-m-n --fields=+afkst {1}'.format(TAGS_FILE_LOCATION, f))

    functionNames = []
    with open(TAGS_FILE_LOCATION) as tagLines:
        for tagLine in tagLines:
            fname = tagLine.split('\t')[0]
            faccess = tagLine.split('access:')
            if (len(faccess) > 1):
                if(faccess[1].startswith('public')):
                    functionNames.append(fname)
                    logger.debug('Discovered function {0}'.format(fname))

    filenameNoPath = f.split('/')[-1]
    filenameNoExt = filenameNoPath.split('.')[0]
    className = filenameNoExt
    logger.debug('Class name should be {0}'.format(filenameNoExt))
    levelName = f.split('/')[-2]
    logger.debug('Level should be {0}'.format(levelName))
    tierName = f.split('/')[-3]
    logger.debug('Tier name should be {0}'.format(tierName))
    if (tierName == 'Architecture' or tierName == 'Environment'):
        levelName = f.split('/')[-4]
        logger.debug('Level should be {0}'.format(levelName))
        tierName = f.split('/')[-5]
        logger.debug('Tier name should be {0}'.format(tierName))
    #For GTest
    gtestPath = 'Test/GTest/{0}/{1}/{2}GTest.cpp'.format(tierName, levelName, filenameNoExt)
    #print(t.pattern)
    logger.debug('Looking for test functions in {0}'.format(gtestPath))
    testFileExists = os.path.exists(gtestPath)
    testFunctionNames = []
    
    if (testFileExists):
        os.system('ctags -f {0} --c++-kinds=+p-m-n --fields=+afkst {1}'.format(TAGS_FILE_LOCATION, gtestPath))
        with open(TAGS_FILE_LOCATION) as tagLines:
            for tagLine in tagLines:
                testFunctionName = tagLine.split('TEST(')
                if (len(testFunctionName) > 1):
                    testFunctionName = testFunctionName[1].split(',')
                if (len(testFunctionName) > 1):
                    testFunctionName = testFunctionName[1].split(')')[0]
                    testFunctionNames.append(testFunctionName)
                    logger.debug('Discovered test function {0}'.format(testFunctionName))
    else:
        logger.critical('Test file {0} does not exist. No test functions will be discovered'.format(gtestPath))

    untestedFunctions = []
    for fname in functionNames:
        functionFound = False
        checkFunction = True
        if (fname == className):
            checkFunction = False
        else:
            for pat in ignorePatterns:
                if (pat in fname):
                    checkFunction = False
                    break
        if (checkFunction):
            for testFunctionName in testFunctionNames:
                if (fname in testFunctionName):
                    functionFound = True
                    break
            if (not functionFound):
                logger.critical('Function {0} does not seem to be tested'.format(fname))
                untestedFunctions.append(fname)
            else:
                logger.debug('Function {0} seems to be tested'.format(fname))
    return (className, untestedFunctions)

def CheckFunctionalTests(files):
    functionalTestStatus = {}
    for f in files:
        if (f.startswith('Source') and f.endswith('.h')):
            logger.debug('Checking functional tests for file {0}'.format(f))
            untestedFunctions = CheckFunctionalTest(f, ['DLL_API', '~', 'operator'])
            functionalTestStatus[f] = {'class': untestedFunctions[0], 'untested': untestedFunctions[1]}

    return functionalTestStatus

def CheckHeaderMakefile(f):
    return {}

def CheckHeader(f):
    atFileOK = False
    atBriefOK = False
    atDateOK = False
    atAuthorOK = False
    atCopyrightOK = False

    filename = f.split('/')[-1]
    filenameSplit = filename.split('.')
    className = filenameSplit[0]
    extension = '.h'
    if (len(filenameSplit) > 1):
        extension = filenameSplit[1].rsplit()[0]
    isHeaderFile = (extension == 'h')

    with open(f) as lines:
        for line in lines:
            line = line.rstrip()
            if ('* @file ' in line):
                atFileFilename = line.split('@file ')
                if (len(atFileFilename) > 1):
                    atFileFilename = atFileFilename[1]
                else:
                    atFileFilename = ''
                logger.debug('Going to check if detected filename {0} is in the actual filename {1}'.format(atFileFilename, f))
                atFileOK = (atFileFilename in f)
                if (not(atFileOK)):
                    logger.critical('@file incorrect format detected: {0}'.format(line))
            elif ('* @brief ' in line):
                possibleBriefs = []
                expectedBrief = 'file for class {0}'.format(className)
                if (isHeaderFile):
                    expectedBrief = 'Header ' + expectedBrief
                    possibleBriefs.append(expectedBrief)
                else:
                    possibleBriefs.append('Class ' + expectedBrief)
                    possibleBriefs.append('Source ' + expectedBrief)
                for expectedBrief in possibleBriefs:
                    logger.debug('Expected brief: {0}'.format(expectedBrief))
                    atBriefOK = (expectedBrief in line)
                    if (not(atBriefOK)):
                        logger.debug('Unexpected brief found: {0}'.format(line))
                    else:
                        break
            elif ('* @author ' in line):
                author = line.split('@author ')[1]
                #Remove accents
                author = unidecode.unidecode(author)
                logger.debug('Author {0} found'.format(author))
                with open('AUTHORS') as authors:
                    for existingAuthor in authors:
                        existingAuthor = existingAuthor.rstrip()
                        existingAuthor = unidecode.unidecode(existingAuthor)
                        atAuthorOK = (author == existingAuthor)
                        if (atAuthorOK):
                            break
                if (not(atAuthorOK)):
                    logger.critical('Author {0} was not found in the AUTHORS list'.format(author))
            elif ('* @copyright ' in line):
                atCopyrightOK = True
            elif ('* @date ' in line):
                dateStr = line.split('@date ')[1]
                try:
                    datetime.datetime.strptime(dateStr, '%d/%m/%Y')
                    atDateOK = True
                except Exception as e:
                    logger.critical('Invalid date found: {0}'.format(e))
                    atDateOK = False
            elif ('*/' in line):
                break

    allOK = atFileOK and atBriefOK and atDateOK and atAuthorOK and atCopyrightOK
    status = {'allok': allOK, '@file': atFileOK, '@brief': atBriefOK, '@date': atDateOK, '@author': atAuthorOK, '@copyright': atCopyrightOK}
    return status

def CheckHeaders(files):
    headersStatus = {}
    for f in files:
        if (f.startswith('Makefile')):
            headersStatus[f] = CheckHeaderMakefile(f)
        elif (f.endswith('.cpp') or f.endswith('.h')):
            headersStatus[f] = CheckHeader(f)

    return headersStatus 

def GetChangedFiles(repo, branch):
    changedFiles = []
    index = repo.index
    diffs = index.diff(branch)
    for diff in diffs:
        fname = diff.a_rawpath.decode('ascii')
        changedFiles.append(fname)
        logger.debug('The file {0} changed w.r.t. to the branch {1}'.format(fname, branch))
    return changedFiles

def ParseLCovFile(lines):
    coverage = {}
    for line in lines:
        line = line.rstrip()
        results = line.split('|')
        if (len(results) > 2):
            filename = results[0].rstrip()
            if (filename.endswith('.h') or filename.endswith('.cpp')):
                lines = 0
                functions = 0
                #Some coverage results may be empty
                try:
                    lines = float(results[1].split('%')[0]) / 100
                except Exception as e:
                    pass
                try:
                    functions = float(results[2].split('%')[0]) / 100
                except Exception as e:
                    pass
                coverage[filename] = {'lines': lines, 'functions': functions}

    return coverage

#Allow to keep enviroment variables and wait for the command to complete while printing to the screen
def ExecShellCommand(command, logOutput = True, logErrors = True):
    logger.info('Calling command {0}'.format(command))
    if not isinstance(command, list):
        command = [command]
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True, shell=True)
    if (logOutput):
        for outLine in iter(process.stdout.readline, ''):
            logger.debug(outLine)
    if (logErrors):
        for errLine in iter(process.stderr.readline, ''):
            logger.critical(errLine)
    process.wait()
    return process

def ExecLCov(clean, lcovBuildDir, lcovBuildPrefix, gtestExec, gtestFilters):
    #Build with coverage enabled
    if (clean):
        ExecShellCommand('make -f Makefile.cov clean')

    ExecShellCommand('make -f Makefile.cov')
   
    #Run baseline coverage
    ExecShellCommand('lcov --capture --initial --directory . --no-external --output-file {0}/{1}.initial'.format(lcovBuildDir, lcovBuildPrefix))
    
    #Execute the tests
    for f in gtestFilters:
        ExecShellCommand('{0} --gtest_filter={1}'.format(gtestExec, f))

    #Create test coverage data file
    ExecShellCommand('lcov --capture --directory . --no-external --output-file {0}/{1}.tests'.format(lcovBuildDir, lcovBuildPrefix))

    #Combine baseline and test coverage data
    ExecShellCommand('lcov --add-tracefile {0}/{1}.initial --add-tracefile {0}/{1}.tests --output-file {0}/{1}.1'.format(lcovBuildDir, lcovBuildPrefix))

    #Remove false positives
    ExecShellCommand('lcov --remove {0}/{1}.1 "/Test*" --output-file {0}/{1}.2'.format(lcovBuildDir, lcovBuildPrefix))
    ExecShellCommand('lcov --remove {0}/{1}.2 "*gtest*" --output-file {0}/{1}'.format(lcovBuildDir, lcovBuildPrefix))

def GetLCovOutput(lcovBuildDir, lcovBuildPrefix):
    process = ExecShellCommand('lcov -l {0}/{1}'.format(lcovBuildDir,lcovBuildPrefix), False, False)
    errLines = process.stderr.readlines()
    ok = True
    for line in errLines:
        if ('ERROR' in line):
            logger.critical('Error executing lcov: {0}'.format(line))
            ok = False

    lcovLines = []
    if (ok):
        lcovLines = process.stdout.readlines()
        
    return lcovLines

def ChangeBranch(repo, targetBranch):
    ok = True
    currentBranch = repo.active_branch.name
    try:
        logger.debug('Checking out to branch {0} from branch {1}'.format(targetBranch, currentBranch))
        repo.git.checkout(targetBranch)
    except Exception as e:
        ok = False
        logger.critical('Failed checkout to branch {0} from branch {1}'.format(targetBranch, currentBranch))

    return ok

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Script to support the QA activities')
    logOptions = ['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG']
    parser.add_argument('-l', '--loglevel', type=str, help='Log level, one of: CRITICAL, ERROR, WARNING, INFO, DEBUG', default='INFO', choices=logOptions)
    parser.add_argument('-b', '--branch', type=str, help='Target branch against which the changes should be validated', default='develop')
    parser.add_argument('-df', '--dfile', type=str, help='Validate a specific file (mostly for debug)', default='')
    parser.add_argument('-af', '--allfiles', action='store_true', help='Validates all source files (mostly for debug)', default='')
    parser.add_argument('-lg', '--gtestlcov', type=str, help='Lcov GTest executor location', default='Test/GTest/cov/MainGTest.ex')
    parser.add_argument('-lf', '--gtestlcovfilter', type=str, help='Lcov GTest filters', nargs='*', default=['*'])
    parser.add_argument('-ld', '--gtestlcovdir', type=str, help='Lcov GTest build directory', default='Build')
    parser.add_argument('-lp', '--gtestlcovprefix', type=str, help='Lcov GTest build file prefix', default='MARTe2.coverage.info')

    args = parser.parse_args()
    
    logCriticalities = [logging.CRITICAL, logging.ERROR, logging.WARNING, logging.INFO, logging.DEBUG]
    logCriticality = logCriticalities[logOptions.index(args.loglevel)]
    logger.setLevel(logCriticality)
    consoleHandler.setLevel(logCriticality)

    repo = git.Repo('.')
#    if (len(args.dfile) != 0):
#        changedFiles = [args.dfile]
#    elif (args.allfiles):
#        changedFiles = [f for f in glob.glob('Source/**/*.h', recursive=True)]
#    else:
#        changedFiles = GetChangedFiles(repo, args.branch)
#    CheckLint(['MARTeApp.cpp', 'Bootstrap.cpp'])
#   
#    functionalTestStatus = CheckFunctionalTests(changedFiles)
#    for ft in functionalTestStatus:
#        if (len(functionalTestStatus[ft]['untested']) > 0):
#            logger.info('Class: {0} Untested functions: {1}'.format(functionalTestStatus[ft]['class'], functionalTestStatus[ft]['untested']))
#
#    headersStatus = CheckHeaders(changedFiles)
#    for hs in headersStatus:
#        logger.debug('File: {0} header status: {1}'.format(hs, headersStatus[hs]))
#        for status in headersStatus[hs]:
#            if (status != 'allok'):
#                if (not headersStatus[hs][status]):
#                    logger.critical('Invalid {0} in file {1}'.format(status, hs))

    #Execute LCov in current branch
    #ExecLCov(True, args.gtestlcovdir, args.gtestlcovprefix, args.gtestlcov, args.gtestlcovfilter)
    lcovLines = GetLCovOutput(args.gtestlcovdir, args.gtestlcovprefix)
    if (len(lcovLines) == 0):
        logger.critical('Failed to read coverage file')
    else:
        covResultsCurrentBranch = ParseLCovFile(lcovLines)
        logger.debug(covResultsCurrentBranch)

    #Execute LCov in target branch
    currentBranch = repo.active_branch.name
    lcovPrefixTargetBranch = '{0}_to_compare'.format(args.gtestlcovprefix)
    ok = ChangeBranch(repo, args.branch)
    if (ok):
        ExecLCov(True, args.gtestlcovdir, lcovPrefixTargetBranch, args.gtestlcov, args.gtestlcovfilter)
        lcovLines = GetLCovOutput(args.gtestlcovdir, lcovPrefixTargetBranch)
        if (len(lcovLines) == 0):
            logger.critical('Failed to read coverage file')
        else:
            covResultsCurrentBranch = ParseLCovFile(lcovLines)
            logger.debug(covResultsCurrentBranch)
    else:
        logger.critical('Failed to change branch')
    if (ok):
        ok = ChangeBranch(repo, currentBranch)

