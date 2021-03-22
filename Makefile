CXX       := mpic++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include
LIB     :=
LIBRARIES   :=
EXECUTABLE  := main

all: $(BIN)/$(EXECUTABLE)

runpar: clean all
		mpirun ./$(BIN)/$(EXECUTABLE)

run: clean all
		./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
		$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -g3 $^ -o $@ $(LIBRARIES)

clean:
		-rm $(BIN)/*

