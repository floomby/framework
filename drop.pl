#!/usr/bin/perl

#use strict;
use warnings;

sub new_drop {
    print "enter the name\n";
    $_ = <>;
    chomp $_;
    print "creating $_\n";
    system "cp tools/templates/dropin.cpp dropins/$_.cpp";
    system "sed -i s/NAME/$_/ dropins/$_.cpp";
}

while(<>){
    chomp $_;
    if($_ eq "new"){
        new_drop();
    }elsif($_ eq "quit"){
        exit 0;
    }else{
        print "invalid action\n" ;
    }
}




