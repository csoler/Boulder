#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>

#include "Level.h"

Level::Level()
{
	initDefault() ;
    	mFinished = false ;
}
Level::Level(const std::string& fname)
{
	load(fname) ;
    	mFinished = false ;
}

Level::ObjectId& Level::operator()(uint32_t i,uint32_t j) 
{
	assert(i < mSizeX) ;
	assert(j < mSizeY) ;

	return mContent[mSizeX * j + i] ;
}
Level::ObjectId Level::operator()(uint32_t i,uint32_t j) const
{
	assert(i < mSizeX) ;
	assert(j < mSizeY) ;

	return mContent[mSizeX * j + i] ;
}

void Level::movePlayer(MoveDirection d)
{
    if(mFinished)
        return ;
    
    uint32_t old_player_x = mPlayerX ;
    uint32_t old_player_y = mPlayerY ;
    
    uint32_t new_player_x = mPlayerX;
    uint32_t new_player_y = mPlayerY;
    
    switch(d)
    {
    case Left: if(mPlayerX > 0) new_player_x-- ;
	    break ;
    case Right: if(mPlayerX < mSizeX-1) new_player_x++ ;
	    break ;
    case Top:  if(mPlayerY > 0) new_player_y-- ;
	    break ;
    case Bottom: if(mPlayerY < mSizeY-1) new_player_y++ ;
	    break ;
    default:
	    std::cerr << "(EE) untreated case!" << std::endl;
    }
    
    if(operator()(new_player_x,new_player_y) == Level::Earth || operator()(new_player_x,new_player_y) == Level::Void || operator()(new_player_x,new_player_y) == Level::Exit)
    {
        if(operator()(new_player_x,new_player_y) == Level::Exit)
            mFinished = true ;
        
            mPlayerX = new_player_x ;
            mPlayerY = new_player_y ;
            
	    operator()(old_player_x,old_player_y) = Level::Void ;
	    operator()(mPlayerX,mPlayerY) = Level::Player ;
            
    }
    if(operator()(new_player_x,new_player_y) == Level::Stone 
            && d == Right 
            && new_player_x + 1 < mSizeX-1
            && operator()(new_player_x + 1,new_player_y) == Level::Void)
    {
        operator()(new_player_x + 1,new_player_y) = Level::Stone ;
        operator()(new_player_x    ,new_player_y) = Level::Player ;
        operator()(mPlayerX        ,mPlayerY    ) = Level::Void ;
        
        mPlayerX = new_player_x ;
        mPlayerY = new_player_y ;
    }
    if(operator()(new_player_x,new_player_y) == Level::Stone 
            && d == Left 
            && new_player_x - 1 > 0
            && operator()(new_player_x - 1,new_player_y) == Level::Void)
    {
        operator()(new_player_x - 1,new_player_y) = Level::Stone ;
        operator()(new_player_x    ,new_player_y) = Level::Player ;
        operator()(mPlayerX        ,mPlayerY    ) = Level::Void ;
        
        mPlayerX = new_player_x ;
        mPlayerY = new_player_y ;
    }
}
void Level::load(const std::string &fname) 
{
   std::ifstream is(fname.c_str(),std::ios_base::in) ;
   
   is >> mSizeX ;
   is >> mSizeY ;
   is >> mPlayerX ;
   is >> mPlayerY ;

   std::cerr << "Loading game of size " << mSizeX << " x " << mSizeY << std::endl;
   
   mContent.resize(mSizeX*mSizeY,Level::Earth) ;
   
   for(int i=0;i<mSizeY;++i)
   {
       for(int j=0;j<mSizeX;++j)
       {
           unsigned char s ;
           is >> s ;
           operator()(j,i) = Level::ObjectId(s - ' ');
       }
       
       //unsigned char s ;
       //is >> s ;
   }
   is.close() ;
}
void Level::save(const std::string &fname) const
{
   std::ofstream of(fname.c_str()) ;
   
   of << mSizeX << " " << mSizeY << std::endl;
   of << mPlayerX << " " << mPlayerY << std::endl;

   for(int i=0;i<mSizeY;++i)
   {
       for(int j=0;j<mSizeX;++j)
           of << (unsigned char)(operator()(j,i) + ' ');
       
       of << std::endl;
   }
   of.close() ;
}

void Level::initDefault()
{
	mSizeX = 50 ;
	mSizeY = 30 ;

	mContent.resize(mSizeX*mSizeY,Level::Earth) ;

	for(int i=0;i<mSizeX;++i)
	{
		operator()(i,0) = Level::Wall ;
		operator()(i,mSizeY-1) = Level::Wall ;
	}
	for(int j=0;j<mSizeY;++j)
	{
		operator()(0,j) = Level::Wall ;
		operator()(mSizeX-1,j) = Level::Wall ;
	}

	for(int i=0;i<100;++i)
		operator()(lrand48()%mSizeX,lrand48()%mSizeY) = Level::Wall ;
    
	for(int i=0;i<100;++i)
		operator()(1+(lrand48()%(mSizeX-2)),1+(lrand48()%(mSizeY-2))) = Level::Stone ;
    
    	mPlayerX = 4 ;
    	mPlayerY = 4 ;
        
    	operator()(mPlayerX,mPlayerY) = Level::Player ;
        operator()(mSizeX-3,mSizeY-3) = Level::Exit ;
}


