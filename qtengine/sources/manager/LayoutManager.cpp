/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** LayoutManager
*/

#include "moc_LayoutManager.cpp"
#include "LayoutManager.hpp"

#include "Manager.hpp"
#include "MainWindow.hpp"

#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>

QJsonObject qtengine::LayoutManager::serialize() const
{
	QJsonObject json;
	json["Layouts"] = _json;
	json["Current layout"] = _layoutName;
	return json;
}

void qtengine::LayoutManager::deserialize(const QJsonObject &json)
{
	_json = json["Layouts"].toObject();
	emit layoutsNameChanged(layoutsName());
	openLayout(json["Current layout"].toString());
}

void qtengine::LayoutManager::openLayout(const QString &layoutName)
{
	if (!_json.contains(layoutName)) { return; }
	_layoutName = layoutName;
	Manager::instance()->mainWindow()->deserialize(_json[layoutName].toObject());
}

void qtengine::LayoutManager::onSaveLayout()
{
	auto mainWindow = Manager::instance()->mainWindow();
	bool ok = false;
	QString layoutName = QInputDialog::getText(mainWindow, "Save Layout", "Choose a name for your Layout", QLineEdit::Normal, "Name", &ok);

	if (ok && !layoutName.isEmpty()) {
		if (_json.contains(layoutName) && QMessageBox::warning(mainWindow, "Save Layout", layoutName + " already exists, Do you want to replace it ?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
			return;
		_json[layoutName] = mainWindow->serialize();
		_layoutName = layoutName;
		emit layoutsNameChanged(layoutsName());
	}
}

void qtengine::LayoutManager::onDeleteLayout()
{
	auto mainWindow = Manager::instance()->mainWindow();
	bool ok = false;
	QString layoutNameToDel = QInputDialog::getItem(mainWindow, "Delete Layout", "Choose which layout you want to delete", layoutsName(), 0, false, &ok);

	if (ok && !layoutNameToDel.isEmpty()) {
		_json.remove(layoutNameToDel);
		emit layoutsNameChanged(layoutsName());
	}
}
