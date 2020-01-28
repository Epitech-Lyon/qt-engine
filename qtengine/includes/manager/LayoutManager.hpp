/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutManager
*/

#pragma once

#include "ISerializable.hpp"
#include <QtCore/QObject>
#include <QtCore/QString>

namespace qtengine {
	class LayoutManager : public QObject, public ISerializable {
		Q_OBJECT

	public:
		LayoutManager() = default;
		~LayoutManager() = default;

		QJsonObject serialize() const override;
		void deserialize(const QJsonObject &) override;

		QStringList layoutsName() const { return _json.keys(); }
		QString layoutName() const { return _layoutName; }

		void openLayout(const QString &);

	signals:
		void layoutsNameChanged(const QStringList &);

	public slots:
		void onSaveLayout();
		void onDeleteLayout();

	private:
		QJsonObject _json;
		QString _layoutName;
	};
}
