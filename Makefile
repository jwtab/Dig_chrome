
uname_s := $(shell uname -s)

GXX := c++

TARGET := ./bin/dig

INC := -I ./inc 
LIBS := -L./bin  -lsqlite3

CPPFLAGS := -Wall -std=c++11

SRC_DIR := src
OBJ_DIR := ./objs

SRC := $(wildcard ${SRC_DIR}/*.cpp)
OBJ := $(patsubst %.cpp, ${OBJ_DIR}/%.o, $(notdir ${SRC}))

all:exe

debug:CPPFLAGS += -g
debug:exe

exe:${OBJ}
	$(GXX) -o ${TARGET} ${OBJ} $(LIBS) 

${OBJ_DIR}/%.o:${SRC_DIR}/%.cpp
	test -d ${OBJ_DIR} || mkdir -p ${OBJ_DIR}
	@echo Compiling $< ...
	$(GXX) $(CPPFLAGS) -o $@ -c $< $(INC) 

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${TARGET}

sqlite3:
	gcc src/sqlite3.c -lpthread -ldl -fPIC -shared -o ./bin/libsqlite3.so 
