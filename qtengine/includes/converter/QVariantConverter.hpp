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

		QJsonValue serializeSizeF(const QVariant &) const;
		QVariant deserializeSizeF(const QJsonValue &) const;

		QJsonValue serializeRect(const QVariant &) const;
		QVariant deserializeRect(const QJsonValue &) const;

		QJsonValue serializeRectF(const QVariant &) const;
		QVariant deserializeRectF(const QJsonValue &) const;

		QJsonValue serializeSizePolicy(const QVariant &) const;
		QVariant deserializeSizePolicy(const QJsonValue &) const;

		QJsonValue serializeKeySequence(const QVariant &) const;
		QVariant deserializeKeySequence(const QJsonValue &) const;

		QJsonValue serializeLocale(const QVariant &) const;
		QVariant deserializeLocale(const QJsonValue &) const;
	};
}
