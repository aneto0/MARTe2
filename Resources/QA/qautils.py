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
__date__ = '23/09/2020'

##
# Imports
##
import git
import subprocess

def GetChangedFiles(logger, repo, branch):
    changedFiles = []
    index = repo.index
    diffs = index.diff(branch)
    for diff in diffs:
        fname = diff.a_rawpath.decode('ascii')
        changedFiles.append(fname)
        logger.debug('The file {0} changed w.r.t. to the branch {1}'.format(fname, branch))
    return changedFiles

def ExecShellCommand(logger, command, logOutput = True, logErrors = True):
    """ Allows to keep enviroment variables and wait for the command to complete while printing to the screen
    """
    logger.info('Calling command {0}'.format(command))
    if not isinstance(command, list):
        command = [command]
    process = subprocess.Popen(command, 
                               stdout=subprocess.PIPE, 
                               stderr=subprocess.PIPE, 
                               universal_newlines=True, 
                               shell=True, 
                               encoding='utf8', 
                               errors='ignore')
    if (logOutput):
        for outLine in iter(process.stdout.readline, ''):
            logger.debug(outLine)
    if (logErrors):
        for errLine in iter(process.stderr.readline, ''):
            logger.critical(errLine)
    process.wait()
    return process

def ChangeBranch(logger, repo, targetBranch):
    """ Change git branch 
    """
    ok = True
    currentBranch = repo.active_branch.name
    try:
        logger.debug('Checking out to branch {0} from branch {1}'.format(targetBranch, currentBranch))
        repo.git.checkout(targetBranch)
        repo.git.pull()
    except Exception as e:
        ok = False
        logger.critical('Failed checkout to branch {0} from branch {1}. Error: {2}'.format(targetBranch, currentBranch, e))

    return ok


