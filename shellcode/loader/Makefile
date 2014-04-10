loader: reflect.o loader.cpp
	x86_64-w64-mingw32-g++.exe -std=c++11 loader.cpp reflect.o -o loader -static

reflect.o: reflect/reflect.cpp reflect/reflect.h reflect/apiget.h
	x86_64-w64-mingw32-g++.exe reflect/reflect.cpp -c -std=c++11 -nostartfiles -fno-exceptions -Os -fomit-frame-pointer

clean:
	rm -f loader.exe reflect.o 2>/dev/null
