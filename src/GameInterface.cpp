#include <iostream>
#include <math.h>

#include <QKeyEvent>
#include <QFileDialog>
#include <QtGui/QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

#include "GameInterface.h"
#include "BoulderGame.h"

using namespace std ;

GameInterface::GameInterface()
	:QMainWindow(NULL)
{
	setupUi(this) ;
    
	_current_game = NULL ;
    
	if(gameViewer != NULL)
		gameViewer->setCurrentMode(GAME_MODE_NONE) ;

	//mSceneCenterPos = QPointF(0,0);
	//mMousePressed = false ;

	connect(actionNew,SIGNAL(triggered()),this,SLOT(newGame())) ;
	connect(actionSave,SIGNAL(triggered()),this,SLOT(saveGame())) ;
	connect(actionLoad,SIGNAL(triggered()),this,SLOT(loadGame())) ;
	connect(actionLevelEditor,SIGNAL(triggered()),this,SLOT(editGame())) ;
}

void GameInterface::editGame() 
{
    if(gameViewer->currentMode() == GAME_MODE_EDITOR)
    {
		if(_current_game != NULL)
			_current_game->start();

		if(gameViewer != NULL)
			gameViewer->setCurrentMode(GAME_MODE_GAME) ;

        actionLevelEditor->setText("Enter level editor") ;
    }
    else
    {
		if(_current_game != NULL)
			_current_game->stop();
    
		if(gameViewer != NULL)
			gameViewer->setCurrentMode(GAME_MODE_EDITOR) ;

        actionLevelEditor->setText("Quit level editor") ;
    }
}

void GameInterface::quit() const
{
	exit(0) ;
}

//void Graphics_PlayerPiece::mousePressEvent( QGraphicsSceneMouseEvent * event )
//{
//	event->accept() ;
//	_original_offset = offset() ;
//	emit selected() ;
//}
//
//void Graphics_PlayerPiece::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
//{
//	setOffset(offset() + event->pos() - event->lastPos()) ;
//}

//void Graphics_PlayerPiece::mouseReleaseEvent( QGraphicsSceneMouseEvent * )
//{
//	// determine new integer game position
//	
//	int w = scene()->views().first()->width() ;
//	int h = scene()->views().first()->height() ;
//
//	QPointF offsetShear(offset() - _original_offset) ;
//	Position new_pos( _pos[0]+(int)rint(offsetShear.x()/w*GAME_SIZE),_pos[1]+(int)rint(offsetShear.y()/h*GAME_SIZE) ) ;
//
//	setPosition(new_pos) ;
//
//	emit movedToNewPosition(_side,_pos) ;
//}

// This method is responsible for displaying info about what's happening, depending on the
// context:
//		- possible wall positions
//		- possible player positions
//
//QRectF GameInterface::pieceRect(const Position& pos) const
//{
//	QRectF rect ;
//	float ws2 = WALL_SIZE/2 ;
//
//	return QRectF(	QPoint((int)rint(_w*(pos[0]  +ws2)/float(GAME_SIZE)),(int)(_h*(pos[1]  +ws2)/float(GAME_SIZE))),
//	               QPoint((int)rint(_w*(pos[0]+1-ws2)/float(GAME_SIZE)),(int)(_h*(pos[1]+1-ws2)/float(GAME_SIZE))));
//}

//		graphicsView->scene()->removeItem(_walls[i]) ;
//		
//	_walls.clear() ;
//	
//	setPiecePosition(~_player_side,Position(GAME_SIZE/2,0)) ;
//	setPiecePosition( _player_side,Position(GAME_SIZE/2,GAME_SIZE-1)) ;
//
//	_pieces[ _player_side]->setEnabled(true) ;
//	_pieces[~_player_side]->setEnabled(false) ;
//	_pieces[ _player_side]->setVisible(true) ;
//	_pieces[~_player_side]->setVisible(true) ;
//	
//	_winner_rect->setVisible(false) ;
//	_player_wins_text->setVisible(false) ;
//	_computer_wins_text->setVisible(false) ;
//
//	for(uint i=0;i<game.listOfWalls().size();++i)
//		addWallItem(wallRect(game.listOfWalls()[i].p1(),game.listOfWalls()[i].p2())) ;

//void GameInterface::setPiecePosition(GameSide s, const Position& pos) const
//{
//	_pieces[s]->setPosition(pos) ; 
//}

void GameInterface::newGame()
{
    if(_current_game != NULL)
    {
	    _current_game->stop();
	    delete _current_game ;
    }

    _current_game = new BoulderGame ;
    _current_game->start();
    
    gameViewer->setCurrentMode(GAME_MODE_GAME) ;
    gameViewer->setGame(_current_game) ;
}
void GameInterface::saveGame() const
{
	if(_current_game == NULL)
        	return ;

    	QString name = QFileDialog::getSaveFileName(NULL,"Save your level","Choose a file to save to","Levels (*.blv)") ;
        
        if(name.isNull())
            return ;
        
    	_current_game->currentState().save(name.toStdString()) ;
}

void GameInterface::loadGame() 
{
    	QString name = QFileDialog::getOpenFileName(NULL,"Load a level","Choose a file to save to","Levels (*.blv)") ;
        
        if(name.isNull())
            return ;
        

    BoulderGame *tmp_game = new BoulderGame ;
    tmp_game->currentState().load(name.toStdString()) ;
    
    if(_current_game != NULL)
    {
	    _current_game->stop();
	    delete _current_game ;
    }
    
    _current_game = tmp_game ;
    
    gameViewer->setGame(_current_game) ;
    //gameViewer->setCurrentMode(GAME_MODE_GAME) ;
    
    if(gameViewer->currentMode() == GAME_MODE_GAME)
		_current_game->start();
}
void GameInterface::about() const
{
	QMessageBox::information(NULL,QString("About Boulder"),
		QString("Boulder implements the classical <i>Boulder Dash</i> game. <br/><br/>Programing by Cyril Soler.")) ;
}















