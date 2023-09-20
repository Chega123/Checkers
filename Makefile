TARGET = program

CXX = g++

CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lGL -lGLU -lglut

SRCS = main.cpp Board.cpp Checker.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
