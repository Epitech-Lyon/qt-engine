/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** QVariantConverter
*/

#include "QVariantConverter.hpp"

#include <QtCore/QJsonObject>
#include <QtCore/QBuffer>
#include <QtCore/QDebug>
#include <QtGui/QPalette>
#include <QtGui/QFont>

QJsonValue libraryObjects::QVariantConverter::serialize(const QVariant &variant)
{
	QBuffer buffer;
	buffer.open(QIODevice::WriteOnly);

	QDataStream out(&buffer);
	out << variant;
	return QJsonValue(QString::fromLatin1(buffer.data().toBase64()));
}

QString libraryObjects::QVariantConverter::toString(const QVariant &value)
{
	auto type = value.type();

	switch (type) {
		case QVariant::String:
			return "\"" + value.toString() + "\"";
		case QVariant::Palette:
			return QString("QPalette()"); // TODO FIXME HACK
		case QVariant::Locale:
			{
				auto locale = value.toLocale();

				return QString("QLocale(QLocale::") + locale.languageToString(locale.language()) +
					", QLocale::" + locale.scriptToString(locale.script()) + "Script, QLocale::" +
					locale.countryToString(locale.country()).simplified().remove(' ') + ")";
			}
		case QVariant::Rect:
			{
				auto rect = value.toRect();

				return QString("QRect(") + QString::number(rect.x()) + ", " + QString::number(rect.y()) +
					", " + QString::number(rect.width()) + ", " + QString::number(rect.height()) + ")";
			}
		case QVariant::Font:
		{
			auto font = value.value<QFont>();

			return QString("QFont(\"") + font.family() + "\", " + QString::number(font.pointSize()) + ", " +
				QString::number(font.weight()) + ", " + QString::number(font.italic()) + ")";
		}
		case QVariant::Size:
		case QVariant::SizeF:
		case QVariant::Cursor:
		case QVariant::SizePolicy:
		case QVariant::KeySequence:
		{
			QString out;

			QDebug(&out).nospace().quote() << value;
			out = out.section(',', 1);
			out.remove(0, 1);
			out = out.left(out.lastIndexOf(QChar(')')));
			if (type == QVariant::SizePolicy) {
				auto start = out.indexOf(QChar('('));
				auto end = out.indexOf(QChar('=')) - start;

				out.remove(start + 1, end + 1);
				start = out.indexOf(QChar(','));
				end = out.indexOf(QChar('=')) - start;
				out.remove(start + 1, end);
			}
			return out;
		}
		default:
			if (type != QVariant::Bool && type != QVariant::Int && QVariant::Double != type && type != QVariant::String)
				qDebug() << "UNHANDLED QVARIANT ! :" << value;
			return value.toString();
	}
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
