TEMPLATE = app

FORMS += GameInterface.ui

CONFIG += qt qglviewer 
QT += xml

SOURCES = main.cpp \
          GameViewer.cpp \
	  GameInterface.cpp \
	  BoulderGame.cpp \
	  Level.cpp

HEADERS = GameViewer.h \
	  GameInterface.h \
	  BoulderGame.h \
          Level.h


