/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Exporter
*/

#pragma once

#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QJsonObject>
#include <QtCore/QTextStream>
#include <QtCore/QMetaMethod>

#include "ClassTypeExporter.hpp"

#define QT_ENGINE_HEADER								\
"/***********************************************************\n\
    THIS FILE HAS BEEN GENERATED AUTOMATICLY BY QT-ENGINE\n\
    YOU MAY NOT EDIT THIS FILE CAUSE IT MAY BE OVERRITTEN\n\
 ************************************************************/"
#define JSON_CHILD_NAME "Children"
#define JSON_DATA_NAME "Data"
#define JSON_DYNAMIC_PROPERTY "dynamic"
#define JSON_CODE_PROPERTY "code"
#define JSON_PROPERTIES_NAME "Properties"
#define JSON_CODE_NAME "Code"
#define JSON_OBJECT_NAME "objectName"
#define EXPORT_NAMESPACE "QTEngine"

namespace libraryObjects {
	class Exporter : public QThread {
		Q_OBJECT

		public:
			Exporter(const QString &exportedDirPath, const QString &generateMainFrom, const QStringList &views);
			~Exporter();

			void run() override;
			void writeObjectSource(QTextStream &stream, QList<QPair<QString, QString>> &vars, const QJsonObject &data, unsigned int tabWidth = 0, const QJsonObject *parent = nullptr);
			void writeMain(QString sourceFile, QString className);
			void writeClass(QString sourceFile, QString headerFile, QJsonObject data);
			void writeConstructors(QTextStream &source, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &funs, QString className);
			void writeMethods(QTextStream &source, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &funs, QString className);
			void writeSlots(QTextStream &source, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &funs, QString className);
			void writeProperties(QTextStream &source, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &funs, QString className);
			QJsonObject loadJson(QString file);

		signals:
			void currentViewExportedChanged(int index);
			void error(const QString &errorMessage);

		private:
			QString _exportedDirPath;
			QString _generateMainFrom;
			QStringList _views;
	};
}
