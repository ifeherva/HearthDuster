//
//  card.cpp
//  card
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "card.h"
#include <map>

Card::Card(const QString& id) : id(id) {}

QString ReadStringValue(const QJsonValue& value, QString defaultValue = "") {
    if (value ==  QJsonValue::Undefined || !value.isString()) return defaultValue;
    return value.toString();
}

int ReadIntValue(const QJsonValue& value, int defaultValue = 0) {
    if (value ==  QJsonValue::Undefined || !value.isDouble()) return defaultValue;
    return value.toDouble();
}

bool ReadBoolValue(const QJsonValue& value, bool defaultValue = 0) {
    if (value ==  QJsonValue::Undefined || !value.isBool()) return defaultValue;
    return value.toBool();
}

std::map<QString, CardClass> cardClassMap = {{"NEUTRAL", CLASS_NEUTRAL},
                                            {"WARRIOR", CLASS_WARRIOR},
                                            {"DRUID", CLASS_DRUID},
                                            {"PRIEST", CLASS_PRIEST},
                                            {"HUNTER", CLASS_HUNTER},
                                            {"MAGE", CLASS_MAGE},
                                            {"WARLOCK", CLASS_WARLOCK},
                                            {"PALADIN", CLASS_PALADIN},
                                            {"SHAMAN", CLASS_SHAMAN},
                                            {"ROGUE", CLASS_ROGUE},
                                            {"DREAM", CLASS_DREAM},
                                            {"DEATHKNIGHT", CLASS_DEATHKNIGHT}};


CardClass ReadCardClassValue(const QJsonValue& value, CardClass defaultValue = CLASS_NEUTRAL) {
    QString sValue = ReadStringValue(value, "NEUTRAL");
    if (!cardClassMap.count(sValue)) {
        return defaultValue;
    }
    return cardClassMap[sValue];
}

std::map<QString, CardRarity> cardRarityMap = {{"FREE", RARITY_FREE},
                                            {"COMMON", RARITY_COMMON},
                                            {"RARE", RARITY_RARE},
                                            {"EPIC", RARITY_EPIC},
                                            {"LEGENDARY", RARITY_LEGENDARY}};

CardRarity ReadCardRarityValue(const QJsonValue& value, CardRarity defaultValue = RARITY_FREE) {
    QString sValue = ReadStringValue(value, "FREE");
    if (!cardRarityMap.count(sValue)) {
        return defaultValue;
    }
    return cardRarityMap[sValue];
}

std::map<QString, CardType> cardTypeMap = {{"INVALID", TYPE_INVALID},
                                           {"MINION", TYPE_MINION},
                                           {"SPELL", TYPE_SPELL},
                                           {"WEAPON", TYPE_WEAPON},
                                           {"ENCHANTMENT", TYPE_ENCHANTMENT},
                                           {"HERO", TYPE_HERO},
                                           {"HERO_POWER", TYPE_HERO_POWER}};

CardType ReadCardTypeValue(const QJsonValue& value, CardType defaultValue = TYPE_INVALID) {
    QString sValue = ReadStringValue(value, "INVALID");
    if (!cardTypeMap.count(sValue)) {
        return defaultValue;
    }
    return cardTypeMap[sValue];
}

Card* CardFactory::FromJson(const QJsonObject& jsonobject) {

    auto cardId = ReadStringValue(jsonobject["id"], "");
    if (cardId == "") {
        return NULL;
    }

    Card* card = new Card(cardId);

    card->name = ReadStringValue(jsonobject["name"], "");
    card->artist = ReadStringValue(jsonobject["artist"], "");
    card->flavor = ReadStringValue(jsonobject["flavor"], "");
    card->set = ReadStringValue(jsonobject["set"], "");
    card->text = ReadStringValue(jsonobject["text"], "");
    card->attack = ReadIntValue(jsonobject["attack"], 0);
    card->health = ReadIntValue(jsonobject["health"], 0);
    card->cost = ReadIntValue(jsonobject["cost"], 0);
    card->dbfId = ReadIntValue(jsonobject["dbfId"], 0);
    card->cardClass = ReadCardClassValue(jsonobject["cardClass"]);
    card->playerClass = ReadCardClassValue(jsonobject["playerClass"]);
    card->collectible = ReadBoolValue(jsonobject["collectible"], false);
    card->elite = ReadBoolValue(jsonobject["elite"], false);

    card->rarity = ReadCardRarityValue(jsonobject["rarity"], RARITY_FREE);
    card->type = ReadCardTypeValue(jsonobject["type"]);

    switch (card->type) {
    case TYPE_MINION:
    case TYPE_WEAPON:
        card->shortDescription = QString::number(card->cost) + " mana " + QString::number(card->attack)+"/"+QString::number(card->health)+"\n"+card->text;
        break;
    case TYPE_SPELL:
        card->shortDescription = QString::number(card->cost) + " mana\n" + card->text;
        break;
    default:
        card->shortDescription = "";
        break;
    }

    return card;
}
