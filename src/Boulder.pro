TEMPLATE = app

CONFIG += qt qglviewer 
QT += xml 

FORMS = GameInterface.ui

SOURCES = main.cpp \
          GameViewer.cpp \
	  GameInterface.cpp \
	  BoulderGame.cpp \
	  Level.cpp

HEADERS = GameViewer.h \
	  GameInterface.h \
	  BoulderGame.h \
          Level.h

OBJECTS_DIR = .obj
TARGET = boulder
