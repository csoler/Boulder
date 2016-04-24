#include <iostream>
#include <math.h>

#include <QPainter>
#include <QColor>

#include "GameDrawer.h"
#include "Level.h"
#include "BoulderGame.h"

static const uint32_t SQUARE_SIZE_IN_PIXELS = 50 ;
static const uint32_t mSceneCenterX = 0 ;
static const uint32_t mSceneCenterY = 0 ;

GameDrawer::GameDrawer(int W,int H)
	:mDrawBuffer(W,H)
{
}

int GameDrawer::windowCoordToGameCoordX(float x) 
{ 
    return (int)floor((x - mSceneCenterX) / SQUARE_SIZE_IN_PIXELS) ;
}
int GameDrawer::windowCoordToGameCoordY(float y) 
{ 
    return (int)floor((y - mSceneCenterY) / SQUARE_SIZE_IN_PIXELS) ;
}
float GameDrawer::gameCoordToWindowCoordX(int i) 
{ 
    return SQUARE_SIZE_IN_PIXELS * i + mSceneCenterX;
}
float GameDrawer::gameCoordToWindowCoordY(int i) 
{ 
    return SQUARE_SIZE_IN_PIXELS * i + mSceneCenterY;
}


void GameDrawer::update(const BoulderGame& game,int w,int h)
{
    if(mDrawBuffer.width() != w || mDrawBuffer.height() != h)
        mDrawBuffer = QPixmap(w,h) ;
    
    mDrawBuffer.fill(QColor::fromRgb(0,0,0)) ;
    
    QPainter painter(&mDrawBuffer) ;
    
    painter.setPen(QColor::fromRgb(0,0,0)) ;
    
    const Level& level(game.currentState()) ;

    for(uint i=0;i<=level.sizeX();++i)
        painter.drawLine(QPointF(gameCoordToWindowCoordX(0),gameCoordToWindowCoordY(i)),QPointF(gameCoordToWindowCoordX(level.sizeY()),gameCoordToWindowCoordY(i))) ;

    for(uint j=0;j<=level.sizeY();++j)
	    painter.drawLine(QPointF(gameCoordToWindowCoordX(j),gameCoordToWindowCoordY(0)),  QPointF(gameCoordToWindowCoordX(j),gameCoordToWindowCoordY(level.sizeX()))) ;
    
    int resolution = SQUARE_SIZE_IN_PIXELS ;
            
    for(int i=0;i<game.currentState().sizeX();++i)
	    for(int j=0;j<game.currentState().sizeY();++j)
	    {
                    float mx = gameCoordToWindowCoordX(i) ;
                    float my = gameCoordToWindowCoordX(j) ;
                    
                    painter.drawPixmap(mx,my,getGameSprite( game.currentState()(i,j), resolution )) ;
	    }
}

QPixmap GameDrawer::pixmap() const
{
	return mDrawBuffer ;
}

QPixmap GameDrawer::getImageForObjectId(const Level::ObjectId& oid)
{
    switch(oid)
    {
    case Level::Wall:  return QPixmap(":/images/wall.png");
    case Level::Stone: return QPixmap(":/images/stone.png");
    case Level::Earth: return QPixmap(":/images/ground.png");
    case Level::Exit:  return QPixmap(":/images/door.png");
    case Level::Player:return QPixmap(":/images/player.png");
    case Level::Diamond:return QPixmap(":/images/diamond.png");
            
    case Level::Void:  {
	    QColor col = QColor::fromHsv(0,255,0) ;
	    QPixmap pix(128,128) ;
	    pix.fill(col) ;
	    return pix ;
    }
    default:
        QColor col = QColor::fromHsv(oid*20,255,255) ;
        QPixmap pix(128,128) ;
        pix.fill(col) ;
        
        return pix ;
    }
}

QPixmap GameDrawer::getGameSprite(const Level::ObjectId& oid,int resolution)
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

//void GameDrawer::drawGameContent(QPainter& painter)
//{
//    if(!mGame)
//        return ;
//    
//}
