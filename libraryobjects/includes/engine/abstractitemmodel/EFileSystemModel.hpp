/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EFileSystemModel
*/

#pragma once

#include "Object.hpp"
#include <QtWidgets/QFileSystemModel>

namespace libraryObjects {
	typedef Object<QFileSystemModel> EFileSystemModel;

	namespace FileSystemModel {
		void setRootPath(AObject *object);
	}
}
