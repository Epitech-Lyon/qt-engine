/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ComboBoxFilter
*/

#include "moc_ComboBoxFilter.cpp"
#include "ComboBoxFilter.hpp"

qtengine::ComboBoxFilter::ComboBoxFilter(QWidget *parent)
	: QComboBox(parent)
	, _isBusy(false)
{
	setMaxVisibleItems(7);
	setEditable(true);
	setCompleter(nullptr);
	connect(this, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, [this](const QString &text) { emit currentDataChanged(_datas[_items.indexOf(text)]); });
	connect(this, &QComboBox::editTextChanged, this, &ComboBoxFilter::onEditTextChanged);
}

void qtengine::ComboBoxFilter::setItems(const QStringList &items)
{
	if (_isBusy) { return; }

	_isBusy = true;
	clear();
	_datas.clear();
	_items = items;
	addItems(_items);
	setCurrentIndex(-1);
	_isBusy = false;
}

void qtengine::ComboBoxFilter::setDatas(const QList<QVariant> &datas)
{
	if (_isBusy) { return; }

	_isBusy = true;
	for (int i = 0; i < datas.size() && i < _items.size(); i += 1)
		_datas[i] = datas[i];
	_isBusy = false;
}

void qtengine::ComboBoxFilter::onEditTextChanged(const QString &text)
{
	if (_isBusy) { return; }

	_isBusy = true;
	auto newItems = _items.filter(QRegExp(text, Qt::CaseSensitive, QRegExp::Wildcard));
	auto currentText = this->currentText();

	clear();
	addItems(newItems);
	setCurrentIndex(newItems.indexOf(currentText));
	setEditText(text);
	_isBusy = false;
}
