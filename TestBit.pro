###################################################################### ##
## Filename: TestBit.pro
## Author: Matthew Mendoza (MM)
###################################################################### ##

QT       += testlib
QT       -= gui

TARGET = tst_testbit
CONFIG   += console test
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    $$PWD/../../mocks \
    $$PWD/../../mocks/BitTests \
    $$PWD/../../mocks/HealthStatusLogger \

SOURCES += \
    $$PWD/tst_testbit.cpp \

HEADERS += \
    $$PWD/../../mocks/BaseSharedMemory_A/basesharedmemory_a.h \
    $$PWD/../../mocks/BitTests/coriolisconfigurationtest.h \
    $$PWD/../../mocks/mockhealthstatuslogger.h \
    

DEFINES += SRCDIR=\\\"$$PWD/\\\" \
           APPLICATION_LOGFILE_PATH=\\\"./\\\" \  ## The logfile location on disk.
           private=public protected=public

QMAKE_CXXFLAGS += -std=c++0x

#For Code Coverage Analysis:
QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0
LIBS += \
    -lgcov
