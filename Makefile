CC = x86_64-w64-mingw32-g++ -std=c++11 -static-libstdc++ -static-libgcc -g -DNEED_META -DLOAD_DEPS -DFANCY_ERROR

SRCS = $(wildcard *.cpp) $(wildcard reflecter/*.cpp)
ASMS = $(wildcard *.s)
OBJS = $(notdir $(SRCS:.cpp=.o)) $(ASMS:.s=.o)

test: $(OBJS)
	$(CC) -o test $(OBJS)


$(OBJS): $(SRCS) $(ASMS)
	$(CC) -c $(SRCS) $(ASMS)

clean:
	rm $(OBJS) test.exe
