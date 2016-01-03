#include <stdlib.h>
#include <assert.h>

#include "Level.h"

Level::Level()
{
	initDefault() ;
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

void Level::initDefault()
{
	mSizeX = 100 ;
	mSizeY = 80 ;

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

	for(int i=0;i<50;++i)
		operator()(lrand48()%mSizeX,lrand48()%mSizeY) = Level::Wall ;
    
	for(int i=0;i<50;++i)
		operator()(1+(lrand48()%(mSizeX-1)),1+(lrand48()%(mSizeY-1))) = Level::Stone ;
}


