/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewManagement
*/

#pragma once

#include "ISerializable.hpp"

#include <QtCore/QObject>

namespace qtengine {
	class ViewManagement : public ISerializable {
	public:
		ViewManagement(QObject *);
		~ViewManagement() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

	private:
		QJsonObject serialize(QObject *) const;
		QObject *_object;
	};
}
