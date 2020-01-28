/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewManager
*/

#include "moc_ViewManager.cpp"
#include "ViewManager.hpp"

QJsonObject qtengine::ViewManager::serialize() const
{
	QJsonObject json;
	json["Current view"] = _currentView;
	return json;
}

void qtengine::ViewManager::deserialize(const QJsonObject &json)
{
	openView(json["Current view"].toString());
}

void qtengine::ViewManager::openView(const QString &currentView)
{
	_currentView = currentView;
	emit currentViewChanged(_currentView);
}
