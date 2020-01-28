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

namespace qtengine {
    class ViewManager : public QObject, public ISerializable {
		Q_OBJECT

    public:
        ViewManager() = default;
        ~ViewManager() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

		void openView(const QString &);

		QString viewExtension() const { return _viewExt; }

		QString viewPath() const { return _viewPath; }
		QString viewName() const { return _viewName; }
		QJsonObject viewObject() const { return _viewObject; }

	public slots:
		void onCreateView(const QString &);
		void onSaveView(const QJsonObject &);
		void onSaveViewAs(const QJsonObject &);

	signals:
		void requestForSave();
		void requestForSaveAs();

		void viewPathChanged(const QString &);
		void viewNameChanged(const QString &);
		void viewObjectChanged(const QJsonObject &);

    private:
		const QString _viewExt = ".view";
		QString _viewPath;
		QString _viewName;
		QJsonObject _viewObject;
    };
}
