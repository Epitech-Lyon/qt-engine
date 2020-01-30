/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** QVariantConverter
*/

#include "QVariantConverter.hpp"

#include <QtCore/QJsonObject>
#include <QtCore/QSize>
#include <QtCore/QSizeF>
#include <QtCore/QRect>
#include <QtWidgets/QSizePolicy>
#include <QtGui/QKeySequence>
#include <QtCore/QLocale>

QJsonValue qtengine::QVariantConverter::serialize(const QVariant &variant)
{
	switch (variant.type()) {
	case QVariant::Size:
		return QVariantConverter().serializeSize(variant);
	case QVariant::SizeF:
		return QVariantConverter().serializeSizeF(variant);
	case QVariant::Rect:
		return QVariantConverter().serializeRect(variant);
	case QVariant::RectF:
		return QVariantConverter().serializeRectF(variant);
	case QVariant::SizePolicy:
		return QVariantConverter().serializeSizePolicy(variant);
	case QVariant::KeySequence:
		return QVariantConverter().serializeKeySequence(variant);
	case QVariant::Locale:
		return QVariantConverter().serializeLocale(variant);
	default:
		return QJsonValue::fromVariant(variant);
	}
}

QVariant qtengine::QVariantConverter::deserialize(QVariant::Type type, const QJsonValue &json)
{
	switch (type) {
	case QVariant::Size:
		return QVariantConverter().deserializeSize(json);
	case QVariant::SizeF:
		return QVariantConverter().deserializeSizeF(json);
	case QVariant::Rect:
		return QVariantConverter().deserializeRect(json);
	case QVariant::RectF:
		return QVariantConverter().deserializeRectF(json);
	case QVariant::SizePolicy:
		return QVariantConverter().deserializeSizePolicy(json);
	case QVariant::KeySequence:
		return QVariantConverter().deserializeKeySequence(json);
	case QVariant::Locale:
		return QVariantConverter().deserializeLocale(json);
	default:
		return json.toVariant();
	}
}

QJsonValue qtengine::QVariantConverter::serializeSize(const QVariant &variant) const
{
	QSize size = variant.toSize();
	QJsonObject json;

	json["width"] = size.width();
	json["height"] = size.height();
	return QJsonValue(json);
}

QVariant qtengine::QVariantConverter::deserializeSize(const QJsonValue &value) const
{
	QSize size;
	QJsonObject json = value.toObject();

	size.setWidth(json["width"].toInt());
	size.setHeight(json["height"].toInt());
	return QVariant(size);
}

QJsonValue qtengine::QVariantConverter::serializeSizeF(const QVariant &variant) const
{
	QSizeF sizeF = variant.toSizeF();
	QJsonObject json;

	json["width"] = sizeF.width();
	json["height"] = sizeF.height();
	return QJsonValue(json);
}

QVariant qtengine::QVariantConverter::deserializeSizeF(const QJsonValue &value) const
{
	QSizeF sizeF;
	QJsonObject json = value.toObject();

	sizeF.setWidth(json["width"].toDouble());
	sizeF.setHeight(json["height"].toDouble());
	return QVariant(sizeF);
}

QJsonValue qtengine::QVariantConverter::serializeRect(const QVariant &variant) const
{
	QRect rect = variant.toRect();
	QJsonObject json;

	json["x"] = rect.x();
	json["y"] = rect.y();
	json["width"] = rect.width();
	json["height"] = rect.height();
	return QJsonValue(json);
}

QVariant qtengine::QVariantConverter::deserializeRect(const QJsonValue &value) const
{
	QRect rect;
	QJsonObject json = value.toObject();

	rect.setX(json["x"].toInt());
	rect.setY(json["y"].toInt());
	rect.setWidth(json["width"].toInt());
	rect.setHeight(json["height"].toInt());
	return QVariant(rect);
}

QJsonValue qtengine::QVariantConverter::serializeRectF(const QVariant &variant) const
{
	QRectF rectF = variant.toRectF();
	QJsonObject json;

	json["x"] = rectF.x();
	json["y"] = rectF.y();
	json["width"] = rectF.width();
	json["height"] = rectF.height();
	return QJsonValue(json);
}

QVariant qtengine::QVariantConverter::deserializeRectF(const QJsonValue &value) const
{
	QRectF rectF;
	QJsonObject json = value.toObject();

	rectF.setX(json["x"].toInt());
	rectF.setY(json["y"].toInt());
	rectF.setWidth(json["width"].toInt());
	rectF.setHeight(json["height"].toInt());
	return QVariant(rectF);
}

QJsonValue qtengine::QVariantConverter::serializeSizePolicy(const QVariant &variant) const
{
	QSizePolicy sizePolicy = variant.value<QSizePolicy>();
	QJsonObject json;

	json["horizontalPolicy"] = static_cast<int>(sizePolicy.horizontalPolicy());
	json["verticalPolicy"] = static_cast<int>(sizePolicy.verticalPolicy());
	json["controlType"] = static_cast<int>(sizePolicy.controlType());
	return QJsonValue(json);
}

QVariant qtengine::QVariantConverter::deserializeSizePolicy(const QJsonValue &value) const
{
	QSizePolicy sizePolicy;
	QJsonObject json = value.toObject();

	sizePolicy.setHorizontalPolicy(static_cast<QSizePolicy::Policy>(json["horizontalPolicy"].toInt()));
	sizePolicy.setVerticalPolicy(static_cast<QSizePolicy::Policy>(json["verticalPolicy"].toInt()));
	sizePolicy.setControlType(static_cast<QSizePolicy::ControlType>(json["controlType"].toInt()));
	return QVariant(sizePolicy);
}

QJsonValue qtengine::QVariantConverter::serializeKeySequence(const QVariant &variant) const
{
	QKeySequence keySequence = variant.value<QKeySequence>();
	QJsonObject json;

	json["String"] = keySequence.toString();
	return QJsonValue(json);
}

QVariant qtengine::QVariantConverter::deserializeKeySequence(const QJsonValue &value) const
{
	QJsonObject json = value.toObject();

	return QVariant(QKeySequence(json["string"].toString()));
}

QJsonValue qtengine::QVariantConverter::serializeLocale(const QVariant &variant) const
{
	QLocale locale = variant.toLocale();
	QJsonObject json;

	json["name"] = locale.bcp47Name();
	return QJsonValue(json);
}

QVariant qtengine::QVariantConverter::deserializeLocale(const QJsonValue &value) const
{
	QJsonObject json = value.toObject();

	return QVariant(QLocale(json["string"].toString()));
}
