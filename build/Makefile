CILK_COMPILER = /Users/alex/opt/OpenCilk-2.0.0-x86_64-Apple-Darwin/bin/clang++
CILK_FLAGS = -fopencilk -I /usr/local/include/eigen3/Eigen -g
SRC = ../main.cpp ../mult.cpp
OBJ = $(notdir $(SRC:.cpp=.o))
TARGET = mm

all: $(TARGET)

$(TARGET): $(OBJ)
	xcrun $(CILK_COMPILER) $(CILK_FLAGS) -o $@ $^

%.o: ../%.cpp ../mult.h
	xcrun $(CILK_COMPILER) $(CILK_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
