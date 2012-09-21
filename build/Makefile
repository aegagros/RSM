# Basic variables
CC=g++
CXXFLAGS=-g -O2 -Wall

# Working folders
#SRCDIR=../src
HEADERDIR=../include

#Files
OBJS=
HEADERS=

# Link libraries
LIBS=

# Target
#TARGET=../bin/basev8test

$(TARGET): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: $(SRCDIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $<

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
