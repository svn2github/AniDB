#!/usr/bin/perl

package export;

use strict;
use Storable;
use File::Copy;

binmode( STDIN,  ':utf8' );
binmode( STDOUT, ':utf8' );
binmode( STDERR, ':utf8' );

$export::VERSION = "0.07";	#26.06.2007

##INIT
##

my $theme;

#SUBs

sub recurse_dir 
{
  my($path) = @_;
  my $basedir = $main::themedir.'/'.$theme;
  $path .= '/' if($path !~ /\/$/);
  for my $f (glob($path.'*')) {
    my $file = $f;
    $file =~ s/($basedir)//mgi;
    my $fname = substr($f,rindex($f,'/')+1,length($f));
    if( -d $f) {
      mkdir($main::outdir.'/'.$file);
      recurse_dir($f);
    } else {
      if ( ($fname eq ".") || ($fname eq "..") || ($fname eq "mylist.tpl") || ($fname eq "anime.tpl") || ($fname eq "extension.dat") || ($fname eq "info.dat"))
      {
        next;
      }
      elsif ( $f =~ /.+?\~/ )
      {
        next;
      }
      copy($f, $main::outdir.'/'.$file);
    }
  }
}

sub cleanup {
  my $dir = shift;
  local *DIR;

  opendir DIR, $dir or die "opendir $dir: $!";
  my $found = 0;
  while ($_ = readdir DIR) {
    next if /^\.{1,2}$/;
    my $path = "$dir/$_";
    unlink $path if -f $path;
    cleanup($path) if -d $path;
  }
  closedir DIR;
  rmdir $dir or print "error - $! \n";
}

sub do_export
{
	$theme = lc $_[0];

	#clean dir
	print " * cleaning output directory...";
  &cleanup($main::outdir);
  mkdir($main::outdir);
	print " done.\n";

	print " * parsing mylist.tpl...";
	&make_main_tpl($theme);
	print " done.\n";

	if (-f $main::themedir.'/'.$theme.'/anime.tpl')
	{
		print " * parsing anime.tpl...";
		mkdir("$main::outdir/anime");
		&make_sub_tpl($theme);
		print " done.\n";
	}

	print " * copying additional files...";
  &recurse_dir($main::themedir.'/'.$theme);
	print " done.\n";
}


sub create_tpl
{
  my $tfile = $_[0];
	my $tpl = HTML::Template::Expr->new(
		'filename' => $tfile, 'die_on_bad_params' => 0,
		'loop_context_vars' => 1, 'global_vars' => 1,
		'no_includes' => 1,
		'functions' =>
		{
			'jsencode' => \&export_funcs::jsencode,
      'jsencodehtml' => \&export_funcs::jsencodehtml,
			'replace' => \&export_funcs::replace,
			'ireplace' => \&export_funcs::ireplace
		}
	);
	
  $tpl->param("global_user" => "test");
  $tpl->param("global_user_id" => "32");
	$tpl->param("global_date" => "26.06.2007 12:00");
	$tpl->param("global_date_short" => "26.06.07");
	$tpl->param("global_animedburl" => "http://anidb.info/perl-bin/animedb.pl");
	$tpl->param("global_userstat_animeadded" => "1");
	$tpl->param("global_userstat_epsadded" => "3");
	$tpl->param("global_userstat_filesadded" => "49");
	$tpl->param("global_userstat_groupsadded" => "2");
	$tpl->param("global_userstat_lamefiles" => "0");
	$tpl->param("global_userstat_lamefilesp" => "0%");
	$tpl->param("global_userstat_independencep" => "3%");
	$tpl->param("global_userstat_leechp" => "97%");
	$tpl->param("global_userstat_reviews" => "1");
	$tpl->param("global_userstat_votes" => "65");
	$tpl->param("global_userstat_totalviewedp" => "3%");
	$tpl->param("global_userstat_totalownedp" => "5%");
	$tpl->param("global_userstat_ownviewedp" => "62%");
	$tpl->param("global_userstat_viewedepcnt" => "68");

  my $filesize = -s 'tpl-alist.dat';

	#fill globals
	if ($filesize < 1000000)
	{
		$tpl->param("global_animecount" => "24");
		$tpl->param("global_epcount" => "90");
		$tpl->param("global_filecount" => "96");
		$tpl->param("global_bytecount" => "21.337.110.425");
		$tpl->param("global_bytecount_h" => "21 GB");
	}
	else
	{
		$tpl->param("global_animecount" => "345");
		$tpl->param("global_epcount" => "562");
		$tpl->param("global_filecount" => "5075");
		$tpl->param("global_bytecount" => "123.375.452.160");
		$tpl->param("global_bytecount_h" => "115 GB");
	}
  
  return ($tpl);
}

sub make_main_tpl
{
	my $theme = $_[0];

	my $tfile = $main::themedir.'/'.$theme.'/mylist.tpl';
	if (!-f $tfile) { return 0; }

	my $tpl = &create_tpl($tfile);
  
  my $alist = retrieve('tpl-alist.dat');
	$tpl->param("loop_anime" => $alist);

	my $ext = &export_utils::get_file_ext($theme);
	open(OUT,">:utf8", "$main::outdir/mylist.$ext");
	print OUT $tpl->output();
	close(OUT);
}


sub make_sub_tpl
{
	my $theme = $_[0];

	my $tfile = $main::themedir.'/'.$theme.'/anime.tpl';
	if (!-f $tfile) { return 0; }

	my $alist = retrieve('tpl-alist.dat');
	#start the loop
	foreach my $alitem (@{$alist})
	{
		my $aid = $alitem->{'data_anime_id'};
		if (!$aid) { next; }

    my $tpl = &create_tpl($tfile);

		my @tmp;
		$tmp[0] = $alitem;
		$tpl->param( "loop_anime" => \@tmp );

		my $ext = &export_utils::get_file_ext($theme);
		my $of = "$main::outdir/anime/a$aid.$ext";

		open(OUT,">:utf8", "$of");
		print OUT $tpl->output();
		close(OUT);
	}
}

1;
