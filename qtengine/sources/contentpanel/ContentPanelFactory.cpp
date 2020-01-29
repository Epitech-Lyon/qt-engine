/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFactory
*/

#include "moc_ContentPanelFactory.cpp"
#include "ContentPanelFactory.hpp"

#include <QtCore/QDebug>

#include "ContentPanelEmpty.hpp"
#include "ContentPanelProjectExplorer.hpp"
#include "ContentPanelViewsExplorer.hpp"
#include "ContentPanelViewProperty.hpp"

qtengine::ContentPanelFactory::ContentPanelFactory()
{
	registerPanel<ContentPanelEmpty>();
	registerPanel<ContentPanelProjectExplorer>();
	registerPanel<ContentPanelViewsExplorer>();
	registerPanel<ContentPanelViewProperty>();
}

qtengine::ContentPanelFactory *qtengine::ContentPanelFactory::instance()
{
	static ContentPanelFactory contentPanelFactory;

	return &contentPanelFactory;
}

qtengine::ContentPanelBase *qtengine::ContentPanelFactory::create(const QString &contentPanelName, QWidget *parent)
{
	ContentPanelBase *contentPanel = nullptr;

	if (_availablesNames.contains(contentPanelName)) {
		contentPanel = _constructors[contentPanelName](parent);
		contentPanel->init();
		if (!contentPanel->canHaveMultipleInstance()) {
			_availablesNames.removeAll(contentPanelName);
			emit availablesNamesChanged(_availablesNames);
			connect(contentPanel, &QObject::destroyed, [this, contentPanelName](QObject *) {
				_availablesNames << contentPanelName;
				emit availablesNamesChanged(_availablesNames);
			});
		}
	} else
		qCritical() << "ContentPanelFactoryError:" << contentPanelName << "is not available";
	return contentPanel;
}
