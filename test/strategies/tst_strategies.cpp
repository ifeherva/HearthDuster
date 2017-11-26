#include <QString>
#include <QtTest>
#include "../../app/src/strategies/jointduplicates.h"
#include "../../app/src/strategies/jointduplicatesgolden.h"
#include "../../app/src/db/cardsdb.h"

class DustStrategyTest : public QObject
{
    Q_OBJECT

public:
    DustStrategyTest();

private Q_SLOTS:
    void initTestCase();
    void testJointDuplicatesDustStrategy();
    void testJointDuplicatesGoldenDustStrategy();
};

DustStrategyTest::DustStrategyTest() {}

void DustStrategyTest::initTestCase()
{
    QString carddbpath = "../app/HearthDuster.app/Contents/MacOS/Cards/cardsDB.enUS.json";

    // init database from locale
    QCOMPARE(CardsDb::initFromFile(carddbpath), 0);
}

void DustStrategyTest::testJointDuplicatesDustStrategy()
{
    JointDuplicatesDustStrategy strategy;

    // rare card
    CollectionCard card = CollectionCard("AT_001", 0, 0);

    // 1.
    QCOMPARE(card.normalCount, 0u);
    QCOMPARE(card.premiumCount, 0u);

    auto dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 0u);
    QCOMPARE(dv.premium, 0u);

    // 2.
    card.normalCount = 3;
    card.premiumCount = 0;

    dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 1u);
    QCOMPARE(dv.premium, 0u);

    // 3.
    card.normalCount = 3;
    card.premiumCount = 4;

    dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 3u);
    QCOMPARE(dv.premium, 2u);

    // 4.
    card.normalCount = 0;
    card.premiumCount = 3;

    dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 0u);
    QCOMPARE(dv.premium, 1u);
}

void DustStrategyTest::testJointDuplicatesGoldenDustStrategy()
{
    JointDuplicatesGoldenDustStrategy strategy;

    // rare card
    CollectionCard card = CollectionCard("AT_001", 0, 0);

    // 1.
    QCOMPARE(card.normalCount, 0u);
    QCOMPARE(card.premiumCount, 0u);

    auto dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 0u);
    QCOMPARE(dv.premium, 0u);

    // 2.
    card.normalCount = 3;
    card.premiumCount = 0;

    dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 1u);
    QCOMPARE(dv.premium, 0u);

    // 3.
    card.normalCount = 3;
    card.premiumCount = 4;

    dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 1u);
    QCOMPARE(dv.premium, 4u);

    // 4.
    card.normalCount = 1;
    card.premiumCount = 3;

    dv = strategy.dustValue(card);
    QCOMPARE(dv.normal, 0u);
    QCOMPARE(dv.premium, 2u);
}

QTEST_APPLESS_MAIN(DustStrategyTest)

#include "tst_strategies.moc"
