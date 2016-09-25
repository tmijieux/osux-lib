SUBDIRS = src po
dist_doc_DATA = README.md
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = osux-0.1.pc

ACLOCAL_AMFLAGS = -I m4

uninstall-hook:
	if test -d ${pkgincludedir}; then rmdir ${pkgincludedir}; fi
	if test -d ${pkgdatadir}; then rmdir ${pkgdatadir}; fi
	if test -d ${docdir}; then rmdir ${docdir}; fi
