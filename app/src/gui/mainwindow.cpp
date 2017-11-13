//
//  mainwindow.cpp
//  Main window
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <algorithm>
#include <QFileInfo>
#include <QDir>
#include <QCheckBox>
#include <QMessageBox>
#include "../db/cardsdb.h"
#include <security.h>
#include "../preferences/preferences.h"
#include "../strategies/strategies.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // build UI
    ui->setupUi(this);
    connect(ui->syncButton, SIGNAL(clicked(bool)), this, SLOT(syncCollection()) );

    INSTALL_STRATEGIES(strategies)

    for (unsigned int i = 0; i < strategies.size(); i++) {
        ui->strategiesComboBox->addItem(strategies[i]->getName());
        ui->strategiesComboBox->setItemData(i, strategies[i]->getDescription(), Qt::ToolTipRole);
    }
    connect(ui->strategiesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCardTable(int)) );

    ui->resultsTableWidget->setHorizontalHeaderLabels(QStringList() << "Name" << "Normal" << "Golden" << "Dust");

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
    for (auto strategy: strategies) {
        delete strategy;
    }
    strategies.clear();
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

    updateCardTable(strategies[ui->strategiesComboBox->currentIndex()]);
}

void MainWindow::updateCardTable(int strategyIdx)
{
    updateCardTable(strategies[strategyIdx]);
}

void MainWindow::updateCardTable(const DustStrategy* strategy)
{
    auto cardsList = this->collection->getCardsFor(strategy, true);
    ui->resultsTableWidget->clear();
    ui->resultsTableWidget->setSortingEnabled(false);
    ui->resultsTableWidget->setRowCount(cardsList.size());

    for (unsigned int row = 0; row < cardsList.size(); row++) {
        auto card = cardsList[row];
        ui->resultsTableWidget->setItem(row, 0, new QTableWidgetItem(card.cardData->name));
        ui->resultsTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(card.superfluous_normal)));
        ui->resultsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(card.superfluous_premium)));
        ui->resultsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(card.dustValue())));
    }
    ui->resultsTableWidget->setSortingEnabled(true);
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
