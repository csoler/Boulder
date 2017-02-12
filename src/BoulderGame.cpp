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

void BoulderGame::explode(Level& level, int i, int j)
{
    for(int k=-1;k<2;++k)
        for(int l=-1;l<2;++l)
            if( i+k > 0 && i+k < mLevelState.sizeX()-1 && j+l > 0 && j+l < mLevelState.sizeY()-1)
				level(i+k,j+l) = Level::Explosion_01 ;
}

void BoulderGame::timerEvent()
{
    mtx.lock() ;
    bool should_redraw = false ;

    Level new_state(mLevelState) ;

    for(int i=0;i<mLevelState.sizeX();++i)
	    for(int j=0;j<mLevelState.sizeY();++j)
	    {
		    Level::ObjectId vij = mLevelState(i,j);
		    Level::ObjectId vijp1 = mLevelState(i,std::min(uint32_t(j+1), mLevelState.sizeY()-1));

		    if( (vij == Level::Stone || vij == Level::Diamond || vij == Level::Bomb) && j+1<mLevelState.sizeY() && vijp1 == Level::Void)
		    {
			    new_state(i,j+1) = vij ;
			    new_state(i,j) = Level::Void ;

                if(mLevelState(i,j+2) == Level::Bomb)
                    explode(new_state,i,j+2) ;

			    should_redraw = true;
		    }

		    if( (vij == Level::Stone || vij == Level::Diamond) && j+1<mLevelState.sizeY() && i+1<mLevelState.sizeX() && (vijp1 == Level::Stone || vijp1 == Level::Diamond) && mLevelState(i+1,j) == Level::Void && mLevelState(i+1,j+1) == Level::Void)
		    {
			    new_state(i+1,j) = vij;
			    new_state(i,j) = Level::Void ;
			    i++ ;
		    }

		    if( (vij == Level::Stone || vij == Level::Diamond) && j+1<mLevelState.sizeY() && i>1 && (vijp1 == Level::Stone || vijp1 == Level::Diamond) && mLevelState(i-1,j) == Level::Void && mLevelState(i-1,j+1) == Level::Void)
		    {
			    new_state(i-1,j) = vij;
			    new_state(i,j) = Level::Void ;
		    }

            if(vij == Level::Explosion_04) new_state(i,j) = Level::Void ;
            if(vij == Level::Explosion_03) new_state(i,j) = Level::Explosion_04 ;
            if(vij == Level::Explosion_02) new_state(i,j) = Level::Explosion_03 ;
            if(vij == Level::Explosion_01) new_state(i,j) = Level::Explosion_02 ;

            if(vij == Level::Bug_left)
            {
                if(mLevelState(i  ,j+1) == Level::Void)
                {
                    new_state(i  ,j+1) = Level::Bug_bottom ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(mLevelState(i-1,j) == Level::Void)
                {
                    new_state(i-1,j) = Level::Bug_left ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_top ;
            }

            if(vij == Level::Bug_bottom)
            {
                if(mLevelState(i+1,j  ) == Level::Void)
                {
                    new_state(i+1,j  ) = Level::Bug_right ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(mLevelState(i  ,j+1) == Level::Void)
                {
                    new_state(i  ,j+1) = Level::Bug_bottom ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_left ;
            }

			if(vij == Level::Bug_right)
            {
                if(mLevelState(i  ,j-1) == Level::Void)
                {
                    new_state(i  ,j-1) = Level::Bug_top ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(mLevelState(i+1,j  ) == Level::Void)
                {
                    new_state(i+1,j  ) = Level::Bug_right ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_bottom ;
            }

			if(vij == Level::Bug_top)
            {
                if(mLevelState(i-1,j  ) == Level::Void)
                {
                    new_state(i-1,j) = Level::Bug_left ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(mLevelState(i  ,j-1) == Level::Void)
                {
                    new_state(i  ,j-1) = Level::Bug_top ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_right ;
            }
	    }

    mLevelState = new_state ;

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



