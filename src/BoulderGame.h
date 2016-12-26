#include <QObject>
#include <QMutex>

#include "Level.h"

class QTimer ;

class BoulderGame: public QObject
{
    Q_OBJECT
    
	public:
		BoulderGame() ;

		void init(const Level& level) ;

		const Level& currentState() const { return mLevelState ;}
		Level& currentState() { return mLevelState ;}

		void stop();
		void start();
        
signals:
        	void changed() ;
            
protected slots:
        	void timerEvent() ;

	private:
			void explode(int i, int j);

		Level mLevelState ;
        
        	QTimer *mTimer ;
            	QMutex mtx ;
};
