#!/bin/sh
case "${1}" in
  small)
    echo "small export"
    cp tpl-alist.dat.small tpl-alist.dat
    perl export.pl $2
    exit 0
    ;;
  big)
    echo "big export"
    cp tpl-alist.dat.big tpl-alist.dat
    perl export.pl $2
    exit 0
    ;;
  *)
    echo "usage: $0 small|big template_name"
    exit 1
	;;
esac	
