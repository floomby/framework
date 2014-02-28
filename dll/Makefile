CC = x86_64-w64-mingw32-g++ -nostdlib -nostartfiles -std=gnu++11 -fno-exceptions -Os -Wno-pmf-conversion


SRCS = $(wildcard *.cpp) $(wildcard ../inject/*.cpp) $(wildcard ../reflecter/*.cpp) $(wildcard ../apiget/*.cpp) $(wildcard ../shell/*.cpp)
OBJS = $(notdir $(SRCS:.cpp=.o))

dll: $(OBJS)
	$(CC) -o test.dll $(OBJS) -shared \
		-Wl,--enable-auto-import \
		-Wl,--no-whole-archive -lkernel32 -lWs2_32 -luser32 -lmsvcrt \
		-Wl,--exclude-all-symbols \
		-Wl,-eDllMain -Wl,-Map=link/map.map

$(OBJS): $(SRCS)
	$(CC) $(FLAGS) -c $(SRCS)

clean:
	rm test.dll $(OBJS) link/map.map
