CC = x86_64-w64-mingw32-g++ -std=c++11 -static-libstdc++ -static-libgcc -g

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

readpe: $(OBJS)
	$(CC) -o readpe $(OBJS)


$(OBJS): $(SRCS)
	$(CC) -c $(SRCS)

clean:
	rm $(OBJS)
