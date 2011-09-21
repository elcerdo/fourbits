HEADERS = widgettab.h widgetfb.h mainwindow.h player.h
SOURCES = widgettab.cc widgetfb.cc mainwindow.cc player.cc
FORMS = mainwindow.ui
CONFIG += release
SOURCES += main.cc
LIBS += -L.
unix {
	HARDWARE_PLATFORM = $$system(uname -a)
	contains( HARDWARE_PLATFORM, x86_64 ) {
	message(Linux 64b)
	LIBS += -lbass.64b
	} else {
	message(Linux 32b)
	LIBS += -lbass.32b
	}
}
macx {
	message(Mac)
	LIBS += -lbass
	PRIVATE_FRAMEWORKS.files = libbass.dylib
	PRIVATE_FRAMEWORKS.path = Contents/MacOS
	QMAKE_BUNDLE_DATA += PRIVATE_FRAMEWORKS
}
