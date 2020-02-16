/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LibraryObjectMimeData
*/

#pragma once

#include <QtCore/QMimeData>

namespace libraryObjects {
	class LibraryObject;

	class LibraryObjectMimeData : public QMimeData {
	public:
		LibraryObjectMimeData(LibraryObject *libraryObject);
		~LibraryObjectMimeData() = default;

		LibraryObject *libraryObject() const { return _libraryObject; }

	private:
		LibraryObject *_libraryObject;
	};
}
