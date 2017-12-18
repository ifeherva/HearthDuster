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
#include <QFileDialog>
#include "../db/cardsdb.h"
#include <security.h>
#include "../preferences/preferences.h"
#include "../strategies/strategies.h"

#define STATUS_MESSAGE_HEARTHSTONE_NOT_RUNNING "Waiting on Hearthstone..."
#define STATUS_MESSAGE_SYNCHING_ERROR "Error while synching collection"
#define STATUS_MESSAGE_SYNCHING_COMPLETE "Collection is up to date"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // build UI
    ui->setupUi(this);

    connect(ui->actionExport, SIGNAL(triggered(bool)), this, SLOT(on_exportCollectionClicked()));

    // init database from locale
    QString locale = Preferences::getLocale();
    QString carddbpath = QFileInfo(QCoreApplication::applicationFilePath()).absoluteDir().absolutePath() + "/Cards/cardsDB."+locale +".json";

    if (CardsDb::initFromFile(carddbpath)) {
        QMessageBox::critical(this, tr("HearthDuster"), tr("Failed to load cards database file! Program will now exit."));
        QCoreApplication::exit();
    }

    statusLabel = new QLabel("");
    ui->statusBar->addPermanentWidget(statusLabel);

    // build menu
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    QAction *aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutBox()));
    menuBar()->addMenu(helpMenu);

    // build strategies
    INSTALL_STRATEGIES(strategies)

    for (unsigned int i = 0; i < strategies.size(); i++) {
        ui->strategiesComboBox->addItem(strategies[i]->name());
        ui->strategiesComboBox->setItemData(i, strategies[i]->description(), Qt::ToolTipRole);
    }
    connect(ui->strategiesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCardTable(int)) );

    ui->resultsTableWidget->clear();
    ui->resultsTableWidget->setColumnCount(4);
    ui->resultsTableWidget->setRowCount(0);
    ui->resultsTableWidget->setHorizontalHeaderLabels(QStringList() << "Card" << "Format" << "Normal" << "Golden" << "Dust");
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->setSortingEnabled(true);

    ui->statusLabel->setText(tr(STATUS_MESSAGE_HEARTHSTONE_NOT_RUNNING));


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

    synchWorkerThread = new SynchWorkerThread(this->collection);
    connect(synchWorkerThread, SIGNAL(synchCompleted(int)), this, SLOT(on_syncCompleted(int)));
    synchWorkerThread->start();
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

void MainWindow::on_syncCompleted(int error)
{
    switch (error) {
        case SynchError::HearthstoneNotRunning :
            ui->statusLabel->setText(tr(STATUS_MESSAGE_HEARTHSTONE_NOT_RUNNING));
            ui->statusLabel->setStyleSheet("QLabel { color : black; }");
            return;
        case SynchError::InvalidCollectionData :
        case SynchError::UnknownError :
            ui->statusLabel->setText(tr(STATUS_MESSAGE_SYNCHING_ERROR));
            ui->statusLabel->setStyleSheet("QLabel { color : red; }");
            return;
        case SynchError::NoError :
            ui->statusLabel->setText(tr(STATUS_MESSAGE_SYNCHING_COMPLETE));
            ui->statusLabel->setStyleSheet("QLabel { color : green; }");
            break;
    }

    updateCardTable(strategies[ui->strategiesComboBox->currentIndex()]);
}

void MainWindow::updateCardTable(int strategyIdx)
{
    updateCardTable(strategies[strategyIdx]);
}

void MainWindow::showAboutBox()
{
    if (this->aboutBox == NULL) {
        this->aboutBox = new AboutDialog(APP_VERSION, this);
    }
    aboutBox->exec();
}

QColor getRarityColor(const CardRarity& rarity)
{
    switch (rarity) {
        case RARITY_FREE:
        case RARITY_COMMON:
            return QColor(233, 239, 235);
        case RARITY_RARE:
            return QColor(27, 95, 178);
        case RARITY_EPIC:
            return QColor(191, 45, 204);
        case RARITY_LEGENDARY:
            return QColor(255, 147, 0);
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

    auto extraColumns = strategy->extraParams();

    ui->resultsTableWidget->clear();
    ui->resultsTableWidget->setSortingEnabled(false);
    ui->resultsTableWidget->setColumnCount(5 + extraColumns.size());
    ui->resultsTableWidget->setRowCount(cardsList.size());
    auto headerLabels = QStringList() << "Card" << "Format" << "Normal" << "Golden" << "Dust";
    for (auto extraColumn : extraColumns) {
        headerLabels << extraColumn;
    }
    ui->resultsTableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    for (unsigned int i = 0; i < extraColumns.size(); i++) {
        ui->resultsTableWidget->horizontalHeader()->setSectionResizeMode(5+i, QHeaderView::ResizeToContents);
    }

    auto bgColor = QColor(165,143,121);
    auto palette = ui->resultsTableWidget->palette();
    palette.setColor(QPalette::Base, bgColor);
    ui->resultsTableWidget->setPalette(palette);

    auto wildBrush = QBrush(QColor(66, 73, 19));
    wildBrush.setStyle(Qt::BDiagPattern);

    unsigned int sumDust = 0;
    for (unsigned int row = 0; row < cardsList.size(); row++) {
        auto card = cardsList[row];
        auto rarityBrush = QBrush(getRarityColor(card.cardData->rarity));

        auto cardTableWidgetItem = new QTableWidgetItem();
        cardTableWidgetItem->setData(Qt::EditRole, card.cardData->name);
        cardTableWidgetItem->setData(Qt::ToolTipRole, card.cardData->shortDescription);
        cardTableWidgetItem->setForeground(rarityBrush);
        bool isStandard = strategy->isStandard(card.cardData);
        if (!isStandard) {
            cardTableWidgetItem->setBackground(wildBrush);
        } else {
            cardTableWidgetItem->setBackgroundColor(bgColor);
        }
        ui->resultsTableWidget->setItem(row, 0, cardTableWidgetItem);

        auto formatTableWidgetItem = new QTableWidgetItem();
        formatTableWidgetItem->setData(Qt::EditRole, isStandard ? "Standard" : "Wild");
        formatTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        formatTableWidgetItem->setBackgroundColor(bgColor);
        ui->resultsTableWidget->setItem(row, 1, formatTableWidgetItem);

        auto normalCountTableWidgetItem = new QTableWidgetItem();
        normalCountTableWidgetItem->setData(Qt::EditRole, card.superfluous_normal);
        normalCountTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        normalCountTableWidgetItem->setBackgroundColor(bgColor);
        ui->resultsTableWidget->setItem(row, 2, normalCountTableWidgetItem);

        auto premiumCountTableWidgetItem = new QTableWidgetItem();
        premiumCountTableWidgetItem->setData(Qt::EditRole, card.superfluous_premium);
        premiumCountTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        premiumCountTableWidgetItem->setBackgroundColor(bgColor);
        ui->resultsTableWidget->setItem(row, 3, premiumCountTableWidgetItem);

        unsigned int dustValue = card.dustValue();
        auto dustValueTableWidgetItem = new QTableWidgetItem();
        dustValueTableWidgetItem->setData(Qt::EditRole, dustValue);
        dustValueTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dustValueTableWidgetItem->setBackgroundColor(bgColor);
        ui->resultsTableWidget->setItem(row, 4, dustValueTableWidgetItem);
        sumDust += dustValue;

        for (unsigned int i = 0; i < extraColumns.size(); i++) {
            auto userValue = card.userData[i];
            auto extraTableWidgetItem = new QTableWidgetItem();
            extraTableWidgetItem->setData(Qt::EditRole, userValue);
            extraTableWidgetItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            extraTableWidgetItem->setBackgroundColor(bgColor);
            ui->resultsTableWidget->setItem(row, 5+i, extraTableWidgetItem);
        }
    }
    ui->resultsTableWidget->setSortingEnabled(true);
    statusLabel->setText("Total dust value: " + QString::number(sumDust));
}

void MainWindow::on_strategyError(const DustStrategy* strategy, QString error)
{
    QMessageBox::warning(this, strategy->name(), error);
}

void MainWindow::on_strategyMessage(const DustStrategy* strategy, QString message)
{
    ui->statusBar->showMessage(strategy->name() + ": " + message, 3000);
}

void MainWindow::on_exportCollectionClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Collection"),
                               "",
                               tr("CSV (*.csv)"));
    if (fileName != "") {
        if (this->collection != NULL) {
            bool success = this->collection->exportToFile(fileName, ExportFormat::CSV);
            if (success) {
                QMessageBox::information(this, "Export collection", "Successfully exported collection to " + fileName);
            } else {
                QMessageBox::critical(this, "Export collection", "Failed to export collection");
            }
        }
    }
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

void SynchWorkerThread::run() {
    while(1) {
        auto error = this->collection->sync();
        if (error == SynchError::NoError) {
            synchUpdateInterval = SYNCH_UPDATE_INTERVAL_SUCCESS;
            emit synchCompleted(error);
        } else {
            synchUpdateInterval = SYNCH_UPDATE_INTERVAL_ERROR;
        }

        sleep(synchUpdateInterval);
    }
}
