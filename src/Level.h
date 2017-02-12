#pragma once

#include <vector>
#include <stdint.h>

class Level
{
	public:
		Level();
		Level(const std::string& fname);

		typedef enum {
							Void         = 0x00,
							Wall         = 0x01,
							Stone        = 0x02,
							Earth        = 0x03,
							Player       = 0x04,
							ClosedExit   = 0x05,
							Diamond      = 0x06,
							Exit         = 0x07,
							Bomb         = 0x08,
							Explosion_01 = 0x09,
							Explosion_02 = 0x0a,
							Explosion_03 = 0x0b,
							Explosion_04 = 0x0c,
							Bug_top      = 0x0d,
							Bug_right    = 0x0e,
							Bug_bottom   = 0x0f,
							Bug_left     = 0x10
		} ObjectId;

        	typedef enum {
            						Left  = 0x00,
            						Top   = 0x01,
            						Right = 0x02,
            						Bottom= 0x03
		} MoveDirection ;
        
		void initDefault();

        	bool finished() const { return mFinished ; }
        	bool allDiamondCollected() const { return mAllDiamondsCollected ; }
            
		ObjectId operator()(uint32_t i,uint32_t j) const ;
		ObjectId& operator()(uint32_t i,uint32_t j) ;

        	void movePlayer(MoveDirection d) ;
            
        	uint32_t sizeX() const { return mSizeX ; }
        	uint32_t sizeY() const { return mSizeY ; }
            
            	void save(const std::string& fname) const ;
            	void load(const std::string& fname) ;
                
                uint32_t collectedDiamonds() const { return mCollectedDiamonds ; }
                
	private:
		uint32_t mSizeX ;
		uint32_t mSizeY ;

		uint32_t mExitX ;
		uint32_t mExitY ;
        
        	uint32_t mPlayerX ;
        	uint32_t mPlayerY ;
            
		std::vector<ObjectId> mContent ;
        	bool mFinished ;
        	bool mAllDiamondsCollected ;
            
            	uint32_t mDiamondsToCollect ;
            	uint32_t mCollectedDiamonds ;
};
