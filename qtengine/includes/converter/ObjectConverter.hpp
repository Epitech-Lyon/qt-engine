/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectConverter
*/

#pragma once

#include "ISerializable.hpp"

#include <QtCore/QObject>
#include <QtCore/QMap>

namespace qtengine {
	class ObjectConverter : public ISerializable {
	public:
		ObjectConverter(QObject *);
		~ObjectConverter() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

        const char *className() const { return _className; }

		QList<const char *> propertiesName() const { return _properties.keys(); }
		QList<const char *> propertiesTypeName() const { return _properties.values(); }
		QMap<const char *, const char *> properties() const { return _properties; }

	private:
		void initProperties(const QMetaObject *);
		void verifyProperties(QJsonObject &) const;
		QObject *_object;
        const char *_className;
		QMap<const char *, const char *> _properties;
	};
}
