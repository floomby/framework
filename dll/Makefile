CC = x86_64-w64-mingw32-g++ -g -nostdlib -nostartfiles -std=c++11 -fno-exceptions -Os


SRCS = $(wildcard *.cpp) $(wildcard ../inject/*.cpp) $(wildcard ../reflecter/*.cpp) $(wildcard ../apiget/*.cpp)
OBJS = $(notdir $(SRCS:.cpp=.o))

dll: $(OBJS)
	$(CC) -o test.dll $(OBJS) -shared \
		-Wl,--enable-auto-import \
		-Wl,--no-whole-archive -lkernel32 \
		-Wl,--exclude-all-symbols \
		-Wl,--retain-symbols-file=link/exports \
		-Wl,-eDllMain -Wl,-Map=link/map.map

$(OBJS): $(SRCS)
	$(CC) $(FLAGS) -c $(SRCS)

clean:
	rm test.dll $(OBJS) link/map.map
