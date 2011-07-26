HEADERS = widgettab.h widgetfb.h mainwindow.h player.h
SOURCES = widgettab.cc widgetfb.cc mainwindow.cc player.cc
FORMS = mainwindow.ui
CONFIG += release
SOURCES += main.cc
LIBS += -L.
unix {
	HARDWARE_PLATFORM = $$system(uname -a)
	contains( HARDWARE_PLATFORM, x86_64 ) {
	# 64-bit Linux
	LIBS += -lbass.64b
	} else {
	# 32-bit Linux
	LIBS += -lbass.32b
	}
}
macos {
	POST_TARGETDEPS = libbass.dylib
}
