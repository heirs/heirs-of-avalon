SRC_DIR = source
OBJ_DIR = object
LIBS = lua5.2 sdl2 SDL2_image gl

CC		= gcc
DEBUG		= -g
CFLAGS		= -W -Wall -I.. -pthread -g -I/usr/include/GL/
LIBCFLAGS	= $(shell pkg-config --cflags $(LIBS) )
LIBLFLAGS	= $(shell pkg-config --libs $(LIBS) )
LFLAGS		= -L./$(OBJ_DIR)
LINK		= $(CC)

TARGET = heirs
EXT = c
SRCS = $(shell find $(SRC_DIR) -type f -name '*.$(EXT)')
OBJS = $(SRCS:$(SRC_DIR)/%.$(EXT)=$(OBJ_DIR)/%.o)

.PHONY: all clean
.SUFFIXES: .o .cc

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	$(CC) $(CFLAGS) $(LIBCFLAGS) $(DEBUG) -o $@ -c $<

all: $(TARGET)
	$(LINK) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBLFLAGS)

$(TARGET): $(OBJS)

clean:
	rm -f $(shell find object/ -type f -name '*.o') 2> /dev/null
	rm -f $(TARGET) 2> /dev/null
