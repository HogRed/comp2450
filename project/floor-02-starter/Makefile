CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET    = the_descent
SRCS      = main.cpp \
            bestiary/Bestiary.cpp bestiary/Search.cpp bestiary/Bench.cpp \
            hero/Hero.cpp hero/Sort.cpp hero/Bench.cpp
OBJS      = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJS)

.PHONY: run clean
