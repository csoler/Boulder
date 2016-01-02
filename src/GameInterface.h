#ifndef _GAMEINTERFACE_H
#define _GAMEINTERFACE_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "ui_GameInterface.h"

class BoulderGame;

class GameInterface: public QMainWindow, private Ui::GameInterface
{
	Q_OBJECT

	public:
		GameInterface() ;

	private slots:
		void quit() const ;
		void newGame() ;
		void about() const ;

	private:
		BoulderGame *_current_game ;

private:
};

#endif

