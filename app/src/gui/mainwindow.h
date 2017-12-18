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
#include "../gui/aboutdialog.h"

#define SYNCH_UPDATE_INTERVAL_START 5000
#define SYNCH_UPDATE_INTERVAL_SUCCESS 10000
#define SYNCH_UPDATE_INTERVAL_ERROR 1000

class SynchWorkerThread : public QThread {
    Q_OBJECT
public:
    SynchWorkerThread(Collection* collection) : collection(collection) {}

protected:
    void run();

private:
    Collection* collection;
    unsigned int synchUpdateInterval = SYNCH_UPDATE_INTERVAL_START;

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
    AboutDialog* aboutBox = NULL;

    void updateCardTable(const DustStrategy* strategy);

public slots:
    void on_syncCompleted(int error);
    void on_strategyError(const DustStrategy* strategy, QString error);
    void on_strategyMessage(const DustStrategy* strategy, QString message);
    void on_exportCollectionClicked();

private slots:
#ifdef __APPLE__
    void requestOSXDebugRights();
#endif

    void updateCardTable(int strategyIdx);
    void showAboutBox();
};

#endif // MAINWINDOW_H
