/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** CustomObject
*/

#pragma once

#include "AObject.hpp"

namespace libraryObjects {
	class CustomObject : public AObject {
	
	// General
	public:
		~CustomObject() = default;

	private:
		CustomObject(QObject *object, const QString &classHierarchy);

	// Register libraryObject
	public:
		static void registerAsLibraryObject(const QString &path);

	private:
		static AObject *initRoot(const QString &path);
	};
}
