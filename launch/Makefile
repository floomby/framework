CC = x86_64-w64-mingw32-g++ -std=c++11 -static-libstdc++ -static-libgcc -g

# this makefile shouldn't exist (but I am bad a Makefiles)

SLAUNCH = $(wildcard ../core/reflect/*.cpp) test.cpp
OLAUNCH = $(notdir $(SLAUNCH:.cpp=.o))

launch: $(OLAUNCH)
	$(CC) -o launch $(OLAUNCH)
	mv launch.exe ../

$(OLAUNCH): $(SLAUNCH)
	$(CC) -c $(SLAUNCH)

clean:
	rm -f $(OLAUNCH) ../launch.exe 2>/dev/null
