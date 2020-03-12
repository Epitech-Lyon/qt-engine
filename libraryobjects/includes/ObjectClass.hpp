/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ObjectClass
*/

#pragma once

#include <QtCore/QList>
#include "ISerializable.hpp"
#include "types/includes/Constructor.hpp"
#include "types/includes/Method.hpp"
#include "types/includes/Property.hpp"

namespace libraryObjects {
	class ObjectClass : public types::ISerializable {
	public:
		ObjectClass() = default;
		~ObjectClass() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		QList<types::Constructor*> getContructors() const { return _constructors; }
		types::Constructor *addConstructor(types::Constructor *constructor) { return addType(_constructors, constructor); }
		void removeConstructor(types::Constructor *constructor) { _constructors.removeAll(constructor); }

		QList<types::Method*> getMethods() const { return _methods; }
		types::Method *addMethod(types::Method *method) { return addType(_methods, method); }
		void removeMethod(types::Method *method) { _methods.removeAll(method); }

		QList<types::Method*> getSignals() const { return _signals; }
		types::Method *addSignal(types::Method *signal) { return addType(_signals, signal); }
		void removeSignal(types::Method *signal) { _signals.removeAll(signal); }

		QList<types::Method*> getSlots() const { return _slots; }
		types::Method *addSlot(types::Method *slot) { return addType(_slots, slot); }
		void removeSlot(types::Method *slot) { _slots.removeAll(slot); }

		QList<types::Property*> getProperties() const { return _properties; }
		types::Property *addProperty(types::Property *property) { return addType(_properties, property); }
		void removeProperty(types::Property *property) { _properties.removeAll(property); }

	private:
		template <typename T> T findExisting(const QList<T> &types, T type)
		{
			for (const auto &tmpType : types)
				if (*tmpType == *type)
					return tmpType;
			return nullptr;
		}

		template <typename T> T addType(QList<T> &types, T type)
		{
			if (!type->isValid()) { return nullptr; }

			auto existingType = findExisting(types, type);
			if (existingType) { return existingType; }

			types.append(type);
			return type;
		}

		template <typename T> QJsonArray serializeTypes(const QList<T> &types) const
		{
			QJsonArray jsonArray;

			for (auto type : types)
				jsonArray.append(type->serialize());
			return jsonArray;
		}
		template <typename T> QList<T*> deserializeTypes(const QJsonArray &jsonArray) const
		{
			QList<T*> types;

			for (auto jsonRef : jsonArray) {
				auto type = new T;

				type->deserialize(jsonRef.toObject());
				if (type->isValid())
					types.append(type);
				else
					delete type;
			}
			return types;
		}

		QList<types::Constructor*> _constructors;
		QList<types::Method*> _methods;
		QList<types::Method*> _signals;
		QList<types::Method*> _slots;
		QList<types::Property*> _properties;
	};
}
