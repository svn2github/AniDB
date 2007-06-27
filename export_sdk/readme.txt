AnimeDB Template SDK
Version: 0.11 - 26.06.2007
(C) 2003-2007 by EXP

Get new Versions here: http://forum.anidb.net/viewtopic.php?t=429
Wiki Page: http://wiki.anidb.info/w/Mylist_export_template_guidelines

----------------------------------------------------------------------------------------------

Content:
1. About this SDK
2. Current version and where to get
3. Resources
4. Installing Perl
5. Additional Module HTML::Template::Expr
6. Changelog


1. About this SDK

  This SDK is ment to allow you to test your templates for the anidb mylist export function on
  your own pc. It should work under Windows and Linux. You might need to install some additional
  software (see bellow for Installing Perl). 

2. Current version and where to get

  Current version of the SDK is 0.11, get here: http://static.anidb.net/files/anidb_template_sdk_0.11.tgz

3. Resources

  The development forum, here: http://forum.anidb.net/viewforum.php?f=11 
  The SDK thread, here: http://forum.anidb.net/viewtopic.php?t=429 
  Image pack, here: http://static.anidb.net/files/anidb_icon_pack.rar
  Wiki page: http://wiki.anidb.info/w/Mylist_export_template_guidelines
  ChangeLog: http://wiki.anidb.info/w/Mylist_export_SDK_changelog
  HTML::Template::Expr info: http://html-template.sourceforge.net/html_template_expr.html 

4. Installing Perl

  This programm needs Perl to create example html files from the templates. If you don't have
  Perl installed (preferably 5.8.8) you'll need to do so now. 
  Under Windows you'll need to download ActivePerl 5.8.8.820+ from activeperl.com:

    http://downloads.activestate.com/ActivePerl/Windows/5.8/ 
    Install it to c:\perl (or some other directory, but remember where you installed it!). 

  Under Linux it should already be installed. If not get it from:

    http://downloads.activestate.com/ActivePerl/Linux/5.8/ 

  If you are not using 5.8.8.820 it's possible you need to update Storable, try the following:

    perl -MCPAN -eshell 
    install Storable 
    bye 

  Note: If you installed 5.8.8.820 don't overwrite Perl's copy of Storable with the one in the SDK. 

5. Additional Module HTML::Template::Expr

  Under Windows with ActivePerl go to it's startmenu entry (might be called "ActiveState ActivePerl 5.8")
  and start "Perl Package Manager". Then enter:

    install HTML-Template-Expr 
    exit
    
  Note: On recent versions the Perl Package Manager is a GUI aplication, the updated instrcutions are
  as follows:
  
    launch PPM (wait a bit for it to sync it's databases)
    type in the search box: HTML-Template-Expr
    Right click on the results and select Install
    Then click on the third icon to the right of the search box, the green arrow (Run Marked Actions)
    exit

  Under Linux you have to do a:

    perl -MCPAN -eshell 
    install HTML::Template::Expr 
    bye 

  You should now be able to execute export.pl, take a look at the provided bat and a sh file
  to see how it is used.
  (export.pl takes one parameter which is the name of the template to use) 

  Note: you can get more information about the HTML::Template::Expr here: 
        http://html-template.sourceforge.net/html_template_expr.html 


6. Changelog:
  See changelog.txt
