//
//  duststrategy.h
//  Dusting strategy abstract class
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//
#ifndef DUSTSTRATEGY_H
#define DUSTSTRATEGY_H

#include "db/card.h"
#include <QObject>
#include <QString>
#include <QVariant>

struct DustPair {
    unsigned int normal = 0;
    unsigned int premium = 0;
    std::vector<QVariant> userData;

    bool isEmpty() {
        return normal == 0 && premium == 0;
    }
};

class DustStrategy : public QObject {

    Q_OBJECT

public:
    DustStrategy() {}
    virtual ~DustStrategy() {}

    virtual QString name() const = 0;
    virtual QString description() const = 0;

    virtual DustPair dustValue(const CollectionCard& card) const = 0;

    virtual const std::vector<QString>& extraParams() const {return m_extraParams;}

    bool isCardElite(const CollectionCard& card) const;
    bool isStandard(const Card* cardDef) const;
    bool isStandard(const CollectionCard& card) const;
protected:
    std::vector<QString> m_extraParams;
signals:
    void sendErrorMessage(const DustStrategy*, QString);
    void sendMessage(const DustStrategy*, QString);
};

#endif // DUSTSTRATEGY_H
