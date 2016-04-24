#include <iostream>
#include <QTimer>
#include <QMessageBox>
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
    mTimer->start(150) ;
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
	    for(int j=mLevelState.sizeY()-1;j>=0;--j)
	    {
		    Level::ObjectId vij = mLevelState(i,j);
		    Level::ObjectId vijp1 = mLevelState(i,std::min(uint32_t(j+1), mLevelState.sizeY()-1));

		    if( (vij == Level::Stone || vij == Level::Diamond) && j+1<mLevelState.sizeY() && vijp1 == Level::Void)
		    {
			    mLevelState(i,j+1) = vij ;
			    mLevelState(i,j) = Level::Void ;

			    should_redraw = true;
		    }

		    if( (vij == Level::Stone || vij == Level::Diamond) && j+1<mLevelState.sizeY() && i+1<mLevelState.sizeX() && (vijp1 == Level::Stone || vijp1 == Level::Diamond) && mLevelState(i+1,j) == Level::Void && mLevelState(i+1,j+1) == Level::Void)
		    {
			    mLevelState(i+1,j) = vij;
			    mLevelState(i,j) = Level::Void ;
			    i++ ;
		    }

		    if( (vij == Level::Stone || vij == Level::Diamond) && j+1<mLevelState.sizeY() && i>1 && (vijp1 == Level::Stone || vijp1 == Level::Diamond) && mLevelState(i-1,j) == Level::Void && mLevelState(i-1,j+1) == Level::Void)
		    {
			    mLevelState(i-1,j) = vij;
			    mLevelState(i,j) = Level::Void ;
		    }
	    }

    if(should_redraw)
    {
	    std::cerr << "Emitting changed" << std::endl;
	    emit changed() ;
    }
    mtx.unlock() ;

    if(mLevelState.finished())
    {
	    QMessageBox::information(NULL,QString("Game is terminated"),QString("C'est gagne!")) ;
	    mTimer->stop() ;
    }
}



