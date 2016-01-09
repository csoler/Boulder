#include <QGLViewer/constraint.h>
#include <QMouseEvent>
#include <QStylePainter>

#include "Level.h"
#include "GameViewer.h"
#include "BoulderGame.h"

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

void GameViewer::setGame(BoulderGame *g)
{
    if(mGame != NULL)
        delete mGame ;
    
    mGame = g ;
    
    //setSceneRadius(2*std::max(mGame->currentState().sizeX(),mGame->currentState().sizeY())) ;
    //updateGL() ;
    
    mGameDrawer.update(*g,width(),height()) ;
    update() ;
}
void GameViewer::resizeEvent(QResizeEvent *e)
{
    if(mGame != NULL)
	    mGameDrawer.update(*mGame,width(),height()) ;
    
    update() ;
}

void GameViewer::paintEvent(QPaintEvent *)
{
    QStylePainter(this).drawPixmap(0,0,mGameDrawer.pixmap()) ;
    
    std::cerr << "In paint event. pasting pixmap of size " << mGameDrawer.pixmap().width() << "x" << mGameDrawer.pixmap().height() << std::endl;
}

void GameViewer::keyPressEvent(QKeyEvent *e)
{
    std::cerr << "In key press event!" << std::endl;
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
        
    default:
        e->ignore() ;
    }
    
    if(redraw)
    {
	mGameDrawer.update(*mGame,width(),height()) ;
	update() ;
    }
}

