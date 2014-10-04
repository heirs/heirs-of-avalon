SRC_DIR = source
OBJ_DIR = object
LUA_LIB = lua5.2

CC		= gcc
DEBUG		= -g
CFLAGS		= -W -Wall -I.. -pthread -g
LUACFLAGS	= $(shell pkg-config --cflags $(LUA_LIB) )
LUALFLAGS	= $(shell pkg-config --libs $(LUA_LIB) )
LFLAGS		= -L./$(OBJ_DIR)
LINK		= $(CC)

TARGET = heirs
EXT = c
SRCS = $(shell find $(SRC_DIR) -type f -name '*.$(EXT)')
OBJS = $(SRCS:$(SRC_DIR)/%.$(EXT)=$(OBJ_DIR)/%.o)

.PHONY: all clean
.SUFFIXES: .o .cc

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	$(CC) $(CFLAGS) $(LUACFLAGS) $(DEBUG) -o $@ -c $<

all: $(TARGET)
	$(LINK) -o $(TARGET) $(OBJS) $(LFLAGS) $(LUALFLAGS)

$(TARGET): $(OBJS)

clean:
	rm -f $(OBJ_DIR)/*.o 2> /dev/null
	rm -f $(TARGET) 2> /dev/null
