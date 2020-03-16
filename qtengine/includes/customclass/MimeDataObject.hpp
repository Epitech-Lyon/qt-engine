/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** MimeDataObject
*/

#pragma once

#include <QtCore/QMimeData>

namespace libraryObjects {
	class AObject;
	class LibraryObject;
	class ObjectClass;
}

namespace qtengine {
	class MimeDataObject : public QMimeData {
	public:
		MimeDataObject(const QObject *sender, libraryObjects::ObjectClass *objectClass, libraryObjects::LibraryObject *libraryObject, libraryObjects::AObject *reference = nullptr);
		~MimeDataObject();

		const QObject *sender() const { return _sender; }
		libraryObjects::ObjectClass *objectClass() const { return _objectClass; }
		libraryObjects::LibraryObject *libraryObject() const { return _libraryObject; }
		libraryObjects::AObject *reference() const { return _reference; }

	private:
		const QObject *_sender;
		libraryObjects::ObjectClass *_objectClass;
		libraryObjects::LibraryObject *_libraryObject;
		libraryObjects::AObject *_reference;
	};
}
