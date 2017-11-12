//
//  mainwindow.cpp
//  Main window
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include "../Database/cardsdb.h"
#include <security.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QErrorMessage* errormessage = new QErrorMessage(this);

    connect(ui->actionSync, SIGNAL(triggered(bool)), this, SLOT(SyncCollection()) );
    connect(errormessage, SIGNAL(accepted()), this, SLOT(ErrorOccured())  );

    //TODO: get locale from preferences
    QString locale = "enUS";

#ifdef __APPLE__
    QString carddbpath = QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath() + "/../Resources/Cards/cardsDB."+locale +".json";
#else
    QString carddbpath = QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath() + "/Cards/cardsDB."+locale +".json";
#endif
    // init database from locale
    if (CardsDb::InitFromFile(carddbpath)) {
        // TODO: proper error dialog
        errormessage->showMessage(tr("Failed to load cards database file!"));
    }

#ifdef __APPLE__
    if (acquireTaskportRight() != 0) {
        errormessage->showMessage(tr("Failed acquire memory reading access rights!"));
    }
#endif
    collection = new Collection();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->collection;
    delete this->errormessage;
}

void MainWindow::SyncCollection()
{
    this->collection->sync();

    auto excessPreferGolden = this->collection->getCardsFor(DustStrategy::ExcessPlayableCardsPreferGoldStrategy);
    
    sort(excessPreferGolden.begin(), excessPreferGolden.end(),
         [](const Card* & a, const Card* & b) -> bool
    {
        return a->rarity > b->rarity;
    });
    
    for (auto card : excessPreferGolden) {
        qDebug() << card->name;
    }
}

void MainWindow::ErrorOccured()
{
    QCoreApplication::exit();
}