QMAKE_CXXFLAGS_EXCEPTIONS_ON = -fexceptions
QMAKE_LFLAGS_EXCEPTIONS_ON   = -Wl
QMAKE_CFLAGS_RELEASE         = -O3
QMAKE_CXXFLAGS_RELEASE       = -O3
INCLUDEPATH  = . $$(BOOST_ROOT) simpleopt
CONFIG      += release static console
#CONFIG      -= rtti             # necessary evil for cryptopp
LIBS        += -lwsock32 -lws2_32 -lwinmm
LIBS        += -L"../lib" -lboost_system -lboost_filesystem -lboost_regex
DEFINES     +=
QT          -= core gui

TARGET  = adbr
DESTDIR = ../bin

SOURCES += adbr.cpp \
           adbr_util.cpp \
           adbr_hash.cpp \
           anidb_connection.cpp \
           inireader/ini_file.cpp \
           inireader/parser.cpp \
           cryptopp/*.cpp
