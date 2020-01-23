/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelFactory
*/

#include "ContentPanelFactory.hpp"

qtengine::ContentPanelFactory::ContentPanelFactory()
{
}

QStringList qtengine::ContentPanelFactory::getNames()
{
	return ContentPanelFactory()._constructors.keys();
}

qtengine::ContentPanelBase *qtengine::ContentPanelFactory::create(const QString &contentPanelName)
{
	ContentPanelFactory contentPanelFactory;

	return contentPanelFactory._constructors.contains(contentPanelName) ? contentPanelFactory._constructors[contentPanelName]() : nullptr;
}
