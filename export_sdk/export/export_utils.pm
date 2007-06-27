#!/usr/bin/perl

package export_utils;

use strict;

$export_utils::VERSION = "0.02";	#21.06.2003

##INIT
##

#SUBs

sub get_file_ext
{
  my $theme = $_[0];

  my $ext = "htm";
  if (-f $main::themedir.'/'.$theme.'/extension.dat') {
    open(EINFO,"<$main::themedir/$theme/extension.dat");
    my $newext = <EINFO>;
    close(EINFO);
    chomp $newext;
    if ($newext =~ /^[a-zA-Z0-9]+$/) {
      $ext = $newext;
    }
  }

  return $ext;
}


sub valid_theme
{
  my $theme = lc $_[0];
  
  if (!($theme =~ /^[a-z0-9\-]+$/)) {
    #illegal chars in theme name
    return 0;
  }

  if (-d $main::themedir."/$theme") {
    if (-f $main::themedir."/$theme/mylist.tpl") {
      return 1;
    }
  }

  #invalid theme
  return 0;
}

1;
