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
            if( i+k > 0 && i+k < level.sizeX()-1 && j+l > 0 && j+l < level.sizeY()-1 && level(i+k,j+l)!=Level::Exit && level(i+k,j+l)!=Level::ClosedExit)
				level(i+k,j+l) = Level::Explosion_01 ;
}

bool BoulderGame::test_bestiole(const Level& L,int x,int y) const
{
    if(x < 0 && x >= L.sizeX())
        return false ;

    Level::ObjectId i = L(x,y) ;

    return (i == Level::Bug_top || i == Level::Bug_bottom || i == Level::Bug_left || i == Level::Bug_right) ;
}

void BoulderGame::timerEvent()
{
    mtx.lock() ;
    bool should_redraw = true ;

    Level new_state(mLevelState) ;
    bool mousse_coincee = true ;

    for(int i=0;i<mLevelState.sizeX();++i)
	    for(int j=0;j<mLevelState.sizeY();++j)
	    {
		    Level::ObjectId vij = mLevelState(i,j);
		    Level::ObjectId vijp1 = mLevelState(i,std::min(uint32_t(j+1), mLevelState.sizeY()-1));

            // Chute des pierres, des diamants et des bombes

		    if( (vij == Level::Stone || vij == Level::Diamond || vij == Level::Bomb) && j+1<mLevelState.sizeY() && vijp1 == Level::Void)
		    {
			    new_state(i,j+1) = vij ;
			    new_state(i,j) = Level::Void ;

                if(mLevelState(i,j+2) == Level::Bomb)
                    explode(new_state,i,j+2) ;

                if(mLevelState(i,j+2) == Level::Bug_bottom) { explode(new_state,i,j+2) ; mLevelState(i,j+2) = Level::Void ; }
                if(mLevelState(i,j+2) == Level::Bug_left)   { explode(new_state,i,j+2) ; mLevelState(i,j+2) = Level::Void ; }
                if(mLevelState(i,j+2) == Level::Bug_top)    { explode(new_state,i,j+2) ; mLevelState(i,j+2) = Level::Void ; }
                if(mLevelState(i,j+2) == Level::Bug_right)  { explode(new_state,i,j+2) ; mLevelState(i,j+2) = Level::Void ; }

			    should_redraw = true;
		    }

            // Bete qui touche la mousse

            if(vij == Level::Level::Bug_bottom || vij == Level::Level::Bug_top || vij == Level::Level::Bug_left || vij == Level::Level::Bug_right)
                if( (i>0 && mLevelState(i-1,j)==Level::Moss)
                 || (j>0 && mLevelState(i  ,j-1)==Level::Moss)
                 || (j+1 < mLevelState.sizeY() && mLevelState(i  ,j+1)==Level::Moss)
                 || (i+1 < mLevelState.sizeX() && mLevelState(i+1,j  )==Level::Moss))
				{
                    explode(new_state,i,j) ;
                    mLevelState(i,j) = Level::Void ;
                    vij = Level::Void;
                }

            // Chute de pierre a droite

		    if( (vij == Level::Stone || vij == Level::Diamond) && j+1<mLevelState.sizeY() && i+1<mLevelState.sizeX() && (vijp1 == Level::Stone || vijp1 == Level::Diamond) && mLevelState(i+1,j) == Level::Void && mLevelState(i+1,j+1) == Level::Void)
		    {
			    new_state(i+1,j) = vij;
			    new_state(i,j) = Level::Void ;
			    i++ ;
		    }

            // Chute de pierre a gauche

		    if( (vij == Level::Stone || vij == Level::Diamond) && j+1<mLevelState.sizeY() && i>1 && (vijp1 == Level::Stone || vijp1 == Level::Diamond) && mLevelState(i-1,j) == Level::Void && mLevelState(i-1,j+1) == Level::Void)
		    {
			    new_state(i-1,j) = vij;
			    new_state(i,j) = Level::Void ;
		    }

            // Explosion en cours

            if(vij == Level::Explosion_04) new_state(i,j) = Level::Void ;
            if(vij == Level::Explosion_03) new_state(i,j) = Level::Explosion_04 ;
            if(vij == Level::Explosion_02) new_state(i,j) = Level::Explosion_03 ;
            if(vij == Level::Explosion_01) new_state(i,j) = Level::Explosion_02 ;

            if(vij == Level::Bug_left)
            {
                if(new_state(i  ,j+1) == Level::Void)
                {
                    new_state(i  ,j+1) = Level::Bug_bottom ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(new_state(i-1,j) == Level::Void)
                {
                    new_state(i-1,j) = Level::Bug_left ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_top ;
            }

            // Deplacement des bestioles

            if(vij == Level::Bug_bottom)
            {
                if(new_state(i+1,j  ) == Level::Void)
                {
                    new_state(i+1,j  ) = Level::Bug_right ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(new_state(i  ,j+1) == Level::Void)
                {
                    new_state(i  ,j+1) = Level::Bug_bottom ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_left ;
            }

			if(vij == Level::Bug_right)
            {
                if(new_state(i  ,j-1) == Level::Void)
                {
                    new_state(i  ,j-1) = Level::Bug_top ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(new_state(i+1,j  ) == Level::Void)
                {
                    new_state(i+1,j  ) = Level::Bug_right ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_bottom ;
            }

			if(vij == Level::Bug_top)
            {
                if(new_state(i-1,j  ) == Level::Void)
                {
                    new_state(i-1,j) = Level::Bug_left ;
                    new_state(i  ,j) = Level::Void ;
                }
                else if(new_state(i  ,j-1) == Level::Void)
                {
                    new_state(i  ,j-1) = Level::Bug_top ;
                    new_state(i  ,j) = Level::Void ;
                }
                else
                    new_state(i,j) = Level::Bug_right ;
            }

            if(vij == Level::Earth || vij==Level::Void)
                if( (i > 0 && mLevelState(i-1,j)==Level::Moss)
                        || (j>0 && mLevelState(i  ,j-1)==Level::Moss)
                        || (j+1 < mLevelState.sizeY() && mLevelState(i  ,j+1)==Level::Moss)
                        || (i+1 < mLevelState.sizeX() && mLevelState(i+1,j  )==Level::Moss))
                {
                    mousse_coincee = false ;

                    if(drand48() < 0.01)
						new_state(i,j) = Level::Moss;
                }
		}

    if(mousse_coincee)
		for(int i=0;i<mLevelState.sizeX();++i)
			for(int j=0;j<mLevelState.sizeY();++j)
                if(new_state(i,j) == Level::Moss)
					new_state(i,j) = Level::Diamond ;

    int x = new_state.playerX() ;
    int y = new_state.playerY() ;

    if(test_bestiole(new_state,x+1,y) || test_bestiole(new_state,x-1,y) || test_bestiole(new_state,x,y+1) || test_bestiole(new_state,x,y-1))
		explode(new_state,x,y);

    mLevelState = new_state ;

    if(should_redraw)
    {
	    //std::cerr << "Emitting changed" << std::endl;
	    emit changed() ;
    }
    mtx.unlock() ;

    if(mLevelState.finished())
    {
	    QMessageBox::information(NULL,QString("Game is terminated"),QString("C'est gagne!")) ;
	    mTimer->stop() ;
    }
}



