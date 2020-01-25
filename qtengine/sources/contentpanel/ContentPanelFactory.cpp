/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFactory
*/

#include "ContentPanelFactory.hpp"

#include <QtCore/QDebug>

#include "ContentPanelEmpty.hpp"
#include "ContentPanelFileExplorer.hpp"

qtengine::ContentPanelFactory::ContentPanelFactory()
{
	_constructors[ContentPanelEmpty().name()] = [](QWidget *parent) { return new ContentPanelEmpty(parent); };
	_constructors[ContentPanelFileExplorer().name()] = [](QWidget *parent) { return new ContentPanelFileExplorer(parent); };
}

qtengine::ContentPanelFactory *qtengine::ContentPanelFactory::instance()
{
	static ContentPanelFactory contentPanelFactory;

	return &contentPanelFactory;
}

QStringList qtengine::ContentPanelFactory::getNames()
{
	return instance()->_constructors.keys();
}

qtengine::ContentPanelBase *qtengine::ContentPanelFactory::create(const QString &contentPanelName, QWidget *parent)
{
	try {
		ContentPanelBase *contentPanel = instance()->_constructors[contentPanelName](parent);

		contentPanel->init();
		return contentPanel;
	} catch(const std::exception &e) {
		qCritical() << e.what() << "when try to create" << contentPanelName;
		return nullptr;
	}
}
