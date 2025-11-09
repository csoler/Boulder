#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QGLViewer/qglviewer.h>

#include "GameInterface.h"
#include "Config.h"

Config *game_config = nullptr;
static QString CONFIG_FILENAME = "boulder.cfg";

int main(int argc,char *argv[])
{
	QApplication app(argc,argv) ;
    
    game_config = new Config(CONFIG_FILENAME.toStdString());

    GameInterface I;

	I.show();

	return app.exec() ;
}
