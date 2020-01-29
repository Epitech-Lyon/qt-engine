/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** QVariantConverter
*/

#pragma once

#include <QtCore/QJsonValue>
#include <QtCore/QVariant>

namespace qtengine {
	class QVariantConverter {
	public:
		static QJsonValue serialize(const QVariant &);
		static QVariant deserialize(QVariant::Type, const QJsonValue &);

	private:
		QJsonValue serializeSize(const QVariant &) const;
		QVariant deserializeSize(const QJsonValue &) const;
	};
}
