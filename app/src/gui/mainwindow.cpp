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

    ui->resultsTableWidget->clear();

    statusLabel = new QLabel("");
    ui->statusBar->addPermanentWidget(statusLabel);

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

QBrush getBrush(const CardRarity& rarity)
{
    switch (rarity) {
        case RARITY_FREE:
        case RARITY_COMMON:
            return QBrush(QColor(233, 239, 235));
        case RARITY_RARE:
            return QBrush(QColor(27, 95, 178));
        case RARITY_EPIC:
            return QBrush(QColor(191, 45, 204));
        case RARITY_LEGENDARY:
            return QBrush(QColor(208, 124, 10));
    }
}

void MainWindow::updateCardTable(const DustStrategy* strategy)
{
    auto cardsList = this->collection->getCardsFor(strategy, true);
    if (cardsList.size() > 0) {
        ui->resultsTableWidget->setEnabled(true);
    } else {
        ui->resultsTableWidget->setEnabled(false);
    }

    ui->resultsTableWidget->clear();
    ui->resultsTableWidget->setSortingEnabled(false);
    ui->resultsTableWidget->setColumnCount(4);
    ui->resultsTableWidget->setRowCount(cardsList.size());
    ui->resultsTableWidget->setHorizontalHeaderLabels(QStringList() << "Card" << "Normal" << "Golden" << "Dust");
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    unsigned int sumDust = 0;
    for (unsigned int row = 0; row < cardsList.size(); row++) {
        auto card = cardsList[row];
        auto bgBrush = getBrush(card.cardData->rarity);

        auto cardTableWidgetItem = new QTableWidgetItem(card.cardData->name);
        cardTableWidgetItem->setBackground(bgBrush);
        ui->resultsTableWidget->setItem(row, 0, cardTableWidgetItem);

        auto normalCountTableWidgetItem = new QTableWidgetItem(QString::number(card.superfluous_normal));
        normalCountTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        normalCountTableWidgetItem->setBackground(bgBrush);
        ui->resultsTableWidget->setItem(row, 1, normalCountTableWidgetItem);

        auto premiumCountTableWidgetItem = new QTableWidgetItem(QString::number(card.superfluous_premium));
        premiumCountTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        premiumCountTableWidgetItem->setBackground(bgBrush);
        ui->resultsTableWidget->setItem(row, 2, premiumCountTableWidgetItem);

        unsigned int dustValue = card.dustValue();
        auto dustValueTableWidgetItem = new QTableWidgetItem(QString::number(dustValue));
        dustValueTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dustValueTableWidgetItem->setBackground(bgBrush);
        ui->resultsTableWidget->setItem(row, 3, dustValueTableWidgetItem);
        sumDust += dustValue;
    }
    ui->resultsTableWidget->setSortingEnabled(true);
    statusLabel->setText("Total dust value: " + QString::number(sumDust));
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
