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

void BoulderGame::explode(int i, int j)
{
    for(int k=-1;k<2;++k)
        for(int l=-1;l<2;++l)
            if( i+k > 0 && i+k < mLevelState.sizeX()-1 && j+l > 0 && j+l < mLevelState.sizeY()-1)
				mLevelState(i+k,j+l) = Level::Explosion_01 ;
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

		    if( (vij == Level::Stone || vij == Level::Diamond || vij == Level::Bomb) && j+1<mLevelState.sizeY() && vijp1 == Level::Void)
		    {
			    mLevelState(i,j+1) = vij ;
			    mLevelState(i,j) = Level::Void ;

                if(mLevelState(i,j+2) == Level::Bomb)
                    explode(i,j+2) ;

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

            if(vij == Level::Explosion_04) mLevelState(i,j) = Level::Void ;
            if(vij == Level::Explosion_03) mLevelState(i,j) = Level::Explosion_04 ;
            if(vij == Level::Explosion_02) mLevelState(i,j) = Level::Explosion_03 ;
            if(vij == Level::Explosion_01) mLevelState(i,j) = Level::Explosion_02 ;
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



