#pragma once

#include <map>
#include <QPixmap>

#include "Level.h"
#include "GameDefs.h"

class BoulderGame ;
class InterfaceButton;

class GameDrawer
{
public:
    GameDrawer(int W,int H);

    virtual void update(const BoulderGame& game, int w, int h, GameMode m) ;
    virtual void drawButtons(const BoulderGame& game,int w,int h,const std::vector<InterfaceButton*>& buttons);
    virtual QPixmap pixmap() const ;

    float gameCoordToWindowCoordX(int i)  ;
    float gameCoordToWindowCoordY(int j)  ;

    int windowCoordToGameCoordX(float x) ;
    int windowCoordToGameCoordY(float y) ;

    void setCellSize(uint s);

private:
    QPixmap mDrawBuffer ;

    void drawTextBox(QPainter &painter, QPixmap &pix, int i, int j, const QString& text) const;
    QPixmap getImageForObjectId(const Level::ObjectId &oid);
    QPixmap getGameSprite(const Level::ObjectId& oid,int resolution);

    typedef std::map<uint64_t,QPixmap> ImageCache ;

    ImageCache mImageCache ;

    uint32_t SQUARE_SIZE_IN_PIXELS ;
};

