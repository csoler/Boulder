#include <QPixmap>
#include <QGLViewer/qglviewer.h>

#include "Level.h"
#include "GameDrawer.h"

class BoulderGame ;

class GameViewer: public QWidget
{
    Q_OBJECT
    
public:
	typedef enum { GAME_MODE_GAME, GAME_MODE_EDITOR, GAME_MODE_NONE } GameMode ;
        
    GameViewer(QWidget *parent=NULL) ;

    void setGame(BoulderGame *g) ;
    void setCurrentMode(GameMode m) ;

    // rederived methods from QGLViewer

    virtual void mousePressEvent(QMouseEvent *e) ;
    virtual void mouseReleaseEvent(QMouseEvent *e) ;
    virtual void mouseMoveEvent(QMouseEvent *e) ;
    virtual void resizeEvent(QResizeEvent *e) ;
    virtual void paintEvent(QPaintEvent *);
    
    virtual void keyPressEvent(QKeyEvent *e) ;
    
public slots:
    void reDraw() ;
    
private:
    void pixelCoordinatesToGameCoordinate(int x,int y,int& i,int& j) const;

    BoulderGame *mGame ;
    GameDrawer mGameDrawer ;

    float mSceneCenterX;
    float mSceneCenterY;
    float mOldMouseX;
    float mOldMouseY;

    bool mMousePressed ;
    GameMode mCurrentMode ;
            
};
