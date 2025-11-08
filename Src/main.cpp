#include <QApplication>
#include <QWidget>
#include <QGLViewer/qglviewer.h>

#include "GameInterface.h"
#include "Config.h"

Config *game_config = nullptr;

int main(int argc,char *argv[])
{
	QApplication app(argc,argv) ;
    
    game_config = new Config("boulder.cfg");

    GameInterface I;

	I.show();

	return app.exec() ;
}
