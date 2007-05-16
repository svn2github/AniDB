(C) 2005 by fahrenheit [fahrenheit@netcabo.pt]
version 1.0.1 (12.01.2005):

To configure the Trade page, simply open the "code.js" file with a text editor and edit the line that says:

	>> var tmail = '';

to your mail, e.g.:

	>> var tmail = 'fahrenheit@netcabo.pt';

Remember the e-mail must be inside the "'" thingies.

Besides this change to someone to be able to see the trade options that person needs to have it enabled in the preference page.

Never the less, you can change the default behaviour for this option by changing this line in the "code.js" file: 

	>> var pref_trade = getCookie("pref_trade") || 0;

to this:

	>> var pref_trade = getCookie("pref_trade") || 1;


After that the Trade function will be active.

Please, don't mess with any of the code following those entries...

------------------------------------------------------------------------

On a sidenote, this template and the anidbstyle one share the same information, so you can basicaly change from one to the other by simply overwriting the *.html and *.js files of one another.

This is an option realy, because i can change the layout of the export files of one of the templates and render this option useless, so to be on the safe side just export your mylist again using the export function on anidb ^_^.

Note: this doesn't work for the whiteorange template, that i also made, hmm, maybe it will work later on..

------------------------------------------------------------------------

Comments, sugestions and such, PM me at the anidb foruns or write a line or two to my mail.