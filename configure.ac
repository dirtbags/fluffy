# Process this file with autoconf to produce a configure script

AC_PREREQ([2.65])

AC_INIT([netre-tools], [1.1337], [pi-rho@tyr.cx])
AC_CONFIG_SRCDIR([src/hdng.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 gnu check-news silent-rules subdir-objects no-dist-gzip dist-xz -Wall -Werror])
AM_MAINTAINER_MODE
AM_SILENT_RULES([yes])
AC_SUBST([VERSION])

# stop CFLAGS duplication
: ${CFLAGS=""}

# Find programs...
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
LT_INIT

# Check for headers
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strncasecmp strtol])

# Is this gcc?
AM_CONDITIONAL([IS_GCC], [test "x$GCC" = xyes])

# debug stuff
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [Enable debug statements]),
	[enable_debug=yes;AM_DEFAULT_VERBOSITY=1], [enable_debug=no])
AM_CONDITIONAL([IS_DEBUG], [test "x$enable_debug" == xyes])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo
echo "$PACKAGE v$VERSION"
echo

echo "Debug enabled : $enable_debug"
echo "Prefix ...... : $prefix"
echo
