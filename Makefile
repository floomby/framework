CC = x86_64-w64-mingw32-gcc
CPP = x86_64-w64-mingw32-g++ -std=gnu++11
CFLAGS = -nostdlib -nostartfiles -Os -fomit-frame-pointer
DLLFLAGS = $(CFLAGS) -fno-exceptions

SCORE = $(wildcard core/*.cpp) $(wildcard core/reflect/*.cpp) $(wildcard core/inject/*.cpp) $(wildcard core/shell/*.cpp) $(wildcard core/apiget/*.cpp) $(wildcard core/migration/*.cpp)
OCORE = $(notdir $(SCORE:.cpp=.o))

SDROP = $(wildcard dropins/*.cpp)
ODROP = $(notdir $(SDROP:.cpp=.o))

SCOMMON = $(wildcard common/*.c)
OCOMMON = $(notdir $(SCOMMON:.c=.o))

.PHONY: clean all tools

all: dll tools

dll: $(OCORE) $(ODROP) $(OCOMMON) gcc.o
	mkdir -p link
	$(CPP) -o framework.dll $(OCORE) $(ODROP) -shared $(DLLFLAGS) $(OCOMMON) gcc.o \
		-Wl,--enable-auto-import \
		-Wl,--no-whole-archive -lkernel32 -lWs2_32 -luser32 -lmsvcrt -lCrypt32 -lShell32 \
		-Wl,--exclude-all-symbols \
		-Wl,-eDllMain -Wl,-Map=link/map.map

$(OCORE): $(SCORE) config.h
	$(CPP) $(DLLFLAGS) -c $(SCORE)

%.o: dropins/%.cpp config.h
	$(CPP) $(DLLFLAGS) -c -o $@ $<

$(OCOMMON): $(SCOMMON)
	$(CC) $(CFLAGS) -c $(SCOMMON)

%.o: gcc/%.s
	$(CC) -c -o $@ $<

launch: $(SLAUNCH) launch/Makefile
	cd launch && make

tools:
	cd tools/offer && make
	cd tools/sc-test && make

clean:
	rm -rf framework.dll link $(OCORE) $(ODROP) $(OCOMMON) gcc.o 2>/dev/null
	cd tools/offer && make clean
	cd tools/sc-test && make clean
