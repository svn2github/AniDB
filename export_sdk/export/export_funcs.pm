#!/usr/bin/perl

package export_funcs;

use strict;

$export_funcs::VERSION = "0.04";	#28.08.2009

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
	
	$str =~ s/\[i\]/\<i\>/g;
	$str =~ s/\[\/i\]/\<\/i\>/g;
	$str =~ s/\[b\]/\<b\>/g;
	$str =~ s/\[\/b\]/\<\/b\>/g;
	$str =~ s/\[u\]/\<u\>/g;
	$str =~ s/\[\/u\]/\<\/u\>/g;
	$str =~ s/\'/\&rsquo\;/g;
	$str =~ s/\"/\&quot\;/g;
	$str =~ s/\`/\&rsquo\;/g;
	$str =~ s/\r//g;
	$str =~ s/\n/\<br \/\>/g;

	return $str;
}

sub jsencodexml
{
    my $str = shift;
    
    $str =~ s/\[i\]/\<i\>/g;
    $str =~ s/\[\/i\]/\<\/i\>/g;
    $str =~ s/\[b\]/\<b\>/g;
    $str =~ s/\[\/b\]/\<\/b\>/g;
    $str =~ s/\[u\]/\<u\>/g;
    $str =~ s/\[\/u\]/\<\/u\>/g;
    $str =~ s/\‘/\&#8217\;/g;
    $str =~ s/\'/\&#0000039\;/g;
    $str =~ s/\"/\&#0000034\;/g;
    $str =~ s/\`/\&#8216\;/g;
    $str =~ s/\r//g;
    $str =~ s/\n/\<br \/\>/g;

    return $str;
}

sub jsencodexmlcs
{
    my $str = shift;    
	$str =~ s/\n//g;
	$str =~ s/\r//g;
	$str =~ s/\`/\'/g;
	if (index($str, '&amp;' ) < 0) { $str =~ s/\&/\&amp\;/g; }
	if (index($str, '&lt;'  ) < 0) { $str =~ s/\</\&lt\;/g; }
	if (index($str, '&gt;'  ) < 0) { $str =~ s/\>/\&gt\;/g; }
	if (index($str, '&apos;') < 0) { $str =~ s/\'/\&apos\;/g; }
	if (index($str, '&quot;') < 0) { $str =~ s/\"/\&quot\;/g; }
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
