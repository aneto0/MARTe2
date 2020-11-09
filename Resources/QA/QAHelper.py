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
import unidecode

from qahelpers import DoxyHelper
from qahelpers import GTestHelper
from qahelpers import HeadersHelper
from qahelpers import FunctionalTestsHelper
from qahelpers import CoverageHelper
from qahelpers import LinterExecutorHelper
from qahelpers import LintIncludeHelper
import qautils
from qareporters import CompositeReporter
from qareporters import ConsoleReporter
from qareporters import HTMLReporter
from qareporters import RedmineReporter 

# Create a custom logger
logger = logging.getLogger('{0}'.format(__name__))

# Create handlers
consoleHandler = logging.StreamHandler()

# Create formatters and add it to handlers
consoleHandlerFormat = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
consoleHandler.setFormatter(consoleHandlerFormat)

# Add handlers to the logger
logger.addHandler(consoleHandler)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description = 'Script to support the QA activities')
    logOptions = ['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG']
    parser.add_argument('-l', '--loglevel', type=str, help='Log level, one of: CRITICAL, ERROR, WARNING, INFO, DEBUG', default='INFO', choices=logOptions)
    parser.add_argument('-b', '--branch', type=str, help='Target branch against which the changes should be validated', default='develop')
    parser.add_argument('-r', '--rootdir', type=str, help='The project root directory', default='.')
    parser.add_argument('-s', '--sourcedir', type=str, help='The project Source directory', default='Source')
    parser.add_argument('-g', '--gtestfolder', type=str, help='The project GTest directory', default='Test/GTest')
    parser.add_argument('-a', '--authors', type=str, help='The project AUTHORS file', default='AUTHORS')
    parser.add_argument('-ru', '--reviewauthor', type=str, help='The name of the person who did the QA review', default='TODO')
    parser.add_argument('-df', '--dfile', type=str, help='Validate a specific file (mostly for debug)', default='')
    parser.add_argument('-af', '--allfiles', action='store_true', help='Validates all source files (mostly for debug)', default='')
    parser.add_argument('-if', '--lintincludes', type=str, help='Lint list of includes files to be linted', nargs='*', default=['MakeDefaults/Lint/marte_flint_files.lnt'])
    parser.add_argument('-ii', '--lintignore', type=str, help='Lint list of files to be ignored', nargs='*', default=['MARTeApp.cpp', 'Bootstrap.cpp'])
    parser.add_argument('-xx', '--linterexec', type=str, help='Linter executable', default='/opt/FlexeLint/flint')
    parser.add_argument('-xd', '--linterdir', type=str, help='Directory where to trigger the lint from', default='MARTe2-dev')
    parser.add_argument('-xh', '--linterhostname', type=str, help='Linter hostname', default='flexelint.codac.local')
    parser.add_argument('-xr', '--linterrsyncsource', type=str, help='Linter source directories to be rsynced', nargs='*', default=['.'])
    parser.add_argument('-xt', '--linterrsynctarget', type=str, help='Linter target directories to be rsynced (should have the same size of linterrsyncsource)', nargs='*', default=['MARTe2-dev'])
    parser.add_argument('-xe', '--linterrsyncexclude', type=str, help='Linter directories not to be rsynced', nargs='*', default=['Lib', 'Test', '.git', 'Build', 'cov'])
    parser.add_argument('-xi', '--linterinclude', type=str, help='Linter include directories', nargs='*', default=['/opt/FlexeLint/supp/lnt', 'MakeDefaults/Lint'])
    parser.add_argument('-xu', '--linterusername', type=str, help='Linter server username', default=os.environ.get('USER', None))
    parser.add_argument('-xf', '--linterfilename', type=str, help='Linter input filename', default='marte_flint_eclipse.lnt')
    parser.add_argument('-xo', '--linteroutputfilename', type=str, help='Linter output filename', default='')
    parser.add_argument('-xg', '--lintergrep', type=str, help='Grep out these keywords', nargs='*', default=['lint-linux.h'])
    parser.add_argument('-lg', '--lcovexec', type=str, help='Lcov GTest executor location', default='Test/GTest/cov/MainGTest.ex')
    parser.add_argument('-lf', '--lcovfilter', type=str, help='Lcov GTest filters', nargs='*', default=['*'])
    parser.add_argument('-ld', '--lcovoutputdir', type=str, help='Where to write the lcov temporary output files', default='Build')
    parser.add_argument('-lp', '--lcovprefix', type=str, help='Lcov GTest build file prefix', default='MARTe2-coverage.info')
    parser.add_argument('-le', '--lcovexecdisable', action='store_true', help='Disable the lcov program? Mostly used for debug')
    parser.add_argument('-la', '--lcovminlines', type=float, help='Minimum line coverage ([0, 1])', default=0.9)
    parser.add_argument('-lb', '--lcovminfun', type=float, help='Minimum function coverage ([0, 1])', default=0)
    parser.add_argument('-do', '--doxygenfile', type=str, help='Name of the doxygen file', default='Doxyfile')
    parser.add_argument('-gg', '--gtestexec', type=str, help='GTest executor location', default='Build/x86-linux/GTest/MainGTest.ex')
    parser.add_argument('-gf', '--gtestfilter', type=str, help='GTest filters', nargs='*', default=['*'])
    parser.add_argument('-gd', '--gtestoutputdir', type=str, help='Where to write the gtest output files', default='Build')
    parser.add_argument('-gp', '--gtestoutputfileprefix', type=str, help='GTest build file prefix', default='gtest_results')
    parser.add_argument('-ge', '--gtestexecdisable', action='store_true', help='Disable the GTest execution and using outputs from previous tests? Mostly used for debug')
    parser.add_argument('-gt', '--gtesttarget', type=str, help='GTest target platform', default='x86-linux')
    parser.add_argument('-ed', '--excludedoxygen', action='store_true', help='Disable doxygen checking' )
    parser.add_argument('-el', '--excludelinterincludes', action='store_true', help='Disable linter include checking' )
    parser.add_argument('-ex', '--excludelinterexec', action='store_true', help='Disable linter execution checking' )
    parser.add_argument('-ec', '--excludecoverage', action='store_true', help='Disable coverage checking' )
    parser.add_argument('-eh', '--excludeheaders', action='store_true', help='Disable headers checking' )
    parser.add_argument('-ef', '--excludefunctional', action='store_true', help='Disable functional tests checking' )
    parser.add_argument('-eg', '--excludegtest', action='store_true', help='Disable GTest checking' )
    parser.add_argument('-er', '--excludereport', action='store_true', help='Disable reporting (mostly for debug)' )
    parser.add_argument('-ro', '--redmineoutputfile', type=str, help='Redmine output file', default='/tmp/redmine.out.txt')
    parser.add_argument('-ho', '--htmloutputfile', type=str, help='HTML output file', default='/tmp/qahelper.html')
    parser.add_argument('-fe', '--functionaltestexceptions', type=str, help='List of files that are not supposed to be tested', nargs='*', default='[]')

    args = parser.parse_args()
    
    logCriticalities = [logging.CRITICAL, logging.ERROR, logging.WARNING, logging.INFO, logging.DEBUG]
    logCriticality = logCriticalities[logOptions.index(args.loglevel)]
    logger.setLevel(logCriticality)
    consoleHandler.setLevel(logCriticality)

    if ((len(args.dfile)) != 0 or (args.allfiles)):
        args.branch = ''

    repo = git.Repo(args.rootdir)
    currentBranch = repo.active_branch.name
    referenceBranch = args.branch
    codeVersion = repo.head.object.hexsha

    conReporter = ConsoleReporter(logger)
    redReporter = RedmineReporter(logger)
    redReporter.Configure({'reviewauthor': args.reviewauthor, 'outputfile': args.redmineoutputfile, 'codeversion': codeVersion})
    htmlReporter = HTMLReporter(logger)
    htmlReporter.Configure({'reviewauthor': args.reviewauthor, 'outputfile': args.htmloutputfile, 'currentbranch': currentBranch, 'referencebranch': referenceBranch, 'codeversion': codeVersion})

    reporter = CompositeReporter(logger)
    reporter.Configure({'reporters': [conReporter, redReporter, htmlReporter]})

    reporter.SetHelper('General')

    if (len(args.dfile) != 0):
        changedFiles = [args.dfile]
        reporter.WriteInfo('The following files will be checked (set by user)')
        for f in changedFiles:
            reporter.WriteInfo(f)
    elif (args.allfiles):
        changedFiles = [f for f in glob.glob('{0}/**/*.h'.format(args.sourcedir), recursive=True)]
        reporter.WriteInfo('All project source files will be checked')
    elif (len(args.branch) > 0):
        # Try to change branch to trap issues early on
        if (not qautils.ChangeBranch(logger, repo, args.branch)):
            logger.critical('Could not change to branch {0}'.format(args.branch))
            exit(1)
        if (not qautils.ChangeBranch(logger, repo, currentBranch)):
            logger.critical('Could not change back to branch {0}'.format(currentBranch))
            exit(1)

        changedFiles = qautils.GetChangedFiles(logger, repo, args.branch)
        reporter.WriteInfo('The following files were detected as changed between branches {0} and {1}'.format(repo.active_branch.name, args.branch))
        for f in changedFiles:
            reporter.WriteInfo(f)
    else:
        logger.critical('One of the following arguments (with the correct value) shall be set -af, -df, -b')
        exit(1)


    #Doxygen
    if (not args.excludedoxygen):
        reporter.SetHelper('Doxygen')
        dh = DoxyHelper(logger)
        dh.Configure({'doxygenfile': args.doxygenfile})
        dh.Run(reporter)

    #Headers
    if (not args.excludeheaders):
        reporter.SetHelper('Headers')
        hh = HeadersHelper(logger)
        hh.Configure({'authors': args.authors, 'files': changedFiles})
        hh.Run(reporter)

    #Linter includes
    if (not args.excludelinterincludes):
        reporter.SetHelper('Lint includes')
        li = LintIncludeHelper(logger)
        li.Configure({'sourcedir': args.sourcedir, 'lintincludes': args.lintincludes, 'lintignore': args.lintignore})
        li.Run(reporter)

    #Linter executor
    if (not args.excludelinterexec):
        reporter.SetHelper('Linter')
        lx = LinterExecutorHelper(logger)
        lx.Configure({'linterexec': args.linterexec, 'linterhostname': args.linterhostname, 'linterdir': args.linterdir, 'linterrsyncsource': args.linterrsyncsource, 'linterrsynctarget': args.linterrsynctarget, 'linterrsyncexclude': args.linterrsyncexclude, 'linterinclude': args.linterinclude, 'linterusername': args.linterusername, 'linterfilename': args.linterfilename, 'lintergrep': args.lintergrep, 'linteroutputfilename': args.linteroutputfilename})
        lx.Run(reporter)

    #Functional tests
    if (not args.excludefunctional):
        reporter.SetHelper('Functional tests')
        fh = FunctionalTestsHelper(logger)
        fh.Configure({'gtestfolder': args.gtestfolder, 'sourcedir': args.sourcedir, 'files': changedFiles, 'exceptions': args.functionaltestexceptions})
        fh.Run(reporter)

    #LCov
    if (not args.excludecoverage):
        reporter.SetHelper('Coverage')
        ch = CoverageHelper(logger)
        ch.Configure({'lcovoutputdir': args.lcovoutputdir, 'lcovprefix': args.lcovprefix, 'lcovexec': args.lcovexec,
            'lcovfilter': args.lcovfilter, 'lcovexecdisable': args.lcovexecdisable, 'lcovminfun': args.lcovminfun, 
            'lcovminlines': args.lcovminlines, 'compbranch': args.branch, 'repo': repo})
        ch.Run(reporter)

    #GTest
    if (not args.excludegtest):
        reporter.SetHelper('GTest')
        gh = GTestHelper(logger)
        gh.Configure({'gtestoutputdir': args.gtestoutputdir, 'gtestoutputfileprefix': args.gtestoutputfileprefix, 'gtestexec': args.gtestexec,
            'gtestfilter': args.gtestfilter, 'gtestexecdisable': args.gtestexecdisable, 'gtesttarget': args.gtesttarget, 'compbranch': args.branch, 'repo': repo})
        gh.Run(reporter)

    if (not args.excludereport):
        reporter.Terminate()

