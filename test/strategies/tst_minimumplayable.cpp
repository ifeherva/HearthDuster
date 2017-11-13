#include <QString>
#include <QtTest>
#include "../../app/src/strategies/minimumplayable.h"
#include "../../app/src/db/cardsdb.h"

class MinimumPlayableDustStrategyTest : public QObject
{
    Q_OBJECT

public:
    MinimumPlayableDustStrategyTest();

private Q_SLOTS:
    void initTestCase();
    void getDustValue();
};

MinimumPlayableDustStrategyTest::MinimumPlayableDustStrategyTest() {}

void MinimumPlayableDustStrategyTest::initTestCase()
{
    QString carddbpath = "../app/HearthDuster.app/Contents/MacOS/Cards/cardsDB.enUS.json";

    // init database from locale
    QCOMPARE(CardsDb::initFromFile(carddbpath), 0);
}

void MinimumPlayableDustStrategyTest::getDustValue()
{
    MinimumPlayableDustStrategy strategy = MinimumPlayableDustStrategy();

    CollectionCard card = CollectionCard("AT_001", 0, 0);

    // 1.
    QCOMPARE(card.normalCount, 0u);
    QCOMPARE(card.premiumCount, 0u);

    auto dv = strategy.getDustValue(card);
    QCOMPARE(dv.normal, 0u);
    QCOMPARE(dv.premium, 0u);

    // 2.
    card.normalCount = 3;
    card.premiumCount = 0;

    dv = strategy.getDustValue(card);
    QCOMPARE(dv.normal, 1u);
    QCOMPARE(dv.premium, 0u);

    // 3.
    card.normalCount = 3;
    card.premiumCount = 4;

    dv = strategy.getDustValue(card);
    QCOMPARE(dv.normal, 3u);
    QCOMPARE(dv.premium, 2u);
}

QTEST_APPLESS_MAIN(MinimumPlayableDustStrategyTest)

#include "tst_minimumplayable.moc"
