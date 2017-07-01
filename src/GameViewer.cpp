#include <QGLViewer/constraint.h>
#include <QMouseEvent>
#include <QStylePainter>
#include <QMessageBox>

#include "Level.h"
#include "GameViewer.h"
#include "BoulderGame.h"

//#define DEBUG_GAME_VIEWER 1

static const uint32_t SQUARE_SIZE_IN_PIXELS = 50 ;

GameViewer::GameViewer(QWidget *parent)
	:QWidget(parent), mGameDrawer(width(),height())
{
    mGame = NULL ;
    
    mMousePressed = false ;
    mSceneCenterX = 0 ;
    mSceneCenterY = 0 ;
    
    setMouseTracking(true) ;
    setFocusPolicy(Qt::StrongFocus) ;
}

void GameViewer::mousePressEvent(QMouseEvent *e)
{
    mMousePressed = true ;
    mOldMouseX = e->x() ;
    mOldMouseY = height()-1-e->y() ;
}
void GameViewer::mouseReleaseEvent(QMouseEvent *e)
{
    mMousePressed = false ;
}
void GameViewer::mouseMoveEvent(QMouseEvent *e)
{
    if(mMousePressed)
    {
    mSceneCenterX += e->x() - mOldMouseX ;
    mSceneCenterY += (height()-1-e->y()) - mOldMouseY ;
    
    update();
    }
    
    mOldMouseX = e->x() ;
    mOldMouseY = height()-1-e->y() ;
}

void GameViewer::pixelCoordinatesToGameCoordinate(int x,int y,int& i,int& j) const
{
}

void GameViewer::setGame(BoulderGame *g)
{
    mGame = g ;
    
    //setSceneRadius(2*std::max(mGame->currentState().sizeX(),mGame->currentState().sizeY())) ;
    //updateGL() ;
    
    QObject::connect(mGame,SIGNAL(changed()),this,SLOT(reDraw())) ;
                     
    mGameDrawer.update(*g,width(),height(),mCurrentMode) ;
    update() ;
}

void GameViewer::reDraw() 
{
	if(mGame != NULL)
	{
		std::cerr << "redraw!" << std::endl;
		mGameDrawer.update(*mGame,width(),height(),mCurrentMode) ;
		update();
	}
 
}
void GameViewer::resizeEvent(QResizeEvent *e)
{
    if(mGame != NULL)
	    mGameDrawer.update(*mGame,width(),height(),mCurrentMode) ;
    
    update() ;
}

void GameViewer::paintEvent(QPaintEvent *)
{
    QStylePainter(this).drawPixmap(0,0,mGameDrawer.pixmap()) ;
#ifdef DEBUG_GAME_VIEWER    
    std::cerr << "In paint event. pasting pixmap of size " << mGameDrawer.pixmap().width() << "x" << mGameDrawer.pixmap().height() << std::endl;
#endif
}

void GameViewer::setCurrentMode(GameMode m)
{
    mCurrentMode = m ;
}
    
void GameViewer::keyPressEvent(QKeyEvent *e)
{
    int i = mGameDrawer.windowCoordToGameCoordX(mOldMouseX) ;
    int j = mGameDrawer.windowCoordToGameCoordY(height() -1 -mOldMouseY) ;
    
    e->accept() ;
    bool redraw = false ;
    
    switch(e->key())
    {
    case Qt::Key_Left: mGame->currentState().movePlayer(Level::Left) ;
	    redraw = true ;
	    break ;

    case Qt::Key_Right:mGame->currentState().movePlayer(Level::Right) ;
	    redraw = true ;
	    break ;
    case Qt::Key_Up:   mGame->currentState().movePlayer(Level::Top) ;
	    redraw = true ;
	    break ;
    case Qt::Key_Down: mGame->currentState().movePlayer(Level::Bottom) ;
	    redraw = true ;
	    break ;

    case Qt::Key_D: if(i > 0 && i < mGame->currentState().sizeX()-1 && j > 0 && j < mGame->currentState().sizeY()-1) 
		            mGame->currentState()(i,j) = Level::Diamond ;
	    redraw = true ;
	    break ;
        
    case Qt::Key_B: if(i > 0 && i < mGame->currentState().sizeX()-1 && j > 0 && j < mGame->currentState().sizeY()-1) 
		            mGame->currentState()(i,j) = Level::Bomb ;
	    redraw = true ;
	    break ;

    case Qt::Key_V: if(i > 0 && i < mGame->currentState().sizeX()-1 && j > 0 && j < mGame->currentState().sizeY()-1) 
		            mGame->currentState()(i,j) = Level::Void ;
	    redraw = true ;
	    break ;

    case Qt::Key_W: if(i >= 0 && i < mGame->currentState().sizeX() && j >= 0 && j < mGame->currentState().sizeY())
		            mGame->currentState()(i,j) = Level::Wall ;
	    redraw = true ;
	    break ;

    case Qt::Key_X: if(i >= 0 && i < mGame->currentState().sizeX() && j >= 0 && j < mGame->currentState().sizeY())
		            mGame->currentState()(i,j) = Level::MetalWall0 ;
	    redraw = true ;
	    break ;

    case Qt::Key_S: if(i > 0 && i < mGame->currentState().sizeX()-1 && j > 0 && j < mGame->currentState().sizeY()-1) 

        mGame->currentState()(i,j) = Level::Stone ;
	    redraw = true ;
	    break ;

    case Qt::Key_E: if(i > 0 && i < mGame->currentState().sizeX()-1 && j > 0 && j < mGame->currentState().sizeY()-1) 

        mGame->currentState()(i,j) = Level::Earth ;
	    redraw = true ;
	    break ;

    default:
	    e->ignore() ;
    }
    
   
    if(redraw)
    {
	mGameDrawer.update(*mGame,width(),height(),mCurrentMode) ;
	update() ;
    }
}

