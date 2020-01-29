/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** QVariantConverter
*/

#include "QVariantConverter.hpp"

QJsonValue qtengine::QVariantConverter::serialize(const QVariant &variant)
{
	switch (variant.type()) {
	case QVariant::Size:
		return QVariantConverter().serializeSize(variant);
	default:
		return QJsonValue::fromVariant(variant);
	}
}

QVariant qtengine::QVariantConverter::deserialize(QVariant::Type type, const QJsonValue &json)
{
	switch (type) {
	case QVariant::Size:
		return QVariantConverter().deserializeSize(json);
	default:
		return json.toVariant();
	}
}

QJsonValue qtengine::QVariantConverter::serializeSize(const QVariant &) const
{
	return QJsonValue("Yes !");
}

QVariant qtengine::QVariantConverter::deserializeSize(const QJsonValue &) const
{
	return QVariant("Yes !");
}
