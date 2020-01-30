/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewConverter
*/

#pragma once

#include "ISerializable.hpp"

#include <QtCore/QObject>

namespace qtengine {
	class ViewConverter : public ISerializable {
	public:
		ViewConverter(QObject *);
		~ViewConverter() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

	private:
		QJsonObject serialize(QObject *) const;
		QObject *_object;
	};
}
