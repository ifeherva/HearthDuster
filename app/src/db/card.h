//
//  card.h
//  card
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef CARD_H
#define CARD_H

#include <QJsonObject>
#include <QVector>

#define CARD_SET_BASIC "CORE"
#define CARD_SET_CLASSIC "EXPERT1"
#define CARD_SET_THE_GRAND_TOURNAMENT "TGT"
#define CARD_SET_OLD_GODS "OG"
#define CARD_SET_KARAZHAN "KARA"
#define CARD_SET_MEAN_STREETS_OF_GADGETZAN "GANGS"
#define CARD_SET_UNGORO "UNGORO"
#define CARD_SET_KNIGHTS_OF_THE_FROZEN_THRONE "ICECROWN"
#define CARD_SET_KOBOLDS_AND_CARACOMBS "LOOTAPALOOZA"

enum CardClass {
    CLASS_NEUTRAL = 0,
    CLASS_WARRIOR,
    CLASS_DRUID,
    CLASS_PRIEST,
    CLASS_HUNTER,
    CLASS_MAGE,
    CLASS_WARLOCK,
    CLASS_PALADIN,
    CLASS_SHAMAN,
    CLASS_ROGUE,
    CLASS_DREAM,
    CLASS_DEATHKNIGHT
};

enum CardRarity {
    RARITY_FREE = 0,
    RARITY_COMMON,
    RARITY_RARE,
    RARITY_EPIC,
    RARITY_LEGENDARY
};

enum CardType {
    TYPE_INVALID,
    TYPE_MINION,
    TYPE_SPELL,
    TYPE_WEAPON,
    TYPE_ENCHANTMENT,
    TYPE_HERO,
    TYPE_HERO_POWER
};

enum CardRace {
    RACE_INVALID = 0,
    RACE_MURLOC,
    RACE_BEAST,
    RACE_ELEMENTAL,
    RACE_MECHANICAL,
    RACE_DEMON,
    RACE_BLOODELF,
    RACE_DRAENEI,
    RACE_DWARF,
    RACE_GNOME,
    RACE_GOBLIN,
    RACE_HUMAN,
    RACE_NIGHTELF,
    RACE_ORC,
    RACE_TAUREN,
    RACE_TROLL,
    RACE_UNDEAD,
    RACE_WORGEN,
    RACE_GOBLIN2,
    RACE_SCOURGE,
    RACE_OGRE,
    RACE_TOTEM,
    RACE_NERUBIAN,
    RACE_PIRATE,
    RACE_DRAGON
};

class Card
{

public:

    Card(const QString& id);

    // properties
    QString id;
    QString artist;
    int attack;
    CardClass cardClass;
    bool collectible;
    bool elite;
    int cost;
    int dbfId;
    QString flavor;
    int health;
    QString name;
    CardClass playerClass;
    CardRarity rarity;
    QVector<QString> referencedTags;
    QString set;
    QString text;
    CardType type;
    CardRace race;
};

class CardFactory {
public:
    static Card* FromJson(const QJsonObject& jsonobject);
};

struct CollectionCard {
    QString id;
    unsigned int normalCount;
    unsigned int premiumCount;

public:
    CollectionCard() {}
    CollectionCard(const QString& id): id(id), normalCount(0), premiumCount(0) {}
    CollectionCard(const QString& id, unsigned int normalCount, unsigned int premiumCount): id(id), normalCount(normalCount), premiumCount(premiumCount) {}
};

#endif // CARD_H
