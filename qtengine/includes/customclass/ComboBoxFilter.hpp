/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ComboBoxFilter
*/

#pragma once

#include <QtWidgets/QComboBox>
#include <QtCore/QMap>

namespace qtengine {
	class ComboBoxFilter : public QComboBox {
		Q_OBJECT

	public:
		ComboBoxFilter(QWidget *parent = nullptr);
		~ComboBoxFilter() = default;

		void setItems(const QStringList &items);
		void setDatas(const QList<QVariant> &datas);

	signals:
		void currentDataChanged(const QVariant &data);

	private slots:
		void onEditTextChanged(const QString &text);

	private:
		QStringList _items;
		QMap<int, QVariant> _datas;
		bool _isBusy;
	};
}
