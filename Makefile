CC = x86_64-w64-mingw32-g++ -std=c++11 -static-libstdc++ -static-libgcc -g

SRCS = $(wildcard *.cpp)
ASMS = $(wildcard *.s)
OBJS = $(SRCS:.cpp=.o) $(ASMS:.s=.o)

test: $(OBJS)
	$(CC) -o test $(OBJS) reflecter/reflect.o apiget/apiget.o


$(OBJS): $(SRCS) $(ASMS)
	$(CC) -c $(SRCS) $(ASMS)

clean:
	rm $(OBJS) test.exe
