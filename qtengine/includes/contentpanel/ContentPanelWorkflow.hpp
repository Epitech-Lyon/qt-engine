/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#pragma once

#include "ContentPanelBase.hpp"

#include <QtWidgets/QListWidget>

namespace QtNodes {
	class FlowView;
	class FlowScene;
};

namespace libraryObjects {
	class AObject;
}

namespace qtengine {
	class ListWidget : public QListWidget {
		Q_OBJECT

	public:
		ListWidget(QWidget *parent = nullptr);
		~ListWidget() = default;

		void addFunction(const QString &name);
		void addVariable(const QString &name);

	private:
	};

	class ContentPanelWorkflow : public ContentPanelBase {
		Q_OBJECT

	public:
		ContentPanelWorkflow(QWidget *parent = nullptr);
		~ContentPanelWorkflow() = default;

		void init() override;

	private slots:
		void onViewObjectChanged(libraryObjects::AObject *viewObject);
		void onAddClicked();

	private:
		QtNodes::FlowView *_view;
		QtNodes::FlowScene *_scene;
		ListWidget *_listPublic;
		ListWidget *_listProtected;
		ListWidget *_listPrivate;
	};
}
