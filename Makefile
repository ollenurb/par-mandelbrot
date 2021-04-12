CXX       := mpicxx
CXX_FLAGS := -Wall -Wextra -std=c++11 -ggdb
BIN     	:= bin
SRC     	:= src
SOURCES 	:= $(wildcard $(SRC)/static/*.cpp) $(wildcard $(SRC)/dynamic/*.cpp) $(wildcard $(SRC)/utils/*.cpp)
INCLUDE 	:= include
LIBRARIES :=
STATIC_EXEC	 := static
DYNAMIC_EXEC := dynamic
MKDIR_P 		 := mkdir -p

all: directories $(BIN)/$(STATIC_EXEC)

.PHONY: directories

directories: $(BIN)

$(BIN):
		$(MKDIR_P) $(BIN)

runpar: clean all
		mpirun ./$(BIN)/$(STATIC_EXEC)

run: clean all
		./$(BIN)/$(STATIC_EXEC)

$(BIN)/$(STATIC_EXEC): $(SOURCES)
		$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -g3 $^ -o $@ $(LIBRARIES)

$(BIN)/$(DYNAMIC_EXEC): $(SRC)/*.cpp
		$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -g3 $^ -o $@ $(LIBRARIES)

clean:
		-rm $(BIN)/*

