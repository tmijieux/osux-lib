AM_CFLAGS = \
	-Wall -Wextra \
	-I$(top_srcdir)/src/include \
	-std=gnu99 \
	$(GLIB_CFLAGS) $(GMODULE_CFLAGS) \
	$(GOBJECT_CFLAGS) $(GIO_CFLAGS)

#preprocessor:
AM_CPPFLAGS= \
    -DLOCALEDIR=\""$(localedir)"\" \
    -DDATADIR=\"$(pkgdatadir)\"

AM_LDFLAGS = \
	$(GLIB_LIBS) $(GMODULE_LIBS) \
	$(GOBJECT_LIBS) $(GIO_LIBS) \
	-no-undefined
