#pragma once

#include <QPixmap>

class BoulderGame ;

class GameDrawer
{
	public: 
		GameDrawer(int W,int H);

		virtual void update(const BoulderGame& game,int w,int h) ;
		virtual QPixmap pixmap() const ;

	private:
		QPixmap mDrawBuffer ;
        
	float gameCoordToWindowCoordX(int i)  ;
	float gameCoordToWindowCoordY(int j)  ;
    
//private:
//    void drawBackgroundGrid();
//    void drawGameContent();
//    void pasteSprite(float x,float y,const QPixmap& pix) ;
//    
//    float gameCoordToWindowCoordY(int i) ;
//    float gameCoordToWindowCoordX(int i) ;
//
//    QPixmap getImageForObjectId(const Level::ObjectId &oid);
//    QPixmap getGameSprite(const Level::ObjectId& oid,int resolution);
//
//    typedef std::map<uint64_t,QPixmap> ImageCache ;
//    
//    ImageCache mImageCache ;

};

