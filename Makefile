CC = x86_64-w64-mingw32-gcc
CPP = x86_64-w64-mingw32-g++ -std=gnu++11
CFLAGS = -nostdlib -nostartfiles -Os -fomit-frame-pointer
DLLFLAGS = $(CFLAGS) -fno-exceptions


SCORE = $(wildcard core/*.cpp) $(wildcard core/reflect/*.cpp) $(wildcard core/inject/*.cpp) $(wildcard core/shell/*.cpp) $(wildcard core/apiget/*.cpp) $(wildcard core/migration/*.cpp)
OCORE = $(notdir $(SCORE:.cpp=.o))

SLAUNCH = $(wildcard core/reflect/*.cpp) launch/launch.cpp

SDROP = $(wildcard dropins/*.cpp)
ODROP = $(notdir $(SDROP:.cpp=.o))

STOOLS = $(wildcard tools/offer/*.cpp)

SCOMMON = $(wildcard common/*.c)
OCOMMON = $(notdir $(SCOMMON:.c=.o))

.PHONY: clean all

all: dll launch tools

dll: $(OCORE) $(ODROP) $(OCOMMON)
	$(CPP) -o test.dll $(OCORE) $(ODROP) -shared $(DLLFLAGS) $(OCOMMON) \
		-Wl,--enable-auto-import \
		-Wl,--no-whole-archive -lkernel32 -lWs2_32 -luser32 -lmsvcrt -lCrypt32 -lShell32 \
		-Wl,--exclude-all-symbols \
		-Wl,-eDllMain -Wl,-Map=link/map.map

$(OCORE): $(SCORE)
	$(CPP) $(DLLFLAGS) -c $(SCORE)

%.o: dropins/%.cpp
	$(CPP) $(DLLFLAGS) -c -o $@ $<

$(OCOMMON): $(SCOMMON)
	$(CC) $(CFLAGS) -c $(SCOMMON)

launch: $(SLAUNCH) launch/Makefile
	cd launch && make

tools: $(STOOLS) tools/offer/Makefile
	cd tools/offer && make

clean:
	rm -f test.dll link/map.map $(OCORE) $(ODROP) $(OCOMMON) 2>/dev/null
	cd launch && make clean
	touch $(SLAUNCH)
	cd tools/offer && make clean
	touch $(STOOLS)
