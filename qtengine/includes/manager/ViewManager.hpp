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
	class LibraryObject;
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

		void createView(const QString &viewPath, libraryObjects::LibraryObject *libraryObject);
		void createViewFrom(const QString &viewPath, const QString &viewPathSource);
		void saveView();
		void closeView();

		QString viewExtension() const { return _viewExt; }

		bool viewIsOpened() const { return _viewIsOpened; }
		QString viewPath() const { return _viewPath; }
		QString viewName() const { return _viewName; }
		libraryObjects::AObject *viewObject() const { return _viewObject; }
		libraryObjects::ObjectClass *viewObjectClass() const { return _viewObjectClass; }

	public slots:
		void onOpenView(const QString &viewPath);

	signals:
		void viewOpened(bool isOpened);
		void viewPathChanged(const QString &viewPath);
		void viewNameChanged(const QString &viewName);
		void viewObjectChanged(libraryObjects::AObject *viewObject);
		void viewObjectClassChanged(libraryObjects::ObjectClass *viewObjectClass);

	private:
		const QString _viewExt = ".view";
		bool _viewIsOpened = false;
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
