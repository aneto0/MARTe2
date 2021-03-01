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
__date__ = '21/09/2020'

##
# Imports
##
from abc import ABCMeta, abstractmethod
from lxml import etree

import datetime
import glob
import qautils
import os
import socket
import unidecode

##
# Class definitions
##
class QAHelper(object):
    """ Abstract class for all QA helpers implementation.
    """

    __metaclass__ = ABCMeta

    def __init__(self, logger):
        """ Sets the logger.
        """
        self.logger = logger

    def Configure(self, args):
        """ Configures the helper instance.
        
        Args:
            args (dict): module specific parameters.
        Returns:
            True if all the parameters are successfully loaded.
        """
        pass

    def Run(self, reporter):
        """ Runs the helper instance and outputs the results to the provided reporter
        
        Args:
            reporter (QAReporter): where to print the output results.
        Returns:
            True if the module executed with no errors. 
        """
        pass

    def ExecShellCommand(self, command, logOutput = True, logErrors = True):
        """ Allows to keep enviroment variables and wait for the command to complete while printing to the screen
        """
        #See qautils
        return qautils.ExecShellCommand(self.logger, command, logOutput, logErrors)

    def ChangeBranch(self, repo, targetBranch):
        """ Change git branch 
        """
        #See qautils
        return qautils.ChangeBranch(self.logger, repo, targetBranch)

class LintIncludeHelper(QAHelper):
    """ Helper QA class that verifies if all the files are being included for linting.
    """

    def Configure(self, args):
        """ Configures the lint helper instance.
        
        Args:
            args (dict): args['sourcedir'] (str) the source directory.
                         args['lintincludes'] ([str]) a list with all the lint include files.
                         args['lintignore'] ([str]) a list with all the files that are not expected to be linted.
        Returns:
            True.
        """
        self.sourceDirectory = args['sourcedir']
        self.lintIncludeFiles = args['lintincludes']
        self.lintIgnoredFiles = args['lintignore']

        return True

    def Run(self, reporter):
        """ Checks that all the cpp files are being included for linting.
        Args:
            args (QAReporter): where to print the output results.
        Returns:
            True if all the cpp files are being included for linting. 
        """
        #Get all .cpp
        cppFilesFullPath = [f for f in glob.glob('{0}/**/*.cpp'.format(self.sourceDirectory), recursive=True)]
        cppFiles = []
        for f in cppFilesFullPath:
            cppFiles.append(f.split('/')[-1])

        ok = True
        lintIncludeFilesList = []
        for lintIncludeFile in self.lintIncludeFiles:
            #Append all the lint definition files to a list
            lintIncludeFilesList.append(open(lintIncludeFile).read())
        #Get all .cpp
        for f in cppFiles:
            self.logger.debug('Checking if file {0} is listed for linting'.format(f))
            found = False
            if (f not in self.lintIgnoredFiles):
                    
                #Check if the file is one of the lint include files
                for lintIncludeFileList in lintIncludeFilesList:
                    found = (f in lintIncludeFileList)
                    if (found):
                        break
                if (not found):
                    self.logger.critical('File {0} is not being linted!'.format(f))
                    reporter.WriteError('File {0} is not being linted!'.format(f))
                    ok = False
        if (ok):
            reporter.WriteOK('All files are being included for linting')
        else:
            reporter.WriteError('Not all files are being included for linting')
        return ok

class LinterExecutorHelper(QAHelper):
    """ Helper QA class that triggers the remote execution of the linter (using rsync + ssh).
    """

    def Configure(self, args):
        """ Configures the lint helper instance.
        
        Args:
            args (dict): args['linterexec'] (str) the name of the Linter executable.
                         args['linterhostname'] (str) the hostname of the computer where the linter is installed.
                         args['linterdir'] (str) directory where to trigger the linter from.
                         args['linterrsyncsource'] ([str]) list of source directories to be rsynced.
                         args['linterrsynctarget'] ([str]) list of target directories to be rsynced.
                         args['linterrsyncexclude'] ([str]) list of directories not to be rsynced.
                         args['linterinclude'] ([str]) list of directories to be included in the lint call.
                         args['linterusername'] (str) username to connect to the remote computer.
                         args['linterfilename'] (str) linter input filename.
                         args['lintergrep'] (str) negative grep to apply to output results.
                         args['linteroutputfilename'] (str) linter output filename.
        Returns:
            True.
        """
        self.linterExec = args['linterexec']
        self.linterHostname = args['linterhostname']
        self.linterDir = args['linterdir']
        self.linterRSyncSource = args['linterrsyncsource']
        self.linterRSyncTarget = args['linterrsynctarget']
        self.linterRSyncExclude = args['linterrsyncexclude']
        self.linterInclude = args['linterinclude']
        self.linterUsername = args['linterusername']
        self.linterFilename = args['linterfilename']
        self.linterGrep = args['lintergrep']
        self.linterOutputFilename = args['linteroutputfilename']

        return True

    def Run(self, reporter):
        """ RSyncs using ssh to the remote PC where the lint program is installed and calls the linter.
        Args:
            args (QAReporter): where to print the output results.
        Returns:
            True if there are no errors output by the linter. 
        """
        ok = True
        #Try to connect to the remote host
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            sock.connect((self.linterHostname, 22))
        except Exception as e:
            ok = False
            self.logger.critical('Could not connect to: {0}. Reason: {1}'.format(self.linterHostname, e))
            reporter.WriteError('Could not connect to: {0}. Linter was not executed'.format(self.linterHostname))
        sock.close()


        if (ok):
            ok = len(self.linterRSyncSource) == len(self.linterRSyncTarget)
            if (not ok):
                self.logger.critical('linterrsyncsource and linterrsynctarget shall have the same size')

        if (ok):
            rsyncExcludes = ''
            for e in self.linterRSyncExclude:
                rsyncExcludes += ' --exclude \'{0}\''.format(e)
            #RSync
            for (s, t) in zip(self.linterRSyncSource, self.linterRSyncTarget):
                if (s != '.'):
                    if (not s.endswith('/')):
                        s += '/'
                while (t.endswith('/')):
                    t = t[0:len(t)-1]
                if (len(t) > 0):
                    rsyncCommand = 'rsync -avz {0} --delete -e ssh {1} {2}@{3}:{4}'.format(rsyncExcludes, s, self.linterUsername, self.linterHostname, t)
                    self.logger.debug('Calling rsync: {0}'.format(rsyncCommand))
                    self.ExecShellCommand('{0}'.format(rsyncCommand))
                else:
                    self.logger.critical('Invalid target')
        if (ok):
            linterIncludes = ''
            linterGreps = ''
            for s in self.linterInclude:
                linterIncludes += ' -i {0}'.format(s)
            for s in self.linterGrep:
                linterGreps += ' | grep -v {0}'.format(s)

            lintCommand = 'ssh {0}@{1} "cd {2} && {3} {4} -v {5} {6}"'.format(self.linterUsername, self.linterHostname, self.linterDir, self.linterExec, linterIncludes, self.linterFilename, linterGreps)
            self.logger.debug('Calling lint command: {0}'.format(lintCommand))
            process = self.ExecShellCommand(lintCommand, False, False)

            outLines = process.stdout.readlines()
            errLines = process.stderr.readlines()
            ignoreKeywords = ['FlexeLint for C/C++ (Unix) Vers. 9.00L, Copyright Gimpel Software 1985-2014', 'Warning 686: Option', '\'-elib(*)\' is suspicious', 'causing meaningless output', 'something is wrong with your Lint configuration', 'receiving a syntax error in a library file']  

            outFile = None
            if (len(self.linterOutputFilename) > 0):
                outFile = open(self.linterOutputFilename, 'w')
            #errorKeywords = ['warning:', 'error:']
            for line in errLines:
                if (outFile is not None):
                    outFile.write(line)
                ignore = False
                for k in ignoreKeywords:
                    if (not ignore):
                        ignore = k in line 
                if (not ignore):
                    ok = False
                    self.logger.debug(line)
                    reporter.WriteError(line)

            for line in outLines:
                if (outFile is not None):
                    outFile.write(line)
                ignore = False
                for k in ignoreKeywords:
                    if (not ignore):
                        ignore = k in line 
                if (not ignore):
                    ok = False
                    self.logger.debug(line)
                    reporter.WriteError(line)

        if (outFile is not None):
            outFile.close()
        if (ok):
            reporter.WriteOK('No linter errors found')
        return ok



class FunctionalTestsHelper(QAHelper):
    """ Helper QA class that verifies if all the public methods are being tested.
    """

    def Configure(self, args):
        """ Configures the lint helper instance.
        
        Args:
            args (dict): args['gtestfolder'] (str) the parent folder for the gtest files
                         args['sourcedir'] (str) the source directory where to look for files that have to be tested. 
                         args['files'] ([str]) the list of files to be verified (may or not include files from the source directory - TODO this may be redundant).
                         args['exceptions'] ([str]) the list of files that are not to be verified (i.e. files for which it is ok not to have a GTest).
        Returns:
            True.
        """
        self.gtestFolder = args['gtestfolder']
        self.sourceDirectory = args['sourcedir']
        self.files = args['files']
        self.exceptions = args['exceptions']

        return True

    def CheckFunctionalTest(self, f, ignorePatterns):
        """ Verifies that all the public methods of file f are being tested at least once.
        """
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
                        self.logger.debug('Discovered function {0}'.format(fname))

        filenameNoPath = f.split('/')[-1]
        filenameNoExt = filenameNoPath.split('.')[0]
        className = filenameNoExt
        self.logger.debug('Class name should be {0}'.format(filenameNoExt))
        levelName = f.split('/')[-2]
        self.logger.debug('Level should be {0}'.format(levelName))
        tierName = f.split('/')[-3]
        self.logger.debug('Tier name should be {0}'.format(tierName))
        if (tierName == 'Architecture' or tierName == 'Environment'):
            levelName = f.split('/')[-4]
            self.logger.debug('Level should be {0}'.format(levelName))
            tierName = f.split('/')[-5]
            self.logger.debug('Tier name should be {0}'.format(tierName))
        #For GTest
        gtestPath = '{0}/{1}/{2}/{3}GTest.cpp'.format(self.gtestFolder, tierName, levelName, filenameNoExt)
        #print(t.pattern)
        self.logger.debug('Looking for test functions in {0}'.format(gtestPath))
        testFileExists = os.path.exists(gtestPath)
        testFunctionNames = []
       
        untestedFile = ''
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
                        self.logger.debug('Discovered test function {0}'.format(testFunctionName))
        else:
            self.logger.critical('Test file {0} does not exist. No test functions will be discovered'.format(gtestPath))
            untestedFile = gtestPath

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
                    self.logger.critical('Function {0} does not seem to be tested'.format(fname))
                    if (fname not in untestedFunctions):
                        untestedFunctions.append(fname)
                else:
                    self.logger.debug('Function {0} seems to be tested'.format(fname))
        return (className, untestedFunctions, untestedFile)

    def Run(self, reporter):
        """ Checks that all public methods are being tested at least once. 
        Args:
            args (QAReporter): where to print the output results.
        Returns:
            True if for all the files all the public methods are being tested.
        """
        functionalTestStatus = {}
        ok = True
        for f in self.files:
            if (f.startswith(self.sourceDirectory) and f.endswith('.h')):
                filenameNoPath = f.split('/')[-1]
                self.logger.debug('Checking if {0} is in {1}'.format(filenameNoPath, self.exceptions))
                #Ignore files that are not to be checked
                if (filenameNoPath not in self.exceptions):
                    self.logger.debug('Checking functional tests for file {0}'.format(f))
                    ret = self.CheckFunctionalTest(f, ['DLL_API', '~', 'operator'])
                    className = ret[0]
                    untestedMethods = ret[1]
                    untestedFile = ret[2]
                    for untestedMethod in untestedMethods:
                        reporter.WriteError('{0}:{1} is not tested'.format(className, untestedMethod))
                        ok = False
                    if (len(untestedFile) > 0):
                        reporter.WriteError('Test file not found: {0}'.format(untestedFile))

        if (ok):
            reporter.WriteOK('All public methods are being tested at least once')
        return ok

class HeadersHelper(QAHelper):
    """ Helper QA class that verifies if all the headers are correctly formatted
    """

    def Configure(self, args):
        """ Configures the headers helper instance.
        
        Args:
            args (dict): args['authors'] (str) the location of the authors file.
                         args['files'] ([str]) the list of files to be verified (may or not include files from the source directory - TODO this may be redundant).
        Returns:
            True.
        """
        self.authors = args['authors']
        self.files = args['files']

        return True


    def CheckHeader(self, f):
        """ Verifies that the header @file, @brief, @author and @copyright are correctly set
        """
        atFileOK = False
        atBriefOK = False
        atDateOK = False
        atAuthorOK = False
        atCopyrightOK = False
        includesOK = True

        filename = f.split('/')[-1]
        filenameSplit = filename.split('.')
        className = filenameSplit[0]
        extension = '.h'
        if (len(filenameSplit) > 1):
            extension = filenameSplit[1].rsplit()[0]
        isHeaderFile = (extension == 'h')
        includes = []
        endOfHeaderCommentReached = False
        with open(f) as lines:
            for line in lines:
                line = line.rstrip()
                if ('* @file ' in line):
                    atFileFilename = line.split('@file ')
                    if (len(atFileFilename) > 1):
                        atFileFilename = atFileFilename[1]
                    else:
                        atFileFilename = ''
                    self.logger.debug('Going to check if detected filename {0} is in the actual filename {1}'.format(atFileFilename, f))
                    atFileOK = (atFileFilename in f)
                    if (not(atFileOK)):
                        self.logger.critical('@file incorrect format detected: {0}'.format(line))
                elif ('* @brief ' in line):
                    if (not endOfHeaderCommentReached):
                        possibleBriefs = []
                        expectedBrief = 'file for class {0}'.format(className)
                        if (isHeaderFile):
                            expectedBrief = 'Header ' + expectedBrief
                            possibleBriefs.append(expectedBrief)
                        else:
                            possibleBriefs.append('Class ' + expectedBrief)
                            possibleBriefs.append('Source ' + expectedBrief)
                        for expectedBrief in possibleBriefs:
                            self.logger.debug('Expected brief: {0}'.format(expectedBrief))
                            atBriefOK = (expectedBrief in line)
                            if (not(atBriefOK)):
                                self.logger.debug('Unexpected brief found: {0}'.format(line))
                            else:
                                break
                elif ('* @author ' in line):
                    author = line.split('@author ')[1]
                    #Remove accents
                    author = unidecode.unidecode(author)
                    self.logger.debug('Author {0} found'.format(author))
                    with open(self.authors) as authors:
                        for existingAuthor in authors:
                            existingAuthor = existingAuthor.rstrip()
                            existingAuthor = unidecode.unidecode(existingAuthor)
                            atAuthorOK = (author == existingAuthor)
                            if (atAuthorOK):
                                break
                    if (not(atAuthorOK)):
                        self.logger.critical('Author {0} was not found in the AUTHORS ({1}) list'.format(author, self.authors))
                elif ('* @copyright ' in line):
                    atCopyrightOK = True
                elif ('* @date ' in line):
                    dateStr = line.split('@date ')[1]
                    try:
                        datetime.datetime.strptime(dateStr, '%d/%m/%Y')
                        atDateOK = True
                    except Exception as e:
                        self.logger.critical('Invalid date found: {0}'.format(e))
                        atDateOK = False
                elif ('#include' in line):
                    includes.append(line)
                elif ('*/' in line):
                    endOfHeaderCommentReached = True
                    includesSort = includes[:]
                    includesSort.sort()
                    if (includesOK):
                        includesOK = (includes == includesSort)
                        if (not includesOK):
                            self.logger.critical('Includes not sorted {0} != {1}'.format(includes, includesSort))
                    #Sort include by blocks /**/
                    includes = []
            
        allOK = atFileOK and atBriefOK and atDateOK and atAuthorOK and atCopyrightOK and includesOK
        status = {'allok': allOK, '@file': atFileOK, '@brief': atBriefOK, '@date': atDateOK, '@author': atAuthorOK, '@copyright': atCopyrightOK, '#includes sorted': includesOK}
        return status


    def CheckHeaderMakefile(self, f):
        """ Verifies that the Makefile copyright are correctly set
        """
        atCopyrightOK = False

        with open(f) as lines:
            for line in lines:
                line = line.rstrip()
                if ('Copyright ' in line):
                    atCopyrightOK = True

        allOK = atCopyrightOK
        status = {'allok': allOK, '@copyright': atCopyrightOK}
        return status


    def Run(self, reporter):
        """ Checks that all the headers are correctly formatted.
        Args:
            args (QAReporter): where to print the output results.
        Returns:
            True if for all the headers are correctly formatted.
        """
        headersStatus = {}
        for f in self.files:
            if (os.path.isfile(f)):
                if (f.startswith('Makefile')):
                    headersStatus[f] = self.CheckHeaderMakefile(f)
                elif (f.endswith('.cpp') or f.endswith('.h')):
                    headersStatus[f] = self.CheckHeader(f)
            else:
                self.logger.warning('File {0} may have been deleted and is no longer available'.format(f))
        
        ok = True
        for hs in headersStatus:
            self.logger.debug('File: {0} header status: {1}'.format(hs, headersStatus[hs]))
            for status in headersStatus[hs]:
                if (status != 'allok'):
                    if (not headersStatus[hs][status]):
                        ok = False
                        self.logger.critical('Invalid {0} in file {1}'.format(status, hs))
                        reporter.WriteError('Invalid {0} in file {1}'.format(status, hs))
        if (ok):
            reporter.WriteOK('All headers are correctly formatted')

        return ok

class CoverageHelper(QAHelper):
    """ Checks the test coverage.
    """

    def Configure(self, args):
        """ Configures the headers helper instance.
        
        Args:
            args (dict): args['lcovoutputdir'] (str) where to store the lcov output (temporary) files (e.g. Build).
                         args['lcovprefix'] (str) Lcov build file prefix (e.g. MARTe2.coverage.info).
                         args['lcovexec'] (str) Lcov GTest executor location. (e.g. Test/GTest/cov/MainGTest.ex).
                         args['lcovfilter'] ([str]) Lcov GTest filters. (e.g. ['BareMetal*', 'Scheduler*', 'FileSystem*']).
                         args['lcovexecdisable'] (bool) If True lcov will not be executed and the output files from a previous run will be used (tipically used only for debug).
                         args['lcovminfun'] (float) Minimum functional coverage ([0, 1]) 
                         args['lcovminlines'] (float) Minimum line coverage ([0, 1]) 
                         args['compbranch'] (str) name of the branch to compare against
                         args['repo'] (git.Repo) git repo to compare coverage against
        Returns:
            True.
        """
        self.lcovOutputDir = args['lcovoutputdir']
        self.lcovBuildPrefix = args['lcovprefix']
        self.lcovExec = args['lcovexec']
        self.lcovTestFilters = args['lcovfilter']
        self.execLCovDisabled = args['lcovexecdisable']
        self.minFunCov = args['lcovminfun']
        self.minLinesCov = args['lcovminlines']
        self.compBranchName = args['compbranch']
        self.repo = args['repo']

    def ExecLCov(self, clean):
        """ Executes lcov
        """
        #Build with coverage enabled
        if (clean):
            self.ExecShellCommand('make -f Makefile.cov clean')

        self.ExecShellCommand('make -f Makefile.cov')
       
        #Run baseline coverage
        self.ExecShellCommand('lcov --capture --initial --directory . --no-external --output-file {0}/{1}.initial'.format(self.lcovOutputDir, self.lcovBuildPrefix))
        
        #Execute the tests
        for f in self.lcovTestFilters:
            self.ExecShellCommand('{0} --gtest_filter={1}'.format(self.lcovExec, f))

        #Create test coverage data file
        self.ExecShellCommand('lcov --capture --directory . --no-external --output-file {0}/{1}.tests'.format(self.lcovOutputDir, self.lcovBuildPrefix))

        #Combine baseline and test coverage data
        self.ExecShellCommand('lcov --add-tracefile {0}/{1}.initial --add-tracefile {0}/{1}.tests --output-file {0}/{1}.1'.format(self.lcovOutputDir, self.lcovBuildPrefix))

        #Remove false positives
        self.ExecShellCommand('lcov --remove {0}/{1}.1 "/Test*" --output-file {0}/{1}.2'.format(self.lcovOutputDir, self.lcovBuildPrefix))
        self.ExecShellCommand('lcov --remove {0}/{1}.2 "*gtest*" --output-file {0}/{1}'.format(self.lcovOutputDir, self.lcovBuildPrefix))

    def GetLCovOutput(self):
        """Gets the lcov output.
        """
        process = self.ExecShellCommand('lcov -l {0}/{1}'.format(self.lcovOutputDir, self.lcovBuildPrefix), False, False)
        errLines = process.stderr.readlines()
        ok = True
        for line in errLines:
            if ('ERROR' in line):
                self.logger.critical('Error executing lcov: {0}'.format(line))
                ok = False

        lcovLines = []
        if (ok):
            lcovLines = process.stdout.readlines()
            
        return lcovLines

    def ParseLCovFile(self, lines):
        """ Parses the lcov output file.
        """
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

    def ReportInsufficientCoverage(self, covType, output, f, actualCov, expectedCov, qualifier = None, referenceCov = None):
        msg = '{0}: insufficient {1} coverage'.format(f, covType)
        if (qualifier is not None):
            msg += ' ({0})'.format(qualifier)
        msg += ': ({0:.1f} % < {1:.1f} %)'.format(actualCov * 100, expectedCov * 100)
        if (referenceCov is not None):
            msg += '. Reference branch ({0:.1f} % < {1:.1f} %)'.format(referenceCov * 100, expectedCov * 100)
        output(msg)

    def ReportCoverageChange(self, covType, output, f, actualCov, expectedCov, qualifier, referenceCov):
        msg = '{0}: was modified. {1} coverage is still OK'.format(f, covType)
        if (qualifier is not None):
            msg += ' ({0})'.format(qualifier)
        msg += '. This branch ({0:.1f} % >= {1:.1f} %). Reference branch ({2:.1f} % >= {1:.1f} %)'.format(actualCov * 100, expectedCov * 100, referenceCov * 100)
        output(msg)

    def Run(self, reporter):
        """ Checks that the functional coverage is as expected.
        Args:
            args (QAReporter): where to print the output results.
        Returns:
            True if no coverage errors are found.
        """
        ok = True
        relativeComparison = False
        ignoreFunCov = (self.minFunCov == 0)
        if (len(self.compBranchName) > 0):
            relativeComparison = True
        #Get absolute results
        if (not self.execLCovDisabled):
            self.ExecLCov(True)
        lcovLines = self.GetLCovOutput()
        if (len(lcovLines) == 0):
            ok = False
            self.logger.critical('Failed to read coverage file {0}'.format(self.lcovBuildPrefix))
            reporter.WriteError('Failed to read coverage file {0}'.format(self.lcovBuildPrefix))
        else:
            covResultsCurrentBranch = self.ParseLCovFile(lcovLines)
            self.logger.debug(covResultsCurrentBranch)
            if (not relativeComparison):
                for f in covResultsCurrentBranch:
                    if (not ignoreFunCov):
                        if (covResultsCurrentBranch[f]['functions'] < self.minFunCov):
                            self.ReportInsufficientCoverage('functional', reporter.WriteError, f, covResultsCurrentBranch[f]['functions'], self.minFunCov)
                            ok = False
                    if (covResultsCurrentBranch[f]['lines'] < self.minLinesCov):
                        self.ReportInsufficientCoverage('line', reporter.WriteError, f, covResultsCurrentBranch[f]['lines'], self.minLinesCov)
                        ok = False

        #Compare results against target branch?
        if (relativeComparison):
            originalPrexif = self.lcovBuildPrefix
            self.lcovBuildPrefix = '{0}_to_compare'.format(self.lcovBuildPrefix)
            if (not self.execLCovDisabled):
                currentBranch = self.repo.active_branch.name
                branchChangedSuccessfully = self.ChangeBranch(self.repo, self.compBranchName)
               
                if (branchChangedSuccessfully):
                    self.ExecLCov(True)
                    branchChangedSuccessfully = self.ChangeBranch(self.repo, currentBranch)

            lcovLines = self.GetLCovOutput()
            if (len(lcovLines) == 0):
                self.logger.critical('Failed to read coverage file {0}'.format(self.lcovBuildPrefix))
                reporter.WriteError('Failed to read coverage file {0}'.format(self.lcovBuildPrefix))
            else:
                covResultsReferenceBranch = self.ParseLCovFile(lcovLines)
                self.logger.debug(covResultsReferenceBranch)
                #Just look into the differences with respect to the comparison branch
                for f in covResultsCurrentBranch:
                    newFile = False
                    functionCoverageChanged = False
                    lineCoverageChanged = False
                    if (f in covResultsReferenceBranch):
                        functionCoverageChanged = covResultsCurrentBranch[f]['functions'] != covResultsReferenceBranch[f]['functions']
                        lineCoverageChanged = covResultsCurrentBranch[f]['lines'] != covResultsReferenceBranch[f]['lines']
                    else:
                        newFile = True

                    covError = False
                    if (not ignoreFunCov):
                        if (covResultsCurrentBranch[f]['functions'] < self.minFunCov):
                            covError = True
                            if (newFile):
                                self.ReportInsufficientCoverage('functional', reporter.WriteError, f, covResultsCurrentBranch[f]['functions'], self.minFunCov, 'new file')
                                ok = False
                            elif(functionCoverageChanged):
                                covIncreased = (covResultsCurrentBranch[f]['functions'] > covResultsReferenceBranch[f]['functions'])
                                qualifier = None
                                output = reporter.WriteOK
                                if (covIncreased):
                                    qualifier = 'but has improved'
                                else:
                                    output = reporter.WriteError
                                    ok = False
                                self.ReportInsufficientCoverage('functional', output, f, covResultsCurrentBranch[f]['functions'], self.minFunCov, qualifier, covResultsReferenceBranch[f]['functions'])

                    if (covResultsCurrentBranch[f]['lines'] < self.minLinesCov):
                        covError = True
                        if (newFile):
                            self.ReportInsufficientCoverage('line', reporter.WriteError, f, covResultsCurrentBranch[f]['lines'], self.minLinesCov, 'new file')
                            ok = False
                        elif(lineCoverageChanged):
                            covIncreased = (covResultsCurrentBranch[f]['lines'] > covResultsReferenceBranch[f]['lines'])
                            qualifier = None
                            output = reporter.WriteOK
                            if (covIncreased):
                                qualifier = 'but has improved'
                            else:
                                output = reporter.WriteError
                                ok = False
                            self.ReportInsufficientCoverage('line', output, f, covResultsCurrentBranch[f]['lines'], self.minLinesCov, qualifier, covResultsReferenceBranch[f]['lines'])

                    if (not covError): 
                        if (not ignoreFunCov):
                            if (functionCoverageChanged):
                                #Function coverage changed
                                covDecreased = (covResultsCurrentBranch[f]['functions'] < covResultsReferenceBranch[f]['functions'])
                                qualifier = None
                                output = reporter.WriteOK
                                if (covDecreased):
                                    output = reporter.WriteWarning
                                    qualifier = 'but has decreased'
                                self.ReportCoverageChange('functional', output, f, covResultsCurrentBranch[f]['functions'], self.minFunCov, qualifier, covResultsReferenceBranch[f]['functions'])

                        if (lineCoverageChanged):
                            #Line coverage changed
                            covDecreased = (covResultsCurrentBranch[f]['lines'] < covResultsReferenceBranch[f]['lines'])
                            qualifier = None
                            output = reporter.WriteOK
                            if (covDecreased):
                                output = reporter.WriteWarning
                                qualifier = 'but has decreased'
                            self.ReportCoverageChange('line', output, f, covResultsCurrentBranch[f]['lines'], self.minLinesCov, qualifier, covResultsReferenceBranch[f]['lines'])

                        if (newFile):
                            if (not ignoreFunCov):
                                msg = '{0}: is new. Functional coverage is OK: ({1:.1f} % >= {2:.1f} %)'.format(f, covResultsCurrentBranch[f]['functions'] * 100, self.minFunCov * 100)
                                reporter.WriteOK(msg)
                            msg = '{0}: is new. Line coverage is OK: ({1:.1f} % >= {2:.1f} %)'.format(f, covResultsCurrentBranch[f]['lines'] * 100, self.minLinesCov* 100)
                            reporter.WriteOK(msg)


            self.lcovBuildPrefix = originalPrexif

        if (ok):
            reporter.WriteOK('No coverage errors found')
        return ok

class DoxyHelper(QAHelper):
    """ Helper QA class that checks for errors in the doxygen generation.
    """

    def Configure(self, args):
        """ Configures the lint helper instance.
        
        Args:
            args (dict): args['doxygenfile'] (str) the Doxygen file.
        Returns:
            True.
        """
        self.doxygenFile = args['doxygenfile']

        return True

    def Run(self, reporter):
        """ Runs doxygen and verifies if there are errors in the output.
        Args:
            args (QAReporter): where to print the output results.
        Returns:
            True if there are no errors in the doxygen generation. 
        """
        process = self.ExecShellCommand('doxygen {0}'.format(self.doxygenFile), True, False)
        ignore = process.stdout.readlines()
        errLines = process.stderr.readlines()
        ok = True
        errorKeywords = ['warning:', 'error:']
        for line in errLines:
            for keyword in errorKeywords:
                if (keyword in line):
                    reporter.WriteError(line)
                    ok = False

        return ok

class GTestHelper(QAHelper):
    """ Helper QA class that checks the number of executed and failed tests.
    """

    def Configure(self, args):
        """ Configures the headers helper instance.
        
        Args:
            args (dict): args['gtestoutputdir'] (str) the location of the output folder (e.g. Build).
                         args['gtestoutputfileprefix'] (str) GTest output xml filename prefix (e.g. gtest_out).
                         args['gtestexec'] (str) GTest executor location. (e.g. Build/x86-linux/GTest/MainGTest.ex).
                         args['gtestfilter'] ([str]) GTest filters. (e.g. ['BareMetal*', 'Scheduler*', 'FileSystem*']).
                         args['gtestexecdisable'] (bool) If True GTest will not be executed and the output files from a previous run will be used (tipically used only for debug).
                         args['gtesttarget'] (str) GTest target platform
                         args['compbranch'] (str) name of the branch to compare against
                         args['repo'] (git.Repo) git repo to compare coverage against
        Returns:
            True.
        """
        self.gtestOutputDir = args['gtestoutputdir']
        self.gtestOutputFilePrefix = args['gtestoutputfileprefix']
        self.gtestExec = args['gtestexec']
        self.gtestTarget = args['gtesttarget']
        self.gtestFilters = args['gtestfilter']
        self.gtestExecDisabled = args['gtestexecdisable']
        self.compBranchName = args['compbranch']
        self.repo = args['repo']

    def ExecGTest(self, clean):
        """ Executes GTest against the predefined filters.
        """

        #Build with coverage enabled
        if (clean):
            self.ExecShellCommand('make -f Makefile.{0} clean'.format(self.gtestTarget))

        self.ExecShellCommand('make -f Makefile.{0}'.format(self.gtestTarget))
       
        #Execute the tests
        for idx, fil in enumerate(self.gtestFilters):
            self.ExecShellCommand('{0} --gtest_filter={1} --gtest_output=xml:{2}/{3}_{4}.xml'.format(self.gtestExec, fil, self.gtestOutputDir, self.gtestOutputFilePrefix, idx))


    def ParseGTestOutputFile(self, f):
        """ Parse the gtest xml output files
        """
        self.logger.debug('Going to parse file {0}'.format(f))
        ret = {}
        try:
            tree = etree.parse(f)
            if (tree is not None):
                xmlRoot = tree.getroot()
                testSuitesNode = xmlRoot
                ret['tests'] = int(testSuitesNode.attrib['tests'])
                ret['failures'] = int(testSuitesNode.attrib['failures'])
                ret['disabled'] = int(testSuitesNode.attrib['disabled'])
                ret['errors'] = int(testSuitesNode.attrib['errors'])
                ret['failedTests'] = list([test.get('name') for test in xmlRoot.xpath('*/testcase[failure]')])
                self.logger.debug(ret)
            else:
                self.logger.critical("Could not find the xml tree for {0}".format(f))

        except Exception as e:
            self.logger.critical('Failed to parse {0}:{1}'.format(f, e))
            
        return ret


    def ReportTestQuantity(self, output, qualifier, currentTotalTests, referenceTotalTests):
        output('Number of tests {0}. Current branch: {1}. Reference branch: {2}'.format(qualifier, currentTotalTests, referenceTotalTests))

    def ReportErrors(self, output, qualifier, currentTotalTests, currentTotalErrors, referenceTotalErrors):
        output('Number of errors {0}. Number of tests: {1}. Current branch: {2}. Reference branch: {3}'.format(qualifier, currentTotalTests, currentTotalErrors, referenceTotalErrors))

    def ReportFailedTests(self, reporter, currentFailedTests, referenceFailedTests):
        common = []
        onlyCurrent = []
        onlyReference = []
        for test in currentFailedTests:
            if test in referenceFailedTests:
                common.append(test)
            else:
                onlyCurrent.append(test)
        for test in referenceFailedTests:
            if test not in common:
                onlyReference.append(test)
        if len(common):
            reporter.SetHelper('Unsolved Failing Tests')
            for test in common: 
                reporter.WriteError(test)
        if len(onlyReference):
            reporter.SetHelper('Solved Failing Tests')
            for test in onlyReference:
                reporter.WriteError(test)
        if len(onlyCurrent):
            reporter.SetHelper('New Failing Tests')
            for test in onlyCurrent:
                reporter.WriteError(test)

    def Run(self, reporter):
        """ Checks that all the test pass.
        Args:
            args (QAReporter): where to print the output results.
        Returns:
            True if no tests fail and if the number of tests did not decrease w.r.t. to the target branch.
        """
        ok = True
        relativeComparison = False
        if (len(self.compBranchName) > 0):
            relativeComparison = True
        #Get absolute results
        if (not self.gtestExecDisabled):
            self.ExecGTest(True)

        currentTotalTests = 0
        currentTotalErrors = 0
        currentFailedTests = []

        for idx, fil in enumerate(self.gtestFilters):
            gtestOutputFile = '{0}/{1}_{2}.xml'.format(self.gtestOutputDir, self.gtestOutputFilePrefix, idx)
            gtestOutput = self.ParseGTestOutputFile(gtestOutputFile)
            if (len(gtestOutput) == 0):
                reporter.WriteError('Failed to retrieve test information from: {0}'.format(gtestOutputFile))
                ok = False
                break
            else:
                currentTotalTests += gtestOutput['tests']
                currentTotalErrors += gtestOutput['failures']
                currentFailedTests += gtestOutput['failedTests']

        if (relativeComparison):
            originalPrexif = self.gtestOutputFilePrefix
            self.gtestOutputFilePrefix = '{0}_to_compare'.format(self.gtestOutputFilePrefix)
            if (not self.gtestExecDisabled):
                currentBranch = self.repo.active_branch.name
                branchChangedSuccessfully = self.ChangeBranch(self.repo, self.compBranchName)
               
                if (branchChangedSuccessfully):
                    self.ExecGTest(True)
                    branchChangedSuccessfully = self.ChangeBranch(self.repo, currentBranch)
           
            referenceTotalTests = 0
            referenceTotalErrors = 0
            referenceFailedTests = []
            for idx, fil in enumerate(self.gtestFilters):
                gtestOutputFile = '{0}/{1}_{2}.xml'.format(self.gtestOutputDir, self.gtestOutputFilePrefix, idx)
                gtestOutput = self.ParseGTestOutputFile(gtestOutputFile)
                if (len(gtestOutput) == 0):
                    reporter.WriteError('Failed to retrieve test information from: {0}'.format(gtestOutputFile))
                    ok = False
                    break
                else:
                    referenceTotalTests += gtestOutput['tests']
                    referenceTotalErrors += gtestOutput['failures']
                    referenceFailedTests += gtestOutput['failedTests']

            if (currentTotalTests == referenceTotalTests):
                self.ReportTestQuantity(reporter.WriteOK, 'unchanged', currentTotalTests, referenceTotalTests)
            elif (currentTotalTests < referenceTotalTests):
                self.ReportTestQuantity(reporter.WriteError, 'decreased', currentTotalTests, referenceTotalTests)
            else:
                self.ReportTestQuantity(reporter.WriteOK, 'increased', currentTotalTests, referenceTotalTests)


            if (currentTotalErrors > 0):
                if (currentTotalErrors > referenceTotalErrors):
                    self.ReportErrors(reporter.WriteError, 'has increased', currentTotalTests, currentTotalErrors, referenceTotalErrors)
                elif (currentTotalErrors < referenceTotalErrors):
                    self.ReportErrors(reporter.WriteWarning, 'has decreased (but there are still errors)', currentTotalTests, currentTotalErrors, referenceTotalErrors)
                else:
                    self.ReportErrors(reporter.WriteWarning, 'has not changed (but there are still errors)', currentTotalTests, currentTotalErrors, referenceTotalErrors)
            else:
                if (currentTotalErrors < referenceTotalErrors):
                    self.ReportErrors(reporter.WriteOK, 'has decreased (and there are no more errors)', currentTotalTests, currentTotalErrors, referenceTotalErrors)
                else:
                    self.ReportErrors(reporter.WriteOK, 'is zero', currentTotalTests, currentTotalErrors, referenceTotalErrors)


            if referenceTotalErrors or currentTotalErrors:
                self.ReportFailedTests(reporter, currentFailedTests, referenceFailedTests)

            self.gtestOutputFilePrefix = originalPrexif
        else:
            msg = '{0} tests executed. {1} tests have failed.'.format(currentTotalTests, currentTotalErrors)
            if (currentTotalErrors == 0):
                reporter.WriteOK(msg)
            else:
                reporter.WriteError(msg)

        return ok

