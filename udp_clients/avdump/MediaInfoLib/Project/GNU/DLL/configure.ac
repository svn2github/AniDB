dnl Test if we are at the good place
AC_INIT(../../../Source/MediaInfoDLL/MediaInfoDLL.cpp)

dnl sets build, host, target variables and the same with _alias
AC_CANONICAL_BUILD

dnl Name and version
AM_INIT_AUTOMAKE(MediaInfoDLL,0)

dnl Program inits
AC_PROG_CC
AC_PROG_CXX
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AC_PROG_INSTALL

dnl #########################################################################
dnl ### Options
dnl #########################################################################

dnl #########################################################################
dnl ### Parsing options
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
ZenLib_Config="../../../../ZenLib/Project/GNU/Library/ZenLib-config"
enableunicode="$($ZenLib_Config Unicode)"
if test "$enableunicode" = "yes"; then
 AC_DEFINE(UNICODE)
 MediaInfoLib_Unicode="yes"
 MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS -DUNICODE"
else
 MediaInfoLib_Unicode="no"
fi

dnl #########################################################################
dnl ### Needed libs
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl MediaInfoLib
dnl
if test -e ../../../../MediaInfoLib/Project/GNU/Library/libMediaInfoLib.la; then
 echo "MediaInfoLib found"
else
 AC_MSG_ERROR([MediaInfoLib is not found])
fi
CXXFLAGS="$CXXFLAGS $(../../../../MediaInfoLib/Project/GNU/Library/MediaInfoLib-config CXXFLAGS)"
LIBS="$LIBS $(../../../../MediaInfoLib/Project/GNU/Library/MediaInfoLib-config LIBS)"

dnl #########################################################################
dnl ### C/C++ compiler options used to compile
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test "$enableunicode" = "yes"; then
 AC_DEFINE(UNICODE)
fi

dnl -------------------------------------------------------------------------
dnl GNU compiler
dnl
dnl C/C++ compiler options used to compile
if test "$GXX" = yes ; then
    CFLAGS="$CFLAGS -Wall -Wundef"
    CXXFLAGS="$CXXFLAGS -Wall -W -Wundef -Wcast-qual -Wno-ctor-dtor-privacy"
fi

dnl #########################################################################
dnl ### Output
dnl #########################################################################

AC_OUTPUT(Makefile)

dnl #########################################################################
dnl ### Report how we have been configured
dnl #########################################################################

echo ""
echo "Configured MediaInfoDLL for '$build'"
echo ""
echo "  Unicode?                                                $enableunicode"
echo ""
echo CXXFLAGS $CXXFLAGS
echo LIBS $LIBS
