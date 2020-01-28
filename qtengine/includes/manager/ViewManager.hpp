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

		QString currentView() const { return _currentView; }
		void openView(const QString &);

	signals:
		void currentViewChanged(const QString &);

    private:
		QString _currentView;
    };
}
