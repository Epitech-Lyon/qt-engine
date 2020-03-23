/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** EFileSystemModel
*/

#include "EFileSystemModel.hpp"
#include "EAbstractItemModel.hpp"

#include "LibraryFunction.hpp"
#include "ObjectManager.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QAbstractItemView>

template<> void libraryObjects::EFileSystemModel::init(AObject *object)
{
	EAbstractItemModel::init(object);
}

template<> QJsonObject libraryObjects::EFileSystemModel::serializeData(AObject *object)
{
	auto fileSystemModel = dynamic_cast<QFileSystemModel*>(object->object());
	if (!fileSystemModel) { return QJsonObject(); }

	QJsonObject json;
	json["rootPath"] = fileSystemModel->rootPath();
	return json;
}

template<> void libraryObjects::EFileSystemModel::deserializeData(const QJsonObject &json, AObject *object)
{
	auto fileSystemModel = dynamic_cast<QFileSystemModel*>(object->object());
	if (!fileSystemModel) { return; }

	auto parent = object->parent();
	if (!parent) { return; }

	auto abstractItemView = dynamic_cast<QAbstractItemView*>(parent->object());
	if (!abstractItemView) { return; }

	abstractItemView->setRootIndex(fileSystemModel->setRootPath(json["rootPath"].toString()));
}

template<> QString libraryObjects::EFileSystemModel::classIncludePath()
{
	return "#include <QtWidgets/QFileSystemModel>";
}

template<> QIcon libraryObjects::EFileSystemModel::icon()
{
	return QIcon();
}

template<> libraryObjects::LibraryFunction *libraryObjects::EFileSystemModel::libraryFunction()
{
	auto libraryFunction = EAbstractItemModel::libraryFunction();

	libraryFunction->addFunctionMenu(LibraryFunction::FunctionMenu("setRootPath", FileSystemModel::setRootPath));
	return libraryFunction;
}

template<> QString libraryObjects::EFileSystemModel::code(AObject *object)
{
	QString code = EAbstractItemModel::code(object);
	auto fileSystemModel = dynamic_cast<QFileSystemModel*>(object->object());
	if (!fileSystemModel) { return code; }

	if (!fileSystemModel->rootPath().isEmpty())
		code += ObjectManager::instance()->objectName(object->id()) + "->setRootPath(\"" + fileSystemModel->rootPath() + "\");\n";
	return code;
}

void libraryObjects::FileSystemModel::setRootPath(AObject *object)
{
	auto parent = object->parent();
	if (!parent) { return; }

	auto abstractItemView = dynamic_cast<QAbstractItemView*>(parent->object());
	if (!abstractItemView) { return; }

	auto fileSystemModel = dynamic_cast<QFileSystemModel*>(object->object());
	if (!fileSystemModel) { return; }

	auto rootPath = QFileDialog::getExistingDirectory(nullptr, "FileSystemModel setRootPath", QDir::homePath());
	if (rootPath.isEmpty()) { return; }

	abstractItemView->setRootIndex(fileSystemModel->setRootPath(rootPath));
}
