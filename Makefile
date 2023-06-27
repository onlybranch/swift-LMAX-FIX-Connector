INC_DIRS = -I ../quickfix/include
QFLIB = /usr/local/lib/libquickfix.so
CC_ARGS = -g -std=c++0x -DDEBUG $(INC_DIRS)
SOURCES=FIXAdapter.cpp CSGuardian.cpp FIXOrder.cpp FIXOrderCollection.cpp
SOURCES+=TimeObject.cpp Platform.cpp CurrencyCollection.cpp
OBJECTS=$(SOURCES:.cpp=.o)
HEADERS=$(SOURCES:.cpp=.h)
EXE=app

all: $(SOURCES) $(HEADERS) $(EXE)

release: CC_ARGS = -O3 -std=