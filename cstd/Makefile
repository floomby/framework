CC = x86_64-w64-mingw32-g++ -g -nostdlib -nostartfiles


SRCS = $(wildcard *.cpp)
ASMS = $(wildcard *.s)
OBJS = $(SRCS:.cpp=.o) $(ASMS:.s=.o)


all: $(OBJS)

$(OBJS): $(SRCS) $(ASMS)
	$(CC) $(FLAGS) -c $(SRCS) $(ASMS)

clean:
	rm test.dll *.o
