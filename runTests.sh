#!/bin/bash
## ######################################################################
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Flow Boiling and Condensation Experiment (FBCE)
##
## Filename: runTests.sh
## File Date: 20160714
##
## Authors **
## Author: Kevin Hannigan 
##         Jordan Higgins
##         Kevin L. Rak (KLR)
##         Matthew Mendoza
#
## Version and Traceability **
## Subversion: @version $Id: runTests.sh 2015 2018-04-05 16:37:42Z mjmendo2 $
##
## Revision History:
##   <Date> <Name of Change Agent>
##   Description:
##     - Bulleted list of changes.
##
## #####################################################################

# This is a script to run all the Qt Unit tests in this directory.
#
# It goes to each directory in this folder which begins with "Test",
# and builds and runs the test project in that folder.
#
# After running the tests, it uses gcov to perform code coverage analysis

SCRIPT_DIR=$(pwd)

#Pull projects to test from current directory listing
declare -A TESTS_TO_RUN
for directory in $(echo Test*/); do
    TESTS_TO_RUN["${directory::-1}"]="${directory::-1}"
done;

#Array for failed build and run by name
declare -a FAILED_BUILD_TESTS
declare -a FAILED_RUN_TESTS

#Command Line Args
declare -A filter
for arg in "$@"
do
    case $arg in
        -xml) #Unit test output will be in xml
            XML_OUTPUT=YES
        ;;
        *) #Run specific tests instead of all tests
            if [[ ${TESTS_TO_RUN[$arg]} ]] ; then
                filter["$arg"]="${TESTS_TO_RUN[$arg]}"
            else
                echo "Unrecognized Argument: $arg"
            fi
        ;;
    esac
    shift # past argument with no value
done

# If there is a filter, then only test those projects
if [[ ${#filter[@]} -ge 1 ]] ; then
    unset TESTS_TO_RUN
    declare -A TESTS_TO_RUN
    for project in "${!filter[@]}"
    do
        TESTS_TO_RUN["$project"]="${filter[$project]}"
    done
fi

# ensure reports directory exists
mkdir -p $SCRIPT_DIR/UnitTestReports

# ensure coverage directory exists, remove any files from previous runs
mkdir -p $SCRIPT_DIR/CodeCoverage
rm -r $SCRIPT_DIR/CodeCoverage/*

echo Beginning unit testing: "${!TESTS_TO_RUN[@]}"

FAILURES_BUILD=0
FAILURES_RUN=0

function runtest()
{
  TEST_NAME=${1::-1}
 echo XML: $XML_OUTPUT
  # go into test directory
  cd $1 

  # create build directory (if necessary) and change to it
  mkdir -p build
  cd build

  #Remove gcda file to prevent coverage numbers from accumulating
  rm -f *.gcda

  #Build project, count the number of suites that fail to build
  qmake ../*.pro -r -spec linux-g++ CONFIG+=debug CONFIG+=declarative_debug
  make -j8 || ((FAILURES_BUILD++))
  
  #Find the executable amidst all the build files
  EXECUTABLE=$(find . -type f -executable -print)
  echo hello $EXECUTABLE
  #echo ${EXECUTABLE[@]}
  #Run the executable
  if [[ $XML_OUTPUT ]] ; then
    # save XML file in reports folder with directory name as filename
    $EXECUTABLE -xunitxml > $SCRIPT_DIR/UnitTestReports/$TEST_NAME.xml || ((FAILURES_RUN++))
  else
    $EXECUTABLE || ((FAILURES_RUN++))
  fi

  echo "************************************"
  echo Collecting Code Coverage Information
  echo "************************************"

  # generate .info file
  geninfo . -o "$SCRIPT_DIR/CodeCoverage/$TEST_NAME.info" || (
    $2 && rm * && cd $SCRIPT_DIR && runtest "$1" false
  )

  # back to script directory
  cd $SCRIPT_DIR
}
count=0

for TEST_NAME in ${!TESTS_TO_RUN[@]}; do
  ((count++))
  TEST=${TEST_NAME:-1}
  checkFailureRun=$FAILURES_RUN
  checkFailureBuild=$FAILURES_BUILD
  status="($count/${#TESTS_TO_RUN[@]}) Test Finished."
  echo "Running $TEST_NAME"
  runtest "$TEST_NAME" true
  if [ "$FAILURES_RUN" -gt "$checkFailureRun" ]; then
  	FAILED_RUN_TESTS[count-1]=$TEST
  fi

  if [ "$FAILURES_BUILD" -gt "$checkFailureBuild" ]; then
  	FAILED_BUILD_TESTS[count-1]=$TEST
  fi
done;

# combine info files into one tracefile
cd $SCRIPT_DIR/CodeCoverage
ls | grep .info | sed -e 's/^/-a\ /' | xargs lcov -o coverage_report.info

# remove unwanted files from our lcov report
# i.e. all moc files, h files, etc
lcov --remove coverage_report.info "moc_*" -o coverage_report.info
lcov --remove coverage_report.info "*.h" -o coverage_report.info
lcov --remove coverage_report.info "*.moc" -o coverage_report.info
lcov --remove coverage_report.info "/usr/include/*" -o coverage_report.info

# remove reuse code from the coverage info
lcov --remove coverage_report.info "COTS/PbreMpCommon/*" -o coverage_report.info
lcov --remove coverage_report.info "GOTS/FBCE/externs/src/QTUtils/*" -o coverage_report.info
lcov --remove coverage_report.info "GOTS/IpsugGS/*" -o coverage_report.info
lcov --remove coverage_report.info "GOTS/PbreMpCommon/*" -o coverage_report.info
lcov --remove coverage_report.info "GOTS/PowerDNA/*" -o coverage_report.info

# remove test console code from the coverage info
lcov --remove coverage_report.info "utilities/TestConsole/*" -o coverage_report.info

# remove test code from the coverage info
# i.e. mock classes and unit test suites
lcov --remove coverage_report.info "Testing/*" -o coverage_report.info

# generate html report
genhtml -o . coverage_report.info

cd $SCRIPT_DIR

# print whether testing succeeded or not
# return an error code if any tests failed to build
if [ "$FAILURES_BUILD" -eq 0 ] && [ "$FAILURES_RUN" -eq 0 ]; then
    echo "Unit testing completed."
    exit 0
else
    echo "Unit testing failed! Failed builds: $FAILURES_BUILD ${FAILED_BUILD_TESTS[@]}, failed runs: $FAILURES_RUN ${FAILED_RUN_TESTS[@]}"
    exit 1
fi
