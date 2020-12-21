.. date: 21/12/2018
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

Quality Assurance Helper
========================

In a nutshell, any component (irrespectively of being from the core, the MARTe2-components or a project that 
claims compliance with the MARTe2 QA system) shall meet the following criteria:

#. The format of the source files shall be according to the styles defined in the QA documentation (see :vcisgldocqa:`Coding standard <MARTe_Coding_Standard_32SNQE.pdf>`);
#. Projects shall be structured as described in the :doc:`deploying </deploying/deploying>` chapter;
#. All methods and variables (both private and public) in the header files shall be fully documented, with no ``doxygen`` errors;
#. Each class shall be declared and defined in independent header and unit files

    #. Ideally the header and unit filenames match the class name;
    #. Private helper classes and methods are allowed in the unit files.

#. All source files shall be linted with no errors (exceptions can be added where needed and will be analysed by the QA reviewer);
#. Each public method shall be tested at least once:

    #. The name of the testing function shall be ``TestFunctionToBeTested``, where ``FunctionToBeTested`` is the name of the public function being tested;

#. Coverage shall be > 90 % for all components and shall be 100 % for components that do not have any dependency on operating system/hardware calls (e.g. GAMs);
#. The output of the QA review shall be written in the associated redmine user-story (e.g. https://vcis-redmine.f4e.europa.eu/issues/351);
#. Once the user-story is QA reviewed it shall be merged into the ``develop`` branch;
#. Every project release (i.e. merge into the ``master`` branch) shall be preceded by a QA review of the ``develop`` branch (e.g. https://vcis-redmine.f4e.europa.eu/issues/674);

QAHelper
~~~~~~~~

After obtaining a user-story number, the developer of a feature (e.g. a new component) shall create a new branch in the relevant ``git`` repository with the name ``UID-FeatureName``, where the ``UID`` is the user-story unique identifier and the ``FeatureName`` is a CaMeL set of words that identify the feature.

Once the development is deemed to be completed (including all the QA features described above), it shall be validated using the :vcisgl:`QAHelper <MARTe2/tree/master/Resources/QA>` python tool. 

By default the :vcisgl:`QAHelper <MARTe2/tree/master/Resources/QA>` will execute all the QA checks described above and report in the selected output formats.

A merge request in the ``develop`` branch shall only be requested when no errors are reported by the ``QAHelper``. A ``redmine`` output report shall be generated and its contents written on the feature's user-story.

.. code-block:: bash

    usage: QAHelper.py [-h] [-l {CRITICAL,ERROR,WARNING,INFO,DEBUG}] [-b BRANCH]
                   [-r ROOTDIR] [-s SOURCEDIR] [-g GTESTFOLDER] [-a AUTHORS]
                   [-ru REVIEWAUTHOR] [-df DFILE] [-af]
                   [-if [LINTINCLUDES [LINTINCLUDES ...]]]
                   [-ii [LINTIGNORE [LINTIGNORE ...]]] [-xx LINTEREXEC]
                   [-xd LINTERDIR] [-xh LINTERHOSTNAME]
                   [-xr [LINTERRSYNCSOURCE [LINTERRSYNCSOURCE ...]]]
                   [-xt [LINTERRSYNCTARGET [LINTERRSYNCTARGET ...]]]
                   [-xe [LINTERRSYNCEXCLUDE [LINTERRSYNCEXCLUDE ...]]]
                   [-xi [LINTERINCLUDE [LINTERINCLUDE ...]]]
                   [-xu LINTERUSERNAME] [-xf LINTERFILENAME]
                   [-xo LINTEROUTPUTFILENAME]
                   [-xg [LINTERGREP [LINTERGREP ...]]] [-lg LCOVEXEC]
                   [-lf [LCOVFILTER [LCOVFILTER ...]]] [-ld LCOVOUTPUTDIR]
                   [-lp LCOVPREFIX] [-le] [-la LCOVMINLINES] [-lb LCOVMINFUN]
                   [-do DOXYGENFILE] [-gg GTESTEXEC]
                   [-gf [GTESTFILTER [GTESTFILTER ...]]] [-gd GTESTOUTPUTDIR]
                   [-gp GTESTOUTPUTFILEPREFIX] [-ge] [-gt GTESTTARGET] [-ed]
                   [-el] [-ex] [-ec] [-eh] [-ef] [-eg] [-er]
                   [-ro REDMINEOUTPUTFILE] [-ho HTMLOUTPUTFILE]
                   [-fe [FUNCTIONALTESTEXCEPTIONS [FUNCTIONALTESTEXCEPTIONS ...]]]
    Script to support the QA activities
    optional arguments:
      -h, --help            show this help message and exit
      -l {CRITICAL,ERROR,WARNING,INFO,DEBUG}, --loglevel {CRITICAL,ERROR,WARNING,INFO,DEBUG}
                            Log level, one of: CRITICAL, ERROR, WARNING, INFO,
                            DEBUG
      -b BRANCH, --branch BRANCH
                            Target branch against which the changes should be
                            validated
      -r ROOTDIR, --rootdir ROOTDIR
                            The project root directory
      -s SOURCEDIR, --sourcedir SOURCEDIR
                            The project Source directory
      -g GTESTFOLDER, --gtestfolder GTESTFOLDER
                            The project GTest directory
      -a AUTHORS, --authors AUTHORS
                            The project AUTHORS file
      -ru REVIEWAUTHOR, --reviewauthor REVIEWAUTHOR
                            The name of the person who did the QA review
      -df DFILE, --dfile DFILE
                            Validate a specific file (mostly for debug)
      -af, --allfiles       Validates all source files (mostly for debug)
      -if [LINTINCLUDES [LINTINCLUDES ...]], --lintincludes [LINTINCLUDES [LINTINCLUDES ...]]
                            Lint list of includes files to be linted
      -ii [LINTIGNORE [LINTIGNORE ...]], --lintignore [LINTIGNORE [LINTIGNORE ...]]
                            Lint list of files to be ignored
      -xx LINTEREXEC, --linterexec LINTEREXEC
                            Linter executable
      -xd LINTERDIR, --linterdir LINTERDIR
                            Directory where to trigger the lint from
      -xh LINTERHOSTNAME, --linterhostname LINTERHOSTNAME
                            Linter hostname
      -xr [LINTERRSYNCSOURCE [LINTERRSYNCSOURCE ...]], --linterrsyncsource [LINTERRSYNCSOURCE [LINTERRSYNCSOURCE ...]]
                            Linter source directories to be rsynced
      -xt [LINTERRSYNCTARGET [LINTERRSYNCTARGET ...]], --linterrsynctarget [LINTERRSYNCTARGET [LINTERRSYNCTARGET ...]]
                            Linter target directories to be rsynced (should have
                            the same size of linterrsyncsource)
      -xe [LINTERRSYNCEXCLUDE [LINTERRSYNCEXCLUDE ...]], --linterrsyncexclude [LINTERRSYNCEXCLUDE [LINTERRSYNCEXCLUDE ...]]
                            Linter directories not to be rsynced
      -xi [LINTERINCLUDE [LINTERINCLUDE ...]], --linterinclude [LINTERINCLUDE [LINTERINCLUDE ...]]
                            Linter include directories
      -xu LINTERUSERNAME, --linterusername LINTERUSERNAME
                            Linter server username
      -xf LINTERFILENAME, --linterfilename LINTERFILENAME
                            Linter input filename
      -xo LINTEROUTPUTFILENAME, --linteroutputfilename LINTEROUTPUTFILENAME
                            Linter output filename
      -xg [LINTERGREP [LINTERGREP ...]], --lintergrep [LINTERGREP [LINTERGREP ...]]
                            Grep out these keywords
      -lg LCOVEXEC, --lcovexec LCOVEXEC
                            Lcov GTest executor location
      -lf [LCOVFILTER [LCOVFILTER ...]], --lcovfilter [LCOVFILTER [LCOVFILTER ...]]
                            Lcov GTest filters
      -ld LCOVOUTPUTDIR, --lcovoutputdir LCOVOUTPUTDIR
                            Where to write the lcov temporary output files
      -lp LCOVPREFIX, --lcovprefix LCOVPREFIX
                            Lcov GTest build file prefix
      -le, --lcovexecdisable
                            Disable the lcov program? Mostly used for debug
      -la LCOVMINLINES, --lcovminlines LCOVMINLINES
                            Minimum line coverage ([0, 1])
      -lb LCOVMINFUN, --lcovminfun LCOVMINFUN
                            Minimum function coverage ([0, 1])
      -do DOXYGENFILE, --doxygenfile DOXYGENFILE
                            Name of the doxygen file
      -gg GTESTEXEC, --gtestexec GTESTEXEC
                            GTest executor location
      -gf [GTESTFILTER [GTESTFILTER ...]], --gtestfilter [GTESTFILTER [GTESTFILTER ...]]
                            GTest filters
      -gd GTESTOUTPUTDIR, --gtestoutputdir GTESTOUTPUTDIR
                            Where to write the gtest output files
      -gp GTESTOUTPUTFILEPREFIX, --gtestoutputfileprefix GTESTOUTPUTFILEPREFIX
                            GTest build file prefix
      -ge, --gtestexecdisable
                            Disable the GTest execution and using outputs from
                            previous tests? Mostly used for debug
      -gt GTESTTARGET, --gtesttarget GTESTTARGET
                            GTest target platform
      -ed, --excludedoxygen
                            Disable doxygen checking
      -el, --excludelinterincludes
                            Disable linter include checking
      -ex, --excludelinterexec
                            Disable linter execution checking
      -ec, --excludecoverage
                            Disable coverage checking
      -eh, --excludeheaders
                            Disable headers checking
      -ef, --excludefunctional
                            Disable functional tests checking
      -eg, --excludegtest   Disable GTest checking
      -er, --excludereport  Disable reporting (mostly for debug)
      -ro REDMINEOUTPUTFILE, --redmineoutputfile REDMINEOUTPUTFILE
                            Redmine output file
      -ho HTMLOUTPUTFILE, --htmloutputfile HTMLOUTPUTFILE
                            HTML output file
      -fe [FUNCTIONALTESTEXCEPTIONS [FUNCTIONALTESTEXCEPTIONS ...]], --functionaltestexceptions [FUNCTIONALTESTEXCEPTIONS [FUNCTIONALTESTEXCEPTIONS ...]]
                            List of files that are not supposed to be tested


