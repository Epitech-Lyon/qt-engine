/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** TreeWidgetProjectExplorer
*/

#pragma once

#include <QtWidgets/QTreeWidget>
#include <QtCore/QMap>

namespace qtengine {
	class TreeWidgetProjectExplorer : public QTreeWidget {
		Q_OBJECT

	public:
		TreeWidgetProjectExplorer(QWidget *parent = nullptr);
		~TreeWidgetProjectExplorer() = default;

		QTreeWidgetItem *addView(const QString &viewPath);
		void removeView(const QString &viewPath);
		void clearViews();

		void setCurrentView(const QString &viewPath);
		QString currentView() const;

	signals:
		void viewDoubleClicked(const QString &viewPath);

	private:
		QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const override;
		QMap<QTreeWidgetItem *, QString> _viewPath;
	};
}
