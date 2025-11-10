#include <iostream>
#include <math.h>

#include <QPainter>
#include <QColor>

#include "GameDrawer.h"
#include "GameInterface.h"
#include "Level.h"
#include "BoulderGame.h"
#include "Config.h"

static const uint32_t mSceneCenterX = 0 ;
static const uint32_t mSceneCenterY = 0 ;

GameDrawer::GameDrawer(int W,int H)
	:mDrawBuffer(W,H)
{
    setCellSize(game_config->getValue("GRID_CELL_SIZE",30u)) ;
}

void GameDrawer::setCellSize(uint s)
{
    SQUARE_SIZE_IN_PIXELS = s;
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

void GameDrawer::drawButtons(const BoulderGame& /* game */,int /* w */,int /* h */,const std::vector<InterfaceButton*>& buttons)
{
    QPainter painter(&mDrawBuffer) ;

    for(uint32_t i=0;i<buttons.size();++i)
    {
		painter.drawPixmap(buttons[i]->x,buttons[i]->y,buttons[i]->pixmap.width(),buttons[i]->pixmap.height(),buttons[i]->pixmap) ;
        painter.drawRect(QRect(QPoint(buttons[i]->x,buttons[i]->y),QSize(30,30))) ;

        std::cerr << "Je dessine un bouton" << std::endl;
    }
}

void GameDrawer::update(const BoulderGame& game,int w,int h,GameMode m)
{
    SQUARE_SIZE_IN_PIXELS = game_config->getValue("GRID_CELL_SIZE",30u);

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
            
    for(uint i=0;i<game.currentState().sizeX();++i)
        for(uint j=0;j<game.currentState().sizeY();++j)
	    {
                    float mx = gameCoordToWindowCoordX(i) ;
                    float my = gameCoordToWindowCoordX(j) ;
                    
                    painter.drawPixmap(mx,my,getGameSprite( game.currentState()(i,j), resolution )) ;
	    }
    
    drawTextBox(painter,mDrawBuffer,50,150,QString::number(level.collectedDiamonds()) + " / " + QString::number(level.requiredDiamonds()));

    if(m == GAME_MODE_EDITOR)
    {
        QPen newPen(QColor::fromRgb(255,0,0), Qt::SolidLine);
        newPen.setWidth(64);
        painter.setPen(newPen);

        painter.drawRect(mDrawBuffer.rect());
    }
}

void GameDrawer::drawTextBox(QPainter& painter,QPixmap& pix,int i, int j, const QString& text) const
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QFont font(painter.font()) ;
    font.setPointSize(40) ;
    painter.setFont(font);

    int text_width  = QFontMetricsF(font).width(text);
    int text_height = QFontMetricsF(font).height();

    QRect rect(i, j, text_width*1.1, text_height*1.1);
    QColor bgColor(0, 0, 0, 128);  // black with 50% opacity

    painter.fillRect(rect, bgColor);

    painter.setPen(QRgb(0xffffffff)) ;
    painter.drawText(i+text_width*0.05,j+text_height*0.85,text);
}

QPixmap GameDrawer::pixmap() const
{
	return mDrawBuffer ;
}

QPixmap GameDrawer::getImageForObjectId(const Level::ObjectId& oid)
{
    switch(oid)
    {
    case Level::Wall:           return QPixmap(":/images/wall.png");
    case Level::Stone:          return QPixmap(":/images/stone.png");
    case Level::Earth:          return QPixmap(":/images/ground.png");
    case Level::Exit:           return QPixmap(":/images/opendoor.png");
    case Level::ClosedExit:     return QPixmap(":/images/door.png");
    case Level::Player:         return QPixmap(":/images/player.png");
    case Level::Diamond:        return QPixmap(":/images/diamond.png");
    case Level::Bomb:           return QPixmap(":/images/bomb.png");
    case Level::Explosion_01:   return QPixmap(":/images/explosion_01.png");
    case Level::Explosion_02:   return QPixmap(":/images/explosion_02.png");
    case Level::Explosion_03:   return QPixmap(":/images/explosion_03.png");
    case Level::Explosion_04:   return QPixmap(":/images/explosion_04.png");
    case Level::Bug_top:        return QPixmap(":/images/bug_top.png");
    case Level::Bug_left:       return QPixmap(":/images/bug_left.png");
    case Level::Bug_bottom:     return QPixmap(":/images/bug_bottom.png");
    case Level::Bug_right:      return QPixmap(":/images/bug_right.png");
    case Level::Moss:           return QPixmap(":/images/moss.png");
    case Level::MetalWall0:     return QPixmap(":/images/metal_wall.png");
    case Level::MetalWall1:     return QPixmap(":/images/metal_wall_s.png");

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

