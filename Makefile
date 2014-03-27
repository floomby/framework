CC = x86_64-w64-mingw32-g++ -std=gnu++11
DLLFLAGS = -nostdlib -nostartfiles -fno-exceptions -Os -fomit-frame-pointer

SCORE = $(wildcard core/*.cpp) $(wildcard core/reflect/*.cpp) $(wildcard core/inject/*.cpp) $(wildcard core/shell/*.cpp) $(wildcard core/apiget/*.cpp) $(wildcard core/migration/*.cpp)
OCORE = $(notdir $(SCORE:.cpp=.o))

SLAUNCH = $(wildcard core/reflect/*.cpp) launch/test.cpp

SDROP = $(wildcard dropins/*.cpp)
ODROP = $(notdir $(SDROP:.cpp=.o))

STOOLS = $(wildcard tools/offer/*.cpp)

.PHONY: clean all

all: dll launch tools

dll: $(OCORE) $(ODROP)
	$(CC) -o test.dll $(OCORE) $(ODROP) -shared $(DLLFLAGS) \
		-Wl,--enable-auto-import \
		-Wl,--no-whole-archive -lkernel32 -lWs2_32 -luser32 -lmsvcrt \
		-Wl,--exclude-all-symbols \
		-Wl,-eDllMain -Wl,-Map=link/map.map

$(OCORE): $(SCORE)
	$(CC) $(DLLFLAGS) -c $(SCORE)

$(ODROP): $(SDROP)
	$(CC) $(DLLFLAGS) -c $(SDROP)


launch: $(SLAUNCH) launch/Makefile
	cd launch && make

tools: $(STOOLS) tools/offer/Makefile
	cd tools/offer && make

clean:
	rm -f test.dll link/map.map $(OCORE) $(ODROP) 2>/dev/null
	cd launch && make clean
	touch $(SLAUNCH)
	cd tools/offer && make clean
	touch $(STOOLS)
