/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFactory
*/

#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtWidgets/QWidget>
#include <functional>

namespace qtengine {
	class ContentPanelBase;

	class ContentPanelFactory {
	public:
		static QStringList getNames();
		static ContentPanelBase *create(const QString &, QWidget * = nullptr);

	private:
		~ContentPanelFactory() = default;
		ContentPanelFactory();
		QMap<QString, std::function<ContentPanelBase *(QWidget *)>> _constructors;
		static ContentPanelFactory *instance();
	};
};
