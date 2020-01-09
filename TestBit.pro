###################################################################### ##
##
## Developed for NASA Glenn Research Center
## By: Flight Software Branch (LSS)
##
## Project: Flow Boiling and Condensation Experiment (FBCE)
##
## Filename: TestBit.pro
## File Date: 20161220
##
## Authors ##
## Author: Jordan R. Higgins (JRH)
##
## Version and Traceability ##
## Subversion: @version $Id: TestBit.pro 1181 2017-08-01 21:04:21Z jrhiggi2 $
##
## Revision History:
##   <Date> <Name of Change Agent>
##   Description:
##     - Bulleted list of changes.
##
###################################################################### ##

QT       += testlib
QT       -= gui

TARGET = tst_testbit
CONFIG   += console test
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    $$PWD/../../mocks \
    $$PWD/../../mocks/BaseSharedMemory_A \
    $$PWD/../../mocks/BitTests \
    $$PWD/../../mocks/Commands \
    $$PWD/../../mocks/CommonThreadable_A \
    $$PWD/../../mocks/EcamsSharedMemory \
    $$PWD/../../mocks/HealthStatusLogger \
    $$PWD/../../../CSCI/ECAMS/ecamsmutable/Bit \
    $$PWD/../../../CSCI/ECAMS/includes \
    $$PWD/../../../CSCI/global/Common \
    $$PWD/../../../CSCI/global/includes \
    $$PWD/../../../CSCI/global/utils \
    $$PWD/../../../CSCI/global/utils/CommonUtils

SOURCES += \
    $$PWD/tst_testbit.cpp \
    $$PWD/../../../CSCI/ECAMS/ecamsmutable/Bit/bitimpl.cpp \
    $$PWD/../../../CSCI/global/utils/CommonUtils/commonutils.cpp

HEADERS += \
    $$PWD/../../mocks/BaseSharedMemory_A/basesharedmemory_a.h \
    $$PWD/../../mocks/BitTests/coriolisconfigurationtest.h \
    $$PWD/../../mocks/BitTests/corioliszerotest.h \
    $$PWD/../../mocks/BitTests/pbittwotest.h \
    $$PWD/../../mocks/Commands/commands.h \
    $$PWD/../../mocks/CommonThreadable_A/Threadable/threadable.h \
    $$PWD/../../mocks/DBus/dcamsPBitInterface.h \
    $$PWD/../../mocks/EcamsSharedMemory/ecamssharedmemory.h \
    $$PWD/../../mocks/HealthStatusLogger/healthstatuslogger.h \
    $$PWD/../../mocks/basemock.h \
    $$PWD/../../mocks/functionthread.h \
    $$PWD/../../mocks/mockbasesharedmemory_a.h \
    $$PWD/../../mocks/mockcommands.h \
    $$PWD/../../mocks/mockcommonthreadable_a.h \
    $$PWD/../../mocks/mockhealthstatuslogger.h \
    $$PWD/../../mocks/mocktest.h \
    $$PWD/../../mocks/BitTests/coriolisconfigurationtest.h \
    $$PWD/../../mocks/BitTests/coriolisnpfhflowtest.h \
    $$PWD/../../mocks/BitTests/corioliswaterflowtest.h \
    $$PWD/../../mocks/BitTests/corioliszerotest.h \
    $$PWD/../../mocks/BitTests/daqcubeconnectiontest.h \
    $$PWD/../../mocks/BitTests/dcamsconnectiontest.h \
    $$PWD/../../mocks/BitTests/degastest.h \
    $$PWD/../../mocks/BitTests/fbcainterfacetest.h \
    $$PWD/../../mocks/BitTests/ipsuconnectiontest.h \
    $$PWD/../../mocks/BitTests/pbittwotest.h \
    $$PWD/../../mocks/BitTests/testmoduleheatertest.h \
    $$PWD/../../mocks/EcamsSharedMemory/ecamssharedmemory.h \
    $$PWD/../../../CSCI/ECAMS/ecamsmutable/Bit/bitimpl.h \
    $$PWD/../../../CSCI/ECAMS/includes/bit.h

DEFINES += SRCDIR=\\\"$$PWD/\\\" \
           APPLICATION_LOGFILE_PATH=\\\"./\\\" \  ## The logfile location on disk.
           CONFIGURATION_FILE_PATH=\\\"./FBCE.ini\\\" \ ## The configuration location on disk.
           private=public protected=public

QMAKE_CXXFLAGS += -std=c++0x

#For Code Coverage Analysis:
QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0
LIBS += \
    -lgcov

include($$PWD/../../../GOTS/FBCE/externs/src/SAFE-DART/libsafedart/libsafedart.pro)
include($$PWD/../../../GOTS/FBCE/externs/src/QTUtils/QtUtils_Flight.pro)

DEFINES -= APPLICATION_LOGFILE_PATH=\\\"/home/FBCE_Logs/\\\"
