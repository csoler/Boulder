#pragma once

#include <vector>
#include <stdint.h>

class Level
{
	public:
		Level();

		typedef enum {
							Void    = 0x00,
							Wall    = 0x01,
							Stone   = 0x02,
							Earth   = 0x03
		} ObjectId;

		void initDefault();

		ObjectId operator()(uint32_t i,uint32_t j) const ;
		ObjectId& operator()(uint32_t i,uint32_t j) ;

        	uint32_t sizeX() const { return mSizeX ; }
        	uint32_t sizeY() const { return mSizeY ; }
	private:
		uint32_t mSizeX ;
		uint32_t mSizeY ;

		std::vector<ObjectId> mContent ;
};
