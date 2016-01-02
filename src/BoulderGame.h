#include "Level.h"

class BoulderGame
{
	public:
		BoulderGame() ;

		void init(const Level& level) ;

		const Level& currentState() const { return mLevelState ;}

	private:
		Level mLevelState ;
};
