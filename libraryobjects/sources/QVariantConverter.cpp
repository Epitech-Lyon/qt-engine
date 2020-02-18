/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** QVariantConverter
*/

#include "QVariantConverter.hpp"

#include <QtCore/QJsonObject>
#include <QtCore/QBuffer>

#include <QtCore/QSize>
#include <QtCore/QSizeF>
#include <QtCore/QRect>
#include <QtWidgets/QSizePolicy>
#include <QtGui/QKeySequence>
#include <QtCore/QLocale>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtGui/QPalette>

QJsonValue libraryObjects::QVariantConverter::serialize(const QVariant &variant)
{
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly);

	QDataStream out(&buffer);
	out << variant;
	return QJsonValue(QString::fromLatin1(buffer.data().toBase64()));
}

QVariant libraryObjects::QVariantConverter::deserialize(const QJsonValue &value)
{
	QByteArray byteArray = QByteArray::fromBase64(value.toString().toLatin1());
	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::ReadOnly);

	QDataStream in(&buffer);
	QVariant variant;
	in >> variant;
	return variant;
}
