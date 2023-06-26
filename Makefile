INC_DIRS = -I ../quickfix/include
QFLIB = /usr/local/lib/libquickfix.so
CC_ARGS = -g -std=c++0x -DDEBUG $(INC_DIRS)
SOURCES=FIXAdapter.cpp CSGuardian.cpp FIXOrder.cpp FIXOrderCollection.cpp
SOURCES+=TimeObject