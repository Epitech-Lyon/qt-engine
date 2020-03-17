/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectClass
*/

#pragma once

#include <QtCore/QMap>
#include <QtCore/QList>
#include "ISerializable.hpp"

#include "ClassType.hpp"

namespace libraryObjects {
	class ObjectClass : public types::ISerializable {
	public:
		ObjectClass() = default;
		ObjectClass(QMetaObject *metaObject);
		~ObjectClass();

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		QList<types::ClassType*> getClassType(types::ClassType::Type type) const { return _classTypes[type]; }
		types::ClassType *addClassType(types::ClassType *classType);
		void removeClassType(types::ClassType *classType) { _classTypes[classType->type()].removeAll(classType); }

	private:
		QMap<types::ClassType::Type, QList<types::ClassType*>> _classTypes;
	};
}
