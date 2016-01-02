#include <QPixmap>
#include <QGLViewer/qglviewer.h>

#include "Level.h"

class BoulderGame ;

class GameViewer: public QGLViewer
{
public:
    GameViewer(QWidget *parent=NULL) ;

    virtual void draw() ;

    void setGame(BoulderGame *g) ;

    // rederived methods from QGLViewer

    virtual void mousePressEvent(QMouseEvent *e) ;
    virtual void mouseReleaseEvent(QMouseEvent *e) ;
    virtual void mouseMoveEvent(QMouseEvent *e) ;
private:
    void drawBackgroundGrid();
    void drawGameContent();
    void pasteSprite(float x,float y,const QPixmap& pix) ;
    
    float gameCoordToWindowCoordY(int i) ;
    float gameCoordToWindowCoordX(int i) ;

    QPixmap getImageForObjectId(const Level::ObjectId &oid);
    QPixmap getGameSprite(const Level::ObjectId& oid,int resolution);

    typedef std::map<uint64_t,QPixmap> ImageCache ;
    
    ImageCache mImageCache ;

    BoulderGame *mGame ;

    float mSceneCenterX;
    float mSceneCenterY;
    float mOldMouseX;
    float mOldMouseY;

    bool mMousePressed ;
};
