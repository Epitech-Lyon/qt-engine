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
		void addConstructor(types::Constructor *constructor) { if (constructor->isValid() && !contains(_constructors, constructor)) _constructors.append(constructor); }
		void removeConstructor(types::Constructor *constructor) { _constructors.removeAll(constructor); }

		QList<types::Method*> getMethods() const { return _methods; }
		void addMethod(types::Method *method) { if (method->isValid() && !contains(_methods, method)) _methods.append(method); }
		void removeMethod(types::Method *method) { _methods.removeAll(method); }

		QList<types::Method*> getSignals() const { return _signals; }
		void addSignal(types::Method *signal) { if (signal->isValid() && !contains(_signals, signal)) _signals.append(signal); }
		void removeSignal(types::Method *signal) { _signals.removeAll(signal); }

		QList<types::Method*> getSlots() const { return _slots; }
		void addSlot(types::Method *slot) { if (slot->isValid() && !contains(_slots, slot)) _slots.append(slot); }
		void removeSlot(types::Method *slot) { _slots.removeAll(slot); }

		QList<types::Property*> getProperties() const { return _properties; }
		void addProperty(types::Property *property) { if (property->isValid() && !contains(_properties, property)) _properties.append(property); }
		void removeProperty(types::Property *property) { _properties.removeAll(property); }

		template <typename T> static bool contains(const QList<T> &types, T type)
		{
			for (const auto &tmpType : types)
				if (*tmpType == *type)
					return true;
			return false;
		}

	private:
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
