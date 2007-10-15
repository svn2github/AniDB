#!/usr/bin/perl
# AniDB.net media player wrapper which uses the AniDB UDP API
# (via the AniDB::UDPClient library) to add a file as watched
# once you've finished watching it.
# (C) 2007 by EXP

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

use AniDB::UDPClient;
use Log::Log4perl qw(:easy);
use Data::Dumper;

$main::VERSION = '0.01'; #2007-10-12

#############################################
#  MODIFY THESE VALUES TO MATCH YOUR SETUP  #
#############################################

# Your AniDB (http://www.anidb.net) usename.
# NOTE: AniDB usernames are always *lowercase*.
use constant USERNAME => 'username';

# Your AniDB password.
use constant PASSWORD => 'password';

# The command line to start the real player app.
# You may also add any additional parameters here.
use constant PLAYER => '/usr/bin/gmplayer';

# This script will ignore any files which are not located
# within one of the directories specified here.
# I.e. if you have a directory "anime" with all your anime
# and a directory "movies" with all your non-anime movies,
# then you should only add the "anime" directory here.
# Set to undef to deactivate this check.
use constant ANIME_DIRECTORIES => ('/data/anime');

# Minimal time in minutes which needs to elapse between the
# the player startup and termination in order to consider
# the file watched.
# (this is a pretty ugly hack as we neither take the real
# length of the file into account not any pause/unpause
# or forward/rewind actions by the user)
use constant MIN_LENGTH => 10;

# Minimal file size in bytes. All files smaller than this
# value will be ignored.
use constant MIN_SIZE => 10 * 1024 * 1024;

# Should files known to AniDB be added to your mylist,
# if they are not already on it?
use constant ADD_TO_MYLIST => 1;

# Debug Log level
#   possible values: $TRACE, $DEBUG, $INFO, $WARN, $ERROR, $FATAL
use constant PLAYER_LOGLEVEL => $TRACE;
# Log file, if specified all log output will be appended to the given file.
use constant PLAYER_LOGFILE => undef;
#use constant PLAYER_LOGFILE => '/tmp/playerclient.log';
# Alternatively you can specify your own Log4Perl configuration file
use constant PLAYER_LOGCONFIG => undef;
#use constant PLAYER_LOGCONFIG => 'playerclient-log4perl.conf';

#############################################
#  DO NOT MODIFY ANYTHING BELOW THIS LINE!  #
#############################################

init_logging();

DEBUG( 'STARTING UP - V'.$main::VERSION.' (lib: V'.$AniDB::UDPClient::VERSION.') - args: [' . join( ', ', @ARGV ) . ']' );

#make sure we have a media file as parameter
if ( !$ARGV[0] || !-f $ARGV[0] )
{
	LOGDIE "Usage: $0 <media file>\n";
}

#keep track of the startup time
my $start = time();

my $cmd = PLAYER . ' "' . $ARGV[0] . '"';
DEBUG( 'starting player: ' . $cmd );

#start the media player
system($cmd);

my $delay = time() - $start;
DEBUG( 'player finished - after ' . $delay . ' secs' );

#check the file size
if ( -s $ARGV[0] < MIN_SIZE )
{
	INFO('IGNORED: the media file was too small, not marking file as watched');
	DEBUG('EXITING');
	exit;
}

#recheck the time
if ( $delay < MIN_LENGTH * 60 )
{
	INFO('IGNORED: the player exited too early, not marking file as watched.');
	DEBUG('EXITING');
	exit;
}

#check the directory
if ( defined ANIME_DIRECTORIES )
{
	my $dirOk = 0;
	foreach my $dir (ANIME_DIRECTORIES)
	{
		if ( $ARGV[0] =~ /^$dir/i )
		{
			DEBUG( 'file located in anime directory: ' . $dir );
			$dirOk = 1;
			last;
		}
	}

	if ( !$dirOk )
	{
		INFO('IGNORED: the file is not located inside the specified anime directories.');
		DEBUG('EXITING');
		exit;
	}
}

#ok, mark the file as watched
mark_watched( $ARGV[0] );

DEBUG('EXITING');

############
#   SUBs   #
############

sub mark_watched
{
	my $file = shift;

	INFO( 'marking file as watched: ' . $file );

	DEBUG('connecting to UDP API');
	my $a = AniDB::UDPClient->new(
		username  => USERNAME,
		password  => PASSWORD,
		client    => 'playerclient',
		clientver => '1'
	);

	if ( !$a->login() )
	{
		ERROR('FAILED: login to UDP API failed');
		return 0;
	}

	DEBUG('retrieving file info');
	#first check if anidb knows this file
	my $fileinfo = $a->file($file);
	TRACE( sub { 'got file data: ' . Data::Dumper::Dumper($fileinfo) } );
	if ( !defined $fileinfo )
	{
		WARN( 'FAILED: the file is not known to AniDB: ' . $file );
		$a->logout();
		return 0;
	}

	DEBUG('retrieving mylist info');
	my $mylistinfo = $a->mylist( {'fid' => $fileinfo->{'fid'}} );
	TRACE( sub { 'got mylist data: ' . Data::Dumper::Dumper($mylistinfo) } );
	if ( !defined $mylistinfo )
	{
		if (ADD_TO_MYLIST)
		{
			DEBUG('ADDING: adding file to mylist, with state watched.');
			if ( !$a->mylistadd( {'fid' => $fileinfo->{'fid'}, 'viewed' => 1} ) )
			{
				ERROR( 'FAILED: error while adding file to mylist: ' . $file );
				$a->logout();
				return 0;
			}
			$a->logout();
			return 1;
		}
		else
		{
			WARN( 'FAILED: the file is not in your mylist: ' . $file );
			$a->logout();
			return 0;
		}
	}

	if ( $mylistinfo->{'viewdate'} == 0 )
	{
		#the file is currently marked as unwatched
		#mark it as watched
		DEBUG('WATCHED: marking file as watched.');
		if ( !$a->mylistadd( {'fid' => $fileinfo->{'fid'}, 'viewed' => 1, 'edit' => 1} ) )
		{
			ERROR( 'FAILED: error while adding file to mylist: ' . $file );
			$a->logout();
			return 0;
		}
	}
	else
	{
		DEBUG('IGNORED: the file is already marked watched.');
	}

	$a->logout();
	return 1;
} ## end sub mark_watched

sub init_logging
{
	if ( defined PLAYER_LOGCONFIG )
	{
		#use configuration file
		if ( !-f PLAYER_LOGCONFIG )
		{
			LOGDIE 'unable to find file: ' . PLAYER_LOGCONFIG;
		}

		Log::Log4perl::init(PLAYER_LOGCONFIG);
	}
	else
	{
		my %cfg = ( 'level' => PLAYER_LOGLEVEL );
		if ( defined PLAYER_LOGFILE )
		{
			$cfg{'file'} = '>>' . PLAYER_LOGFILE;
		}
		Log::Log4perl->easy_init( \%cfg );
	}
}
