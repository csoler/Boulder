#include <QGLViewer/constraint.h>
#include <QMouseEvent>

#include "Level.h"
#include "GameViewer.h"
#include "BoulderGame.h"

static const uint32_t SQUARE_SIZE_IN_PIXELS = 50 ;

GameViewer::GameViewer(QWidget *parent)
	:QGLViewer(parent)
{
    mGame = NULL ;
    mMousePressed = false ;
    mSceneCenterX = 0 ;
    mSceneCenterY = 0 ;
    
    setMouseTracking(true) ;
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
    
    updateGL();
    }
    
    mOldMouseX = e->x() ;
    mOldMouseY = height()-1-e->y() ;
}

void GameViewer::setGame(BoulderGame *g)
{
    if(mGame != NULL)
        delete mGame ;
    
    mGame = g ;
    
    setSceneRadius(2*std::max(mGame->currentState().sizeX(),mGame->currentState().sizeY())) ;
    updateGL() ;
}
void GameViewer::draw()
{
    if(!mMousePressed)
        return ;
    
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST) ;
    
    glOrtho(0,width(),0,height(),1,-1) ;
    
    glClearColor(1,1,1,1) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    
    drawBackgroundGrid() ;
    drawGameContent() ;
    
    glFlush() ;
}

float GameViewer::gameCoordToWindowCoordX(int i) 
{ 
    return SQUARE_SIZE_IN_PIXELS * i + mSceneCenterX;
}
float GameViewer::gameCoordToWindowCoordY(int i) 
{ 
    return SQUARE_SIZE_IN_PIXELS * i + mSceneCenterY;
}
void GameViewer::drawBackgroundGrid()
{
    if(!mGame)
	    return ;

    const Level& level(mGame->currentState()) ;
    glColor3f(1,0.7,0.5) ;

    glBegin(GL_LINES) ;

    for(uint i=0;i<=level.sizeX();++i)
    {
	    glVertex2f(gameCoordToWindowCoordX(0),gameCoordToWindowCoordY(i)) ;
	    glVertex2f(gameCoordToWindowCoordX(level.sizeY()),gameCoordToWindowCoordY(i)) ;
    }

    for(uint j=0;j<=level.sizeY();++j)
    {
	    glVertex2f(gameCoordToWindowCoordX(j),gameCoordToWindowCoordY(0)) ;
	    glVertex2f(gameCoordToWindowCoordX(j),gameCoordToWindowCoordY(level.sizeX())) ;
    }

    glEnd() ;
}

void GameViewer::drawGameContent()
{
    if(!mGame)
        return ;
    
    int resolution = SQUARE_SIZE_IN_PIXELS ;
            
    for(int i=0;i<mGame->currentState().sizeX();++i)
	    for(int j=0;j<mGame->currentState().sizeY();++j)
	    {
                    float mx = gameCoordToWindowCoordX(i) ;
                    float my = gameCoordToWindowCoordX(j) ;
                    
                    pasteSprite(my,my,getGameSprite( mGame->currentState()(i,j), resolution )) ;
	    }
}

QPixmap GameViewer::getImageForObjectId(const Level::ObjectId& oid)
{
    switch(oid)
    {
    //case Level::Wall: return QPixmap(":/images/wall.png").toPixmap() ;
    default:
        QColor col = QColor::fromHsv(oid*20,255,255) ;
        QPixmap pix(128,128) ;
        pix.fill(col) ;
        
        return pix ;
    }
}

QPixmap GameViewer::getGameSprite(const Level::ObjectId& oid,int resolution)
{
    // store them in a cache
    
    uint32_t item_id = (int)oid + 1000*resolution ;	// this should allow enough object ids!
    
    ImageCache::const_iterator it = mImageCache.find(item_id) ;
    
    if(it == mImageCache.end())
    {
        std::cerr << "Image for id " << oid << " and resolution " << resolution << " not in cache. Creating!" << std::endl;
        
        QPixmap pix = getImageForObjectId(oid).scaled(QSize(resolution,resolution),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        
        mImageCache[item_id] = pix ;
        
        return pix ;
    }
    else
        return it->second ;
}

void GameViewer::pasteSprite(float x,float y,const QPixmap& pix)
{
    std::cerr << "Pasting sprite" << std::endl;
    glMatrixMode(GL_MODELVIEW) ;
    glPushMatrix();
    glLoadIdentity() ;
    
    glTranslatef(x,y,0) ;
    
    glDrawPixels(pix.width(),pix.height(),GL_RGB,GL_UNSIGNED_BYTE,const_cast<QPixmap*>(&pix)->data_ptr().data()) ;
    
    glPopMatrix();
}

// New drawing method:
// - draw all level in a pixmap or a QImage
// - draw the image in the screen at a scrolled position using glDrawPixels()
// - inthe pixmap, only draw what has changed.


