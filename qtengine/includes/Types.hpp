/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Types
*/

#pragma once

#include <QtCore/QObject>

namespace qtengine {
    class Types : QObject {
        Q_OBJECT

    public:
        typedef enum {
            Public,
            Protected,
            Private
        } Access;
        Q_ENUM(Access)

        typedef enum {
            Function,
            Variable
        } Type;
        Q_ENUM(Type)

    private:
        Types() = default;
        ~Types() = default;
    };
}
