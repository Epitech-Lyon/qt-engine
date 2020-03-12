/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Utils
*/

#pragma once

#include <functional>
#include <QtCore/QMetaMethod>
#include <QtCore/QString>

namespace types {
    inline QString typeToString(QMetaMethod::MethodType type)
    {
        QMap<QMetaMethod::MethodType, QString> typeVal = {
            { QMetaMethod::Method, "Method" },
            { QMetaMethod::Signal, "Signal" },
            { QMetaMethod::Slot, "Slot" },
            { QMetaMethod::Constructor, "Constructor" },
        };

        return typeVal[type];
    }

    inline QString accessToString(QMetaMethod::Access access)
    {
        QMap<QMetaMethod::Access, QString> accessVal = {
            {QMetaMethod::Private, "Private"},
            {QMetaMethod::Protected, "Protected"},
            {QMetaMethod::Public, "Public"}
        };

        return accessVal[access];
    }
}
