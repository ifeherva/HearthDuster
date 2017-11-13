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
#include <QCheckBox>
#include <QMessageBox>
#include "../db/cardsdb.h"
#include <security.h>
#include "../preferences/preferences.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionSync, SIGNAL(triggered(bool)), this, SLOT(SyncCollection()) );

    QString locale = Preferences::getLocale();
    QString carddbpath = QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath() + "/Cards/cardsDB."+locale +".json";

    // init database from locale
    if (CardsDb::initFromFile(carddbpath)) {
        QMessageBox::critical(this, tr("HearthDuster"), tr("Failed to load cards database file! Program will now exit."));
        QCoreApplication::exit();
    }

#ifdef __APPLE__
    if (Preferences::showMemoryReadingWarning()) {

        QCheckBox* cb = new QCheckBox("Show this warning at every start");
        cb->setCheckState(Qt::Checked);
        QMessageBox msgbox;
        msgbox.setModal(true);
        msgbox.setText(tr("HearthDuster needs elevated priviliages to read you card collection directly from Hearthstone.\n"
                          "The operating system might ask for a password. This is a completely safe operation."));
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.addButton(QMessageBox::Ok);
        msgbox.setDefaultButton(QMessageBox::Ok);
        msgbox.setCheckBox(cb);

        QObject::connect(cb, &QCheckBox::stateChanged, [this](int state) {
            Preferences::setShowMemoryReadingWarning(static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked);
        });

        msgbox.exec();
        delete cb;
    }

    requestOSXDebugRights();
#endif

    collection = new Collection();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->collection;
}

void MainWindow::syncCollection()
{
    auto error = this->collection->sync();

    switch (error) {
        case SynchError::HearthstoneNotRunning :
            QMessageBox::critical(this, tr("HearthDuster"), tr("Failed to synchronize card collection: Hearthstone is not running."));
            return;
        case SynchError::InvalidCollectionData :
        case SynchError::UnknownError :
            QMessageBox::critical(this, tr("HearthDuster"), tr("Failed to synchronize card collection: Unkown error occured."));
            return;
        case SynchError::NoError : break;
    }

    /*auto excessPreferGolden = this->collection->getCardsFor(DustStrategy::ExcessPlayableCardsPreferGoldStrategy);
    
    sort(excessPreferGolden.begin(), excessPreferGolden.end(),
         [](const Card* & a, const Card* & b) -> bool
    {
        return a->rarity > b->rarity;
    });
    
    for (auto card : excessPreferGolden) {
        //qDebug() << card->name;
    }*/
}

#ifdef __APPLE__
void MainWindow::requestOSXDebugRights()
{
    if (acquireTaskportRight() != 0) {
        QMessageBox::critical(this, tr("HearthDuster"), tr("Failed to acquire memory reading access rights! Program will now exit."));
        QCoreApplication::exit();
    }
}
#endif
