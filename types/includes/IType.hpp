/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** IType
*/

#pragma once

#include <QtCore/QObject>
#include "ISerializable.hpp"

namespace types {
    class IType : public QObject, public ISerializable {
    public:
        virtual ~IType() = default;

    protected:
        IType() = default;

        template<typename T> inline void setValue(T &value, const T &newValue, std::function<void ()> emitFunction)
        {
            if (value != newValue) {
                value = newValue;
                emitFunction();
            }
        }
    };
}
