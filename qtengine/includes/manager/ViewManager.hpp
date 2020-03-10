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
	class ObjectClass;
}

namespace qtengine {
	class ViewManager : public QObject, public types::ISerializable {
		Q_OBJECT

	// General
	public:
		ViewManager();
		~ViewManager();

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &json) override;

		void closeView();
		void openView(const QString &viewPath);
		bool createView(libraryObjects::AObject *viewObject);

		QString viewExtension() const { return _viewExt; }

		QString viewPath() const { return _viewPath; }
		QString viewName() const { return _viewName; }
		libraryObjects::AObject *viewObject() const { return _viewObject; }
		libraryObjects::ObjectClass *viewObjectClass() const { return _viewObjectClass; }

	public slots:
		void onCreateView(const QString &viewPath);
		void onSaveView();
		void onSaveViewAs();

	signals:
		void viewPathChanged(const QString &viewPath);
		void viewNameChanged(const QString &viewName);
		void viewObjectChanged(libraryObjects::AObject *viewObject);
		void viewObjectClassChanged(libraryObjects::ObjectClass *viewObjectClass);

	private:
		const QString _viewExt = ".view";
		QString _viewPath;
		QString _viewName;
		libraryObjects::AObject *_viewObject;
		libraryObjects::ObjectClass *_viewObjectClass;

	// Current Object
	public:
		libraryObjects::AObject *currentObject() const { return _currentObject; }

	public slots:
		void setCurrentObject(libraryObjects::AObject *currentObject);

	signals:
		void currentObjectChanged(libraryObjects::AObject *currentObject);

	private:
		libraryObjects::AObject *_currentObject;

	// Object getter
	public:
		static libraryObjects::AObject *getViewObject(const QString &viewPath);
		static libraryObjects::ObjectClass *getViewObjectClass(const QString &viewPath);
	};
}
