#!/usr/bin/python

import sys

def ror( dword, bits ):
  return ( dword >> bits | dword << ( 32 - bits ) ) & 0xFFFFFFFF

def my_unicode( string, uppercase=True ):
  result = "";
  if uppercase:
    string = string.upper()
  for c in string:
    result += c + "\x00"
  return result


def hash( module, function, bits=13 ):
  module_hash = 0
  function_hash = 0
  for c in my_unicode( module + "\x00" ):
    module_hash  = ror( module_hash, bits )
    module_hash += ord( c )
  for c in str( function + "\x00" ):
    function_hash  = ror( function_hash, bits )
    function_hash += ord( c )
  h = module_hash + function_hash & 0xFFFFFFFF
  return h

if __name__ == "__main__":
  argv = sys.argv
  print "0x%08X" % hash(argv[1], argv[2])
