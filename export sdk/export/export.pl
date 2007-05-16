#!/usr/bin/perl
# export.pl - part of the AnimeReactor Anime DB CGI
# (C) 2003 by EXP, all rights reserved!
#
#   THIS PROGRAM IS PROVIDED AS IS AND WITHOUT ANY WARRANTY.
#   USE AT YOUR OWN RISK!

use strict;

#required modules
use export;
use export_utils;
use export_funcs;
use HTML::Template::Expr;

$main::VERSION = "0.02";		#12.08.2003

##INIT:
$| = 1;
$main::themedir = "themes";
$main::outdir = "out";
##

#this is needed to log errors
$SIG{__DIE__} = sub {
		print STDERR "ERROR: @_\n";
                exit 0;
};

#decide what to do
if (!$ARGV[0])
{
    die "usage: $0 <theme>\n";
}

#check theme
if (!&export_utils::valid_theme($ARGV[0]))
{
    die "invalid theme: $ARGV[0]\n";
}

#now start work
my $t = time();
print "Parsing template[".$ARGV[0]."]:\n";
&export::do_export($ARGV[0]);
print "Done (after ".(time()-$t)." seconds).\n";
