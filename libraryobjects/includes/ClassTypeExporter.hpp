/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeExporter
*/

#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QMap>

namespace types {
	class ClassType;
}

namespace libraryObjects {
	class ClassTypeExporter {
	public:
		ClassTypeExporter(const QJsonObject &json);
		~ClassTypeExporter();

		QString signature() const;

		bool hasBody() const { return _body.size() == 0; }
		QStringList body() const { return _body; }

	private:
		void build();
		types::ClassType *_classType;
		QStringList _body;
	};
}
