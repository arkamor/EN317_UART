# Define directories

SRCDIR		= src
OBJDIR		= obj
BINDIR		= bin

# Library targets

SYSTEMC_INSTALL_PATH ?= $(HOME)/systemc-2.3.4

INCD 		= -I$(SYSTEMC_INSTALL_PATH)/include
LIBS 		= -L$(SYSTEMC_INSTALL_PATH)/lib-linux64

LSC			= -lsystemc

# main targets

MN_SRC	= $(SRCDIR)/main.cpp

MN_OBJ		= $(OBJDIR)/server.o
MN_OUT		= $(BINDIR)/server

# Common targets

OBJS	= $(CLT_OBJ) $(SRV_OBJ) $(LIB_OBJ)
OUT		= $(CLT_OUT) $(SRV_OUT)

# Compiler options & flags

CC 		= gcc

MKDIR_P = mkdir -p

FLAGS	= -g -c -Wall

# Target rules

all: clean main run

force: r_dir $(CLT_OUT) $(SRV_OUT)

# Folder tree creation

dir: ${OBJDIR} ${BINDIR}

${OBJDIR}:
	${MKDIR_P} ${OBJDIR}

${BINDIR}:
	${MKDIR_P} ${BINDIR}

# RELEASE COMPILING

$(CLT_OUT): $(CLT_OBJ) $(LIB_OBJ_C) $(LFLAGS)
	$(CC) -g  $(LIB_OBJ_C) $(CLT_OBJ) -o $(CLT_OUT)
	
$(CLT_OBJ): $(CLT_SRC)
	$(CC) $(FLAGS) $(CLT_SRC) -o $(CLT_OBJ)


# Clean rule

clean:
	rm -rf bin obj a.out

main:
	g++ -I/home/user/systemc-2.3.4/include -L/home/user/systemc-2.3.4/lib-linux64 -lsystemc src/main.cpp

tst:
	$(CC) $(INCD) $(LIBS) $(LSC) src/main.cpp

run:
	LD_LIBRARY_PATH=$(HOME)/systemc-2.3.4/lib-linux64/ ./a.out

bouiboui: clean
	g++ -I/home/user/systemc-2.3.4/include -L/home/user/systemc-2.3.4/lib-linux64 -lsystemc src/Uart.cpp src/Testbench.cpp src/main.cpp

# LD_LIBRARY_PATH=$HOME/systemc-2.3.4/lib-linux64/ ./a.out
