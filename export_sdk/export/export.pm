#!/usr/bin/perl

package export;

use strict;
use Storable;
use File::Copy;

$export::VERSION = "0.05";	#12.08.2003

##INIT
##

#SUBs

sub do_export
{
    my $theme = lc $_[0];
    
    #clean dir
    print " * cleaning output directory...";
    if (-d "$main::outdir/anime") {
        unlink(<$main::outdir/anime/*.*>);
        rmdir("$main::outdir/anime");
    }
    unlink(<$main::outdir/*.*>);
    print " done.\n";
				
    print " * parsing mylist.tpl...";
    &make_main_tpl($theme);
    print " done.\n";
    
    if (-f $main::themedir.'/'.$theme.'/anime.tpl') {
        print " * parsing anime.tpl...";
        mkdir("$main::outdir/anime");
	&make_sub_tpl($theme);
	print " done.\n";
    }
    
    print " * copying additional files...";
    opendir(TDIR,$main::themedir.'/'.$theme);
    while (my $f = readdir(TDIR))
    {
    	if ( ($f eq ".") || ($f eq "..") || ($f eq "mylist.tpl") || ($f eq "anime.tpl") || ($f eq "extension.dat") || ($f eq "info.dat") ) {
    		next;
    	} elsif (-d $main::themedir.'/'.$theme.'/'.$f) {
    		next;
    	}
    	#link($main::themedir.'/'.$theme.'/'.$f, $main::outdir.'/'.$f);
    	copy($main::themedir.'/'.$theme.'/'.$f, $main::outdir.'/'.$f);
    }
    closedir(TDIR);
    print " done.\n";
}


sub make_main_tpl
{
    my $theme = $_[0];

    my $tfile = $main::themedir.'/'.$theme.'/mylist.tpl';
    if (!-f $tfile) { return 0; }

    my $tpl = HTML::Template::Expr->new(
        'filename' => $tfile, 'die_on_bad_params' => 0,
	'loop_context_vars' => 1, 'global_vars' => 1,
	'no_includes' => 1,
	'functions' =>
	{
	    'jsencode' => \&export_funcs::jsencode,
	    'replace' => \&export_funcs::replace,
	    'ireplace' => \&export_funcs::ireplace
	}
    );

    my $alist = retrieve('tpl-alist.dat');
    $tpl->param("loop_anime" => $alist);

    #fill globals
    $tpl->param("global_user" => "test");
    $tpl->param("global_date" => "08.07.2003 21:04");
    $tpl->param("global_date_short" => "08.07.03");
    $tpl->param("global_animecount" => "15");
    $tpl->param("global_epcount" => "71");
    $tpl->param("global_filecount" => "72");
    $tpl->param("global_bytecount" => "14.221.110.425");
    $tpl->param("global_bytecount_h" => "13 GB");
    $tpl->param("global_animedburl" => "http://anidb.ath.cx/perl-bin/animedb.pl");

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
	
	#create HTML::Template object for mylist.tpl
	my $tpl = HTML::Template::Expr->new(
    	    'filename' => $tfile, 'die_on_bad_params' => 0,
	    'loop_context_vars' => 1, 'global_vars' => 1,
	    'no_includes' => 1, 'blind_cache' => 1,
	    'functions' =>
	    {
	   	 'jsencode' => \&export_funcs::jsencode,
	   	 'replace' => \&export_funcs::replace,
	  	 'ireplace' => \&export_funcs::ireplace
	    }
	);

	my @tmp;
	$tmp[0] = $alitem;
	$tpl->param( "loop_anime" => \@tmp );

	#fill globals
        $tpl->param("global_user" => "test");
	$tpl->param("global_date" => "08.07.2003 21:04");
	$tpl->param("global_date_short" => "08.07.03");
        $tpl->param("global_animecount" => "15");
	$tpl->param("global_epcount" => "71");
        $tpl->param("global_filecount" => "72");
	$tpl->param("global_bytecount" => "14.221.110.425");
        $tpl->param("global_bytecount_h" => "13 GB");
	$tpl->param("global_animedburl" => "http://anidb.ath.cx/perl-bin/animedb.pl");
    
        my $ext = &export_utils::get_file_ext($theme);
	my $of = "$main::outdir/anime/a$aid.$ext";

	open(OUT,">$of");
	print OUT $tpl->output();
        close(OUT);
    }
}


1;
