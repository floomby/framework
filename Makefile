CC = x86_64-w64-mingw32-g++ -std=gnu++11
DLLFLAGS = -nostdlib -nostartfiles -fno-exceptions -Os


SRCS = $(wildcard *.cpp) $(wildcard ../inject/*.cpp) $(wildcard ../reflecter/*.cpp) $(wildcard ../apiget/*.cpp) $(wildcard ../shell/*.cpp)
OBJS = $(notdir $(SRCS:.cpp=.o))

SCORE = $(wildcard core/*.cpp) $(wildcard core/reflect/*.cpp) $(wildcard core/inject/*.cpp) $(wildcard core/shell/*.cpp)
OCORE = $(notdir $(SCORE:.cpp=.o))

SLAUNCH = $(wildcard core/reflect/*.cpp) launch/test.cpp

dll: core
	$(CC) -o test.dll $(OCORE) -shared $(DLLFLAGS) \
		-Wl,--enable-auto-import \
		-Wl,--no-whole-archive -lkernel32 -lWs2_32 -luser32 -lmsvcrt \
		-Wl,--exclude-all-symbols \
		-Wl,-eDllMain -Wl,-Map=link/map.map

core: $(SCORE)
	$(CC) $(DLLFLAGS) -c $(SCORE)

#dropins:

$(OBJS): $(SRCS)
	$(CC) $(FLAGS) -c $(SRCS)


launch: $(SLAUNCH) launch/Makefile
	cd launch && make

clean:
	rm -f test.dll $(OBJS) link/map.map $(OCORE) 2>/dev/null
	cd launch && make clean
