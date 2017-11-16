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
#include <QLabel>
#include <QThread>
#include "../collection.h"

class SynchWorkerThread : public QThread {
    Q_OBJECT
public:
    SynchWorkerThread(Collection* collection) : collection(collection) {}

protected:
    void run();

private:
    Collection* collection;

    // Define signal:
    signals:
    void synchCompleted(int error);
};

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
    std::vector<const DustStrategy*> strategies;
    QLabel* statusLabel = NULL;
    SynchWorkerThread* synchWorkerThread = NULL;

    void updateCardTable(const DustStrategy* strategy);

public slots:
    void on_syncCompleted(int error);

private slots:
#ifdef __APPLE__
    void requestOSXDebugRights();
#endif

    void updateCardTable(int strategyIdx);
};

#endif // MAINWINDOW_H
