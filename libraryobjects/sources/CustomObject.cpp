/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** CustomObject
*/

#include "CustomObject.hpp"

#include "LibraryObjectManager.hpp"
#include "ViewConverter.hpp"
#include "LibraryFunction.hpp"
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>

libraryObjects::CustomObject::CustomObject(QObject *object, const QString &classHierarchy)
	: AObject(object, classHierarchy)
{
}

void libraryObjects::CustomObject::registerAsLibraryObject(const QString &path)
{
	auto constructor = [path]() {
		AObject *root = initRoot(path);
		QObject *object = nullptr;
		QString classHierarchy;
		CustomObject *ret = nullptr;

		if (root) {
			object = root->object();
			object->setObjectName(QFileInfo(path).baseName());
			classHierarchy = root->classHierarchy() + "::" + object->objectName();
			root->removeObject();
			delete root;
			ret = new CustomObject(object, classHierarchy);
		}
		return ret;
	};
	auto object = constructor();
	auto serializeData = [](AObject *) { return QJsonObject(); };
	auto deserializeData = [](const QJsonObject &, AObject *) {};
	auto code = [](AObject *) { return ""; };
	QString classIncludePath = "#include \"" + object->className() + ".hpp\"";

	if (object) {
		auto librayObject = new LibraryObject(constructor, serializeData, deserializeData, object->classHierarchy(), classIncludePath, QIcon(), new LibraryFunction(), code);

		LibraryObjectManager::instance()->registerCustomObject(QFileInfo(path).fileName(), librayObject);
		delete object;
	}
}

libraryObjects::AObject *libraryObjects::CustomObject::initRoot(const QString &path)
{
	QFileInfo fileInfo(path);
	QFile file(fileInfo.filePath());
	QJsonObject json;

	if (file.open(QIODevice::ReadOnly)) {
		json = QJsonDocument::fromJson(file.readAll()).object();
		file.close();
	}
	return ViewConverter().deserialize(json["Engine"].toObject());
}
