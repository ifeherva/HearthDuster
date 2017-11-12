//
//  mainwindow.h
//  Main window
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QErrorMessage>
#include <QMessageBox>

#include "../collection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Collection* collection = NULL;

    QErrorMessage* errormessage = NULL;

public slots:
    void SyncCollection();
    void ErrorOccured();

private slots:
#ifdef __APPLE__
    void AcquireTaskportRight();
#endif
};

#endif // MAINWINDOW_H
