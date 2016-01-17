#include <iostream>
#include <QTimer>
#include "BoulderGame.h"

BoulderGame::BoulderGame()
{
    mTimer = NULL ;
}

void BoulderGame::init(const Level& level)
{
    // init level content
    
    mtx.lock() ;
    
    mLevelState = level ;
    mtx.unlock() ;
    
    // init player' position, state, etc
}

void BoulderGame::start()
{
    if(mTimer != NULL)
        delete mTimer ;
    
    mTimer = new QTimer ;
    
    QObject::connect(mTimer,SIGNAL(timeout()),this,SLOT(timerEvent())) ;
    mTimer->start(200) ;
}
void BoulderGame::stop()
{
    mTimer->stop() ;
}

void BoulderGame::timerEvent()
{
    mtx.lock() ;
    bool should_redraw = false ;
    
    for(int i=0;i<mLevelState.sizeX();++i)
	    for(int j=0;j<mLevelState.sizeY();++j)
            	if(mLevelState(i,j) == Level::Stone && mLevelState(i,j+1) == Level::Void)
 			{
                    mLevelState(i,j+1) = Level::Stone ;
                    mLevelState(i,j) = Level::Void ;
                    
                    should_redraw = true;
                }
    
    if(should_redraw)
    {
        std::cerr << "Emitting changed" << std::endl;
        emit changed() ;
    }
    mtx.unlock() ;
}
