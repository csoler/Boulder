#include <QGLViewer/constraint.h>
#include <QMessageBox>

#include "PreferencesDialog.h"
#include "Config.h"
#include "GameDrawer.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    // init all values

    cellSize_SB->setValue(game_config->getValue("GRID_CELL_SIZE",50));

    // connect what should be connected.

    connect(cellSize_SB,SIGNAL(valueChanged(int)),this,SLOT(updateCellSize(int)));

    // save on close

    connect(this,SIGNAL(accepted()),this,SLOT(save()),Qt::DirectConnection);	// we use direct connection so that the widget is still here when we read values
}

void PreferencesDialog::updateCellSize(int)
{
    game_config->setValue("GRID_CELL_SIZE",cellSize_SB->value());
}

void PreferencesDialog::save()
{
    std::cerr << "Saving configuration..." << std::endl;

    game_config->save();
}



