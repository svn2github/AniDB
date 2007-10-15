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

use constant USERNAME => 'username';
use constant PASSWORD => 'password';

use strict;
use warnings;

use File::Copy;
use Getopt::Long;

my $addmylist = 1;
my $norename  = 0;
my $noclean   = 0;
my $debug     = 0;
my $format    =
  "\%anime_name_english\%_\%episode\%\%version\%-\%group_short\%.\%filetype\%";
my $onlyhash = 0;

my $result = GetOptions(
    "addmylist" => \$addmylist,
    "norename"  => \$norename,
    "noclean"   => \$noclean,
    "debug"     => \$debug,
    "format=s"  => \$format,
    "onlyhash"  => \$onlyhash,
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
    username  => USERNAME,
    password  => PASSWORD,
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
        $a->mylistadd( $fileinfo->{fid} );
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
	
Remember to edit this file to change anidb username and password./;
    exit;
}

package AniDB::UDPClient;
use strict;
no strict 'refs';
use warnings;
use IO::Socket;
use Digest::MD4;
use File::Spec;
use Data::Dumper;

#The documentation says it's 2 seconds but that seems to be way to small.
use constant MIN_INTERVAL => 5;

#acodes:
use constant GROUP_NAME          => 0x00000001;
use constant GROUP_NAME_SHORT    => 0x00000002;
use constant EPISODE_NUMBER      => 0x00000100;
use constant EPISODE_NAME        => 0x00000200;
use constant EPISODE_NAME_ROMAJI => 0x00000400;
use constant EPISODE_NAME_KANII  => 0x00000800;
use constant EPISODE_TOTAL       => 0x00010000;
use constant EPISODE_LAST        => 0x00020000;
use constant ANIME_YEAR          => 0x00040000;
use constant ANIME_TYPE          => 0x00080000;
use constant ANIME_NAME_ROMAJI   => 0x00100000;
use constant ANIME_NAME_KANJI    => 0x00200000;
use constant ANIME_NAME_ENGLISH  => 0x00400000;
use constant ANIME_NAME_OTHER    => 0x00800000;
use constant ANIME_NAME_SHORT    => 0x01000000;
use constant ANIME_SYNONYMS      => 0x02000000;
use constant ANIME_CATAGORY      => 0x04000000;

#fcodes:
use constant AID           => 0x00000002;
use constant EID           => 0x00000004;
use constant GID           => 0x00000008;
use constant LID           => 0x00000010;
use constant STATUS        => 0x00000100;
use constant SIZE          => 0x00000200;
use constant ED2K          => 0x00000400;
use constant MD5           => 0x00000800;
use constant SHA1          => 0x00001000;
use constant CRC32         => 0x00002000;
use constant LANG_DUB      => 0x00010000;
use constant LANG_SUB      => 0x00020000;
use constant QUALITY       => 0x00040000;
use constant SOURCE        => 0x00080000;
use constant CODEC_AUDIO   => 0x00100000;
use constant BITRATE_AUDIO => 0x00200000;
use constant CODEC_VIDEO   => 0x00400000;
use constant BITRATE_VIDEO => 0x00800000;
use constant RESOLUTION    => 0x01000000;
use constant FILETYPE      => 0x02000000;
use constant LENGTH        => 0x04000000;
use constant DESCRIPTION   => 0x08000000;

#Status Codes
use constant STATUS_CRCOK  => 0x01;
use constant STATUS_CRCERR => 0x02;
use constant STATUS_ISV2   => 0x04;
use constant STATUS_ISV3   => 0x08;
use constant STATUS_ISV4   => 0x10;
use constant STATUS_ISV5   => 0x20;
use constant STATUS_UNC    => 0x40;
use constant STATUS_CEN    => 0x80;

use constant FILE_ENUM => qw/fid aid eid gid lid status_code size ed2k md5 sha1
  crc32 lang_dub lang_sub quaility source audio_codec audio_bitrate video_codec
  video_bitrate resolution filetype length description group group_short
  episode episode_name episode_name_romaji episode_name_kanii episode_total
  episode_last anime_year anime_type anime_name_romaji anime_name_kanji
  anime_name_english anime_name_other anime_name_short anime_synonyms
  anime_category/;

use constant ANIME_ENUM => qw/aid episodes episode_count special rating
  votes tmprating tmpvotes review_rating reviews year type romaji kanji
  english other short_name synonyms category/;

use subs 'debug';

sub new {
    my $class = shift;
    my $self  = bless {}, $class;

    $self->{hostname} = "api.anidb.info";
    $self->{port}     = 9000;
    $self->{handle}   = IO::Socket::INET->new( Proto => 'udp' ) or die($!);
    $self->{ipaddr}   = gethostbyname( $self->{hostname} )
      or die( "Gethostbyname(" . $self->{hostname} . "):" . $! );
    $self->{sockaddr} = sockaddr_in( $self->{port}, $self->{ipaddr} )
      or die($!);
    $self->{last_command} = 0;
    my %args = @_;

    foreach my $key ( keys %args ) {
        $self->{$key} = $args{$key};
    }
    defined $self->{username}  or die "Username not defined!\n";
    defined $self->{password}  or die "Password not defined!\n";
    defined $self->{client}    or die "Client not defined!\n";
    defined $self->{clientver} or die "Clientver not defined!\n";
    $self->{state_file} =
      File::Spec->catfile( File::Spec->tmpdir(), "adbren_state.tmp" );
    if ( $self->{debug} ) {
        *debug = sub {
            if (@_) {
                foreach my $x (@_) {
                    if ( ref $x eq 'ARRAY' || ref $x eq 'HASH' ) {
                        print STDERR Dumper($x);
                    }
                    else {
                        my $clean = $x;
                        $clean =~ s/[\n\r]*$//;
                        print STDERR $clean . " ";
                    }
                }
                print STDERR "\n";
            }
          }
    }
    else {
        *debug = sub { };
    }
    debug "State File: ", $self->{state_file};
    $self->load_state();
    return $self;
}

sub load_state {
    my ($self) = @_;
    open F, "<", $self->{state_file}
      or debug debug $self->{state_file}, ":", $!;
    if (*F) {
        while (<F>) {
            if (/(.*)==(.*)/) {
                $self->{$1} = $2;
            }
        }
        close F;
        return 0;
    }
    $self->{last_command} = 0;
    return 0;
}

sub save_state {
    my ($self) = @_;
    open F, ">", $self->{state_file} or debug $self->{state_file}, ":", $!;
    if ( defined $self->{skey} && *F ) {
        debug "Writing new state file skey=", $self->{skey};
        print F "skey==" . $self->{skey} . "\n";
        print F "last_command==" . time . "\n";
        close F;
    }
    chmod 0666, $self->{state_file};
}

sub anime {
    my ( $self, $anime ) = @_;
    my $msg = "ANIME s=" . $self->{skey};
    if ( $anime =~ /\d+/ ) {
        $msg .= "&aid=" . $anime . "\n";
    }
    else {
        $msg .= "&aname=" . $anime . "\n";
    }
    $msg = $self->_sendrecv($msg);
    $msg =~ s/.*\n//im;
    my @f = split /\|/, $msg;
    my %animeinfo;
    if ( scalar @f > 0 ) {
        for ( my $i = 0 ; $i < ( scalar @f ) ; $i++ ) {
            $animeinfo{ (ANIME_ENUM)[$i] } = $f[$i];
        }
    }
    else {
        return undef;
    }
    debug "Animeinfo Hash:", \%animeinfo;
    return \%animeinfo;
}

sub file {
    my ( $self, $file ) = @_;
    my $msg = "FILE s=" . $self->{skey} . "";
    if ( -e $file ) {
        print $file. ": Hashing\n";
        my $ed2k = ed2k_hash($file);
        my $size = -s $file;
        $msg .= "&size=" . $size . "&ed2k=" . $ed2k . "";
    }
    else {
        $msg .= "&fid=" . $file;
    }

    # the reason i'm not using -1 is that the api might change to include other
    # fields so the the ENUM might change.
    my $acode =
      GROUP_NAME | GROUP_NAME_SHORT | EPISODE_NUMBER | EPISODE_NAME |
      EPISODE_NAME_ROMAJI | EPISODE_NAME_KANII | EPISODE_TOTAL | EPISODE_LAST |
      ANIME_YEAR | ANIME_TYPE | ANIME_NAME_ROMAJI | ANIME_NAME_KANJI |
      ANIME_NAME_ENGLISH | ANIME_NAME_OTHER | ANIME_NAME_SHORT |
      ANIME_SYNONYMS | ANIME_CATAGORY;
    my $fcode =
      AID | EID | GID | LID | STATUS | SIZE | ED2K | MD5 | SHA1 | CRC32 |
      LANG_DUB | LANG_SUB | QUALITY | SOURCE | CODEC_AUDIO | BITRATE_AUDIO |
      CODEC_VIDEO | BITRATE_VIDEO | RESOLUTION | FILETYPE | LENGTH |
      DESCRIPTION;
    $msg .= "&acode=" . ($acode) . "&fcode=" . ($fcode) . "\n";
    print $file. ": Getting info.\n";
    $msg = $self->_sendrecv($msg);
    $msg =~ s/.*\n//im;
    my @f = split /\|/, $msg;
    my %fileinfo;

    if ( scalar @f > 0 ) {
        for ( my $i = 0 ; $i < ( scalar @f ) ; $i++ ) {
            $fileinfo{ (FILE_ENUM)[$i] } = $f[$i];
        }
        $fileinfo{anime_name_short} =~ s/'/,/g;
        $fileinfo{anime_synonyms}   =~ s/'/,/g;
        $fileinfo{censored} = "cen" if ( $fileinfo{status_code} & STATUS_CEN );
        $fileinfo{censored} = "unc" if ( $fileinfo{status_code} & STATUS_UNC );
        if ( $fileinfo{status_code} & STATUS_ISV2 ) {
            $fileinfo{version} = "v2";
        }
        elsif ( $fileinfo{status_code} & STATUS_ISV3 ) {
            $fileinfo{version} = "v3";
        }
        elsif ( $fileinfo{status_code} & STATUS_ISV4 ) {
            $fileinfo{version} = "v4";
        }
        elsif ( $fileinfo{status_code} & STATUS_ISV5 ) {
            $fileinfo{version} = "v5";
        }
        $fileinfo{crcok}  = $fileinfo{status_code} & STATUS_CRCOK;
        $fileinfo{crcerr} = $fileinfo{status_code} & STATUS_CRCERR;
        return \%fileinfo;
    }
    return undef;
}

sub mylistadd {
    my ( $self, $file ) = @_;
    my $msg = "MYLISTADD s=" . $self->{skey};
    if ( -e $file ) {
        print $file. ": Hashing.\n";
        my $ed2k = ed2k_hash($file);
        my $size = -s $file;
        $msg .= "&size=" . $size . "&ed2k=" . $ed2k . "";
    }
    else {
        $msg .= "&fid=" . $file;
    }
    $msg = $self->_sendrecv($msg);
    if ( $msg =~ /^2.*/ ) {
        print $file. ": Added to mylist.\n";
    }
    else {
        return undef;
    }
    return 1;
}

sub login {
    my ($self) = @_;
    my $msg = "";
    if ( !defined $self->{skey}
        || ( time - $self->{last_command} ) > ( 35 * 60 ) )
    {
        $msg =
            "AUTH user="
          . $self->{username}
          . "&pass="
          . $self->{password}
          . "&protover=3&client="
          . $self->{client}
          . "&clientver="
          . $self->{clientver};
        $msg .= "&nat=1";
        $msg .= "\n";
        $msg = $self->_sendrecv($msg);
        if ( defined $msg && $msg =~ /20[0|1]\ ([a-zA-Z0-9]*)\ ([0-9\.\:]).*/ )
        {
            $self->{skey}   = $1;
            $self->{myaddr} = $2;
        }
        else {
            die "Login Failed: $msg\n";
        }
        $self->save_state();
        return 1;
    }

    return 1;
}

sub logout {
    my ($self) = @_;
    if ( $self->{skey} ) {
        my $msg = "LOGOUT s=" . $self->{skey} . "\n";
        return $self->_sendrecv($msg);
    }
    else {
        return 0;
    }
}

sub notify {
    my ($self) = @_;
    my $msg;
    $msg = "NOTIFY s=" . $self->{skey} . "\n";
    $self->_send($msg);
    return $self->_recv();
}

sub ping {
    my ($self) = @_;
    $self->_send("PING\n");
}

# Sends and reads the reply. Tries up to 5 times.
sub _sendrecv {
    my ( $self, $msg ) = @_;
    my $stat = 0;
    while ( ( time - $self->{last_command} ) < MIN_INTERVAL ) {
        $stat = MIN_INTERVAL - ( time - $self->{last_command} );
        sleep($stat);
        debug "Delay: $stat\n";
    }
    if ( $msg =~ /\n$/ ) {
    }
    else {
        $msg .= "\n";
    }
    send( $self->{handle}, $msg, 0, $self->{sockaddr} ) or die( "Send: " . $! );
    $self->{last_command} = time;
    debug "-->", $msg;
    my $recvmsg;
    my $timer = 0;
    while ( !( $recvmsg = $self->_recv() ) ) {
        if ( $timer > 10 ) {
            print "Timeout while wating for reply.\n";
            return undef;
        }
        $timer++;
        debug "-->", $msg;
        send( $self->{handle}, $msg, 0, $self->{sockaddr} )
          or die( "Send: " . $! );
    }
    if ( $recvmsg =~ /^501.*|^506.*/ ) {
        debug "Invalid session. Reauthing.";
        my $oldskey = $self->{skey};
        undef $self->{skey};
        $self->login();
        $msg =~ s/s=$oldskey/s=$self->{skey}/;
        return $self->_sendrecv($msg);
    }
    debug "<--", $recvmsg;
    return $recvmsg;
}

sub _send {
    my ( $self, $msg ) = @_;
    send( $self->{handle}, $msg, 0, $self->{sockaddr} ) or die( "Send: " . $! );
    debug "-->", $msg;
}

sub _recv {
    my ($self) = @_;
    my $rin = '';
    my $rout;
    vec( $rin, fileno( $self->{handle} ), 1 ) = 1;
    if ( select( $rout = $rin, undef, undef, 10.0 ) ) {
        my $msg;
        recv( $self->{handle}, $msg, 1500, 0 ) or die( "Recv:" . $! );
        return $msg;
    }
    return undef;
}

sub ed2k_hash {
    my ($file) = @_;
    my $ctx    = Digest::MD4->new;
    my $ctx2   = Digest::MD4->new;
    my $buffer;
    open my $handle, "<", $file or die $!;
    binmode $handle;

    my $block  = 0;
    my $b      = 0;
    my $length = 0;

    while ( ( $length = read $handle, $buffer, 102400 ) ) {
        while ( $length < 102400 ) {
            my $missing = 102400 - $length;
            my $missing_buffer;
            my $missing_read = read $handle, $missing_buffer, $missing;
            $length += $missing_read;
            if ( !$missing_read ) {
                last;
            }
            $buffer .= $missing_buffer;
        }
        $ctx->add($buffer);
        $b++;

        if ( $b == 95 ) {
            $ctx2->add( $ctx->digest );
            $b = 0;
            $block++;
        }
    }
    close(F);
    if ( $block == 0 ) {
        return $ctx->hexdigest;
    }
    if ($b == 0) {
    	return $ctx2->hexdigest;
    }
    $ctx2->add( $ctx->digest );
    return $ctx2->hexdigest;
}

