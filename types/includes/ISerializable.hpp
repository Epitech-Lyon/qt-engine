/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ISerializable
*/

#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

namespace types {
	class ISerializable {
	public:
		virtual ~ISerializable() = default;

		virtual QJsonObject serialize() const = 0;
		virtual void deserialize(const QJsonObject &json) = 0;

    protected:
        ISerializable() = default;
	};
}
