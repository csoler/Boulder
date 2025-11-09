#pragma once

#include <QDialog>
#include "ui_PreferencesDialog.h"

class PreferencesDialog: public QDialog, public Ui::PreferencesDialog
{
    Q_OBJECT
    
public:
    PreferencesDialog(QWidget *parent=NULL) ;

private slots:
    void save();
    void updateCellSize(int);
};
