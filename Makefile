EXEC = teste

DIR_SOURCE = src
SOURCE = $(wildcard $(DIR_SOURCE)/*.cpp)

DIR_INCLUDE = inc
INCLUDE = $(wildcard $(DIR_INCLUDE)/*.h)

DIR_OBJ = obj
OBJS= $(addprefix $(DIR_OBJ)/,$(notdir $(SOURCE:.cpp=.o)))

LIBS = -lwiringPi -pthread
CFLAGS = -std=c++14 -c -Wall -W -Wextra -I $(DIR_INCLUDE)

CC = g++

all: $(EXEC)

debug: CFLAGS += -ggdb
debug: reset all

.PHONY: clean reset
clean:
	rm -f $(DIR_OBJ)/*.o
	rm -rf $(DIR_OBJ)
reset: clean
	rm -f $(EXEC)

run: reset all
	./teste


$(EXEC): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

$(DIR_OBJ)/main.o: $(DIR_SOURCE)/main.cpp
	@mkdir -p $(DIR_OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIR_OBJ)/%.o: $(DIR_SOURCE)/%.cpp $(DIR_INCLUDE)/%.h
	@mkdir -p $(DIR_OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)
