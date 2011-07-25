HEADERS = widgetfb.h mainwindow.h player.h
SOURCES = widgetfb.cc mainwindow.cc player.cc
FORMS = mainwindow.ui
CONFIG += release
SOURCES += main.cc
LIBS += -L. -lbass
POST_TARGETDEPS = libbass.dylib
