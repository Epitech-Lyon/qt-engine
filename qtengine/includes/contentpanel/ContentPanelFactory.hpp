/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFactory
*/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtWidgets/QWidget>
#include <functional>

namespace qtengine {
	class ContentPanelBase;

	class ContentPanelFactory : public QObject {
		Q_OBJECT

	public:
		~ContentPanelFactory() = default;
		static ContentPanelFactory *instance();

		QStringList availablesNames() const { return _availablesNames; }
		ContentPanelBase *create(const QString &, QWidget * = nullptr);

	signals:
		void availablesNamesChanged(const QStringList &);

	private:
		ContentPanelFactory();
		template <typename ContentPanelType> void registerPanel()
		{
			QString name = ContentPanelType().name();

			_constructors[name] = [](QWidget *parent) { return new ContentPanelType(parent); };
			_availablesNames << name;
		}

		QMap<QString, std::function<ContentPanelBase *(QWidget *)>> _constructors;
		QStringList _availablesNames;
	};
};
