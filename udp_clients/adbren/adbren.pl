#!/usr/bin/perl -w
# api.anideb.info batch renamer.

# Copyright (C) 2006 clip9

# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

use strict;
use warnings;

use AniDB::UDPClient;
use File::Copy;
use Getopt::ArgvFile home=>1;
use Getopt::Long;

my $addmylist = 1;
my $norename  = 0;
my $noclean   = 0;
my $debug     = 0;
my $format    =
  "\%anime_name_english\%_\%episode\%\%version\%-\%group_short\%.\%filetype\%";
my $onlyhash = 0;
my $username;
my $password;

my $result = GetOptions(
    "addmylist" => \$addmylist,
    "norename"  => \$norename,
    "noclean"   => \$noclean,
    "debug"     => \$debug,
    "format=s"  => \$format,
    "onlyhash"  => \$onlyhash,
    "username=s"=> \$username,
    "password=s"=> \$password,
);

my @files;
while ( ( my $file = shift ) ) {
    push @files, $file;
}

if ( scalar @files < 1 ) {
    print_help();
    exit;
}

if ($onlyhash) {
    foreach my $file (@files) {
        my $ed2k = AniDB::UDPClient::ed2k_hash($file);
        my $size = -s $file;
        print "ed2k://|file|" . $file . "|" . $size . "|" . $ed2k . "|\n";
    }
    exit;
}

my $a = AniDB::UDPClient->new(
    username  => $username,
    password  => $password,
    client    => "adbren",
    clientver => "3",
    debug     => $debug,
);

if ( !$a->login ) {
    die("Not logged in!");
}

my $fileinfo;
my $newname;

foreach my $file (@files) {
    $fileinfo = $a->file($file);
    if ( !defined $fileinfo ) {
        next;
    }
    $newname = $format;
    if ( $fileinfo->{'anime_name_english'} eq "" ) {
        $fileinfo->{'anime_name_english'} = $fileinfo->{'anime_name_romaji'};
    }
    if ( $fileinfo->{'anime_name_romaji'} eq "" ) {
        $fileinfo->{'anime_name_romaji'} = $fileinfo->{'anime_name_english'};
    }
    while ( $newname =~ /\%([^\%]+)\%/ ) {
        my $key = $1;
        if ( defined $fileinfo->{$key} ) {
            if ( !$noclean ) {
                $fileinfo->{$key} =~ s/[^a-zA-Z0-9-]/_/g;
                $fileinfo->{$key} =~ s/[_]+/_/g;
            }
            $newname =~ s/\%$key\%/$fileinfo->{$key}/;
        }
        else {
            $newname =~ s/\%$key\%//;
        }
    }
    $newname =~ s/[_]+/_/g;
    $newname =~ s/_\./\./g;
    $newname =~ s/_-\./-/g;

    print $file. ": Renamed to " . $newname . "\n";
    if ($norename) {
    }
    else {
        if ( -e $newname ) {
            print $file. ": " . $newname
              . " Already exists. Refusing to overwrite\n";
        }
        else {
            move ( $file, $newname )
              or print( $file. ": Rename: " . $! . "\n" );
        }
    }
    if ($addmylist) {
        $a->mylistadd( $fileinfo );
    }
}

sub print_help {
    print qq/adbren.pl [options] <file1> [file2] ...
	Options:
	--format\tFormat. Default: 
	   \%anime_name_english\%_\%episode\%\%version\%-\%group_short\%.\%filetype\%
	--noclean\tDo not clean values of format vars. 
           (Don't remove spaces, etc.)
	--norename\tDo not rename files. Just print the new names.
	--addmylist\tAdd hashed files to mylist.
	--onlyhash\tOnly print ed2k hashes. 
	--debug\t\tDebug mode.

Format vars:
	\%fid\%, \%aid\%, \%eid\%, \%gid\%, \%lid\%, \%status\%, \%size\%, \%ed2k\%, 
	\%md5\%, \%sha1\%, \%crc32\%, \%lang_dub\%, \%lang_sub\%, \%quaility\%, \%source\%, 
	\%audio_codec\%, \%audio_bitrate\%, \%video_codec\%, \%video_bitrate\%,
	\%resolution\%, \%filetype\%, \%length\%, \%description\%, \%group\%, 
	\%group_short\%, \%episode\%, \%episode_name\%, \%episode_name_romaji\%,
	\%episode_name_kanii\%, \%episode_total\%, \%episode_last\%, \%anime_year\%,
	\%anime_type\%, \%anime_name_romaji\%, \%anime_name_kanji\%, 
	\%anime_name_english\%, \%anime_name_other\%, \%anime_name_short\%, 
	\%anime_synonyms\%, \%anime_category\%, \%version\%, \%censored\%
/;
    exit;
}
