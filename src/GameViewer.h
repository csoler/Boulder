#include <QPixmap>
#include <QGLViewer/qglviewer.h>

#include "Level.h"
#include "GameDrawer.h"

class BoulderGame ;

class GameViewer: public QWidget
{
public:
    GameViewer(QWidget *parent=NULL) ;

    void setGame(BoulderGame *g) ;

    // rederived methods from QGLViewer

    virtual void mousePressEvent(QMouseEvent *e) ;
    virtual void mouseReleaseEvent(QMouseEvent *e) ;
    virtual void mouseMoveEvent(QMouseEvent *e) ;
    virtual void resizeEvent(QResizeEvent *e) ;
    virtual void paintEvent(QPaintEvent *);
    
    virtual void keyPressEvent(QKeyEvent *e) ;
    
private:
    BoulderGame *mGame ;
    GameDrawer mGameDrawer ;

    float mSceneCenterX;
    float mSceneCenterY;
    float mOldMouseX;
    float mOldMouseY;

    bool mMousePressed ;
};
