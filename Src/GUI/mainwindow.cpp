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
#include "../db/cardsdb.h"
#include <security.h>
#include "../preferences/preferences.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    errormessage = new QErrorMessage(this);

    connect(ui->actionSync, SIGNAL(triggered(bool)), this, SLOT(SyncCollection()) );
    connect(errormessage, SIGNAL(accepted()), this, SLOT(ErrorOccured())  );

    QString locale = Preferences::GetLocale();

    QString carddbpath = QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath() + "/Cards/cardsDB."+locale +".json";

    // init database from locale
    if (CardsDb::InitFromFile(carddbpath)) {
        // TODO: proper error dialog
        errormessage->showMessage(tr("Failed to load cards database file!"));
    }

#ifdef __APPLE__
    if (Preferences::ShowMemoryReadingWarning()) {

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
            Preferences::SetShowMemoryReadingWarning(static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked);
        });

        msgbox.exec();
        delete cb;
    }

    AcquireTaskportRight();
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
        //qDebug() << card->name;
    }
}

void MainWindow::ErrorOccured()
{
    QCoreApplication::exit();
}

#ifdef __APPLE__
void MainWindow::AcquireTaskportRight()
{
    if (acquireTaskportRight() != 0) {
        errormessage->showMessage(tr("Failed to acquire memory reading access rights!"));
    }
}
#endif
