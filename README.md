Framework
=========

A framework for writing custom 64-bit Win32 payloads
in the form of a dll.

The framework provides some userspace tools for
the payloads:

 * Reflective loading
 * Process migration
 * Reverse socket conections
 * Patching DLLs and EXEs on remote host

Building
--------

You will need:

 * Cygwin (64 bit) -- make, mingw64-x86_64-gcc-g++

    ```
    git clone https://github.com/floomby/framework.git
    cd framework
    make
    ```
    
Usage
-----

    ```
    (./launch.exe &) && nc -l 1234
    
    ```
