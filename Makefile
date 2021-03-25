CXX       := mpicxx
CXX_FLAGS := -Wall -Wextra -std=c++11 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include
LIB     :=
LIBRARIES   :=
EXECUTABLE  := main
MKDIR_P = mkdir -p
OUT_DIR = bin include libs

all: directories $(BIN)/$(EXECUTABLE)

.PHONY: directories

directories: ${OUT_DIR}

${OUT_DIR}:
		${MKDIR_P} ${OUT_DIR}

runpar: clean all
		mpirun ./$(BIN)/$(EXECUTABLE)

run: clean all
		./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
		$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -g3 $^ -o $@ $(LIBRARIES)

clean:
		-rm $(BIN)/*

