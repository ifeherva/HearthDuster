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
    RARITY_FREE,
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

#endif // CARD_H
