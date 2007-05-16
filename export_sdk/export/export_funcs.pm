#!/usr/bin/perl

package export_funcs;

use strict;

$export_funcs::VERSION = "0.02";	#26.08.2003

##INIT
##

#SUBs

sub jsencode
{
    my $str = shift;
    
    $str =~ s/\'/\\\'/g;
    $str =~ s/\"/\\\"/g;
    $str =~ s/\`/\\\`/g;
    $str =~ s/\r//g;
    $str =~ s/\n//g;

    return $str;
}

sub jsencodehtml
{
    my $str = shift;
    
    $str =~ s/\'/\&rsquo\;/g;
    $str =~ s/\"/\&quot\;/g;
    $str =~ s/\`/\&rsquo\;/g; #\`
    $str =~ s/\r//g;
    $str =~ s/\n/\<br\>/g;

    return $str;
}

#replace('a','b','abcdef');
sub replace
{
    my $in = $_[0];
    my $out = $_[1];
    my $str = $_[2];
    
    $str =~ s/$in/$out/g;
    
    return $str;
}

sub ireplace
{
    my $in = $_[0];
    my $out = $_[1];
    my $str = $_[2];
    
    $str =~ s/$in/$out/gi;
    
    return $str;
}

1;
