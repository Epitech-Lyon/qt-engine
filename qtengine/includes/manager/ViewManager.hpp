/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ViewManager
*/

#pragma once

#include "ISerializable.hpp"
#include <QtCore/QObject>
#include <QtCore/QString>

namespace libraryObjects {
	class AObject;	
}

namespace qtengine {
    class ViewManager : public QObject, public ISerializable {
		Q_OBJECT

	// General
    public:
        ViewManager();
        ~ViewManager() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		void openView(const QString &viewPath);

		QString viewExtension() const { return _viewExt; }

		QString viewPath() const { return _viewPath; }
		QString viewName() const { return _viewName; }
		libraryObjects::AObject *viewObject() const { return _viewObject; }

	public slots:
		void onCreateView(const QString &viewPath);
		void onSaveView();
		void onSaveViewAs();

	signals:
		void viewPathChanged(const QString &viewPath);
		void viewNameChanged(const QString &viewName);
		void viewObjectChanged(libraryObjects::AObject *viewObject);

    private:
		const QString _viewExt = ".view";
		QString _viewPath;
		QString _viewName;
		libraryObjects::AObject *_viewObject;

	// Current Object
	public:
		libraryObjects::AObject *currentObject() const { return _currentObject; }
		void setCurrentObject(libraryObjects::AObject *currentObject);

	signals:
		void currentObjectChanged(libraryObjects::AObject *currentObject);

	private:
		libraryObjects::AObject *_currentObject;
    };
}
