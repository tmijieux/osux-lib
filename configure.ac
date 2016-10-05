AC_INIT([osux], [0.1.1], [lmaugere@ipb.fr, tmijieux@ipb.fr])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_SILENT_RULES([yes])

### TOOLS ##

AC_PROG_CC
AM_PROG_AR
PKG_PROG_PKG_CONFIG
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.3])
LT_INIT([win32-dll dlopen disable-static])

### LIBRARIES ##

PKG_CHECK_MODULES([YAML], [yaml-0.1])
PKG_CHECK_MODULES([LZMA], [liblzma])
PKG_CHECK_MODULES([CRYPTO], [libcrypto])
PKG_CHECK_MODULES([SQLITE], [sqlite3])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GMODULE], [gmodule-2.0])
PKG_CHECK_MODULES([GIO], [gio-2.0])
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0])

### VARIABLES ##

### OUTPUT ##
AC_CONFIG_FILES([
                  po/Makefile.in
                  Makefile
                  src/Makefile
                  src/include/Makefile
                  src/lib/Makefile
                  src/lib/beatmap/Makefile
                  src/lib/database/Makefile
                  src/lib/mod/Makefile
                  src/lib/replay/Makefile
                  src/lib/util/Makefile
                  osux-0.1.pc
               ])
AC_OUTPUT
