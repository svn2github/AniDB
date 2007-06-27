#!/usr/bin/perl

package export;

use strict;
use Storable;
use File::Copy;

$export::VERSION = "0.07";	#26.06.2007

##INIT
##

#SUBs

sub do_export
{
	my $theme = lc $_[0];

	#clean dir
	print " * cleaning output directory...";
	if (-d "$main::outdir/anime")
	{
		unlink(<$main::outdir/anime/*.*>);
		rmdir("$main::outdir/anime");
	}
	unlink(<$main::outdir/*.*>);
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
	opendir(TDIR,$main::themedir.'/'.$theme);
	while (my $f = readdir(TDIR))
	{
		if ( ($f eq ".") || ($f eq "..") || ($f eq "mylist.tpl") || ($f eq "anime.tpl") || ($f eq "extension.dat") || ($f eq "info.dat"))
		{
			next;
		}
    elsif ( $f =~ /.+?\~/ )
    {
      next;
    }
		elsif (-d $main::themedir.'/'.$theme.'/'.$f)
		{
			next;
		}
		copy($main::themedir.'/'.$theme.'/'.$f, $main::outdir.'/'.$f);
	}
	closedir(TDIR);
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
	open(OUT,">$main::outdir/mylist.$ext");
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

		open(OUT,">$of");
		print OUT $tpl->output();
		close(OUT);
	}
}

1;
