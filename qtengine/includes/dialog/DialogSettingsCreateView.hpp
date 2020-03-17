/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** DialogSettingsCreateView
*/

#pragma once

#include "DialogBase.hpp"

namespace libraryObjects {
	class LibraryObject;
}

namespace qtengine {
	class DialogSettingsCreateView : public DialogBase {
	public:
		DialogSettingsCreateView(const QString &viewPath, QWidget *parent = nullptr);
		~DialogSettingsCreateView() = default;

		QString viewPath() const { return _viewPath; }
		bool isCopy() const { return _isCopy; }
		libraryObjects::LibraryObject *libraryObject() const { return _libraryObject; }
		QString copyFromView() const { return _copyFromView; }

	private:
		QWidget *initBody();
		QString _viewPath;
		bool _isCopy;
		libraryObjects::LibraryObject *_libraryObject;
		QString _copyFromView;
	};
}
