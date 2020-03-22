/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Exporter
*/

#include "moc_Exporter.cpp"
#include "Exporter.hpp"

#include "ClassType.hpp"
#include "Method.hpp"
#include "Slot.hpp"
#include "ClassTypeExporter.hpp"
#include "QVariantConverter.hpp"

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QFileInfo>

libraryObjects::Exporter::Exporter(const QString &exportedDirPath, bool generateMain, const QStringList &views)
	: _exportedDirPath(exportedDirPath)
	, _generateMain(generateMain)
	, _views(views)
{
}

libraryObjects::Exporter::~Exporter()
{
}

QJsonObject libraryObjects::Exporter::loadJson(QString filename)
{
	QFile file(filename);
	QJsonObject json;

	if (!file.open(QIODevice::ReadOnly))
		throw "Error opening file";
	json = QJsonDocument::fromJson(file.readAll()).object();
	file.close();
	return json;
}

void libraryObjects::Exporter::writeObjectSource(QTextStream &stream, QList<QPair<QString, QString>> &vars, const QJsonObject &data, unsigned int tabWidth, const QJsonObject *parent)
{
	QString tabs;
	QJsonObject obj = data[data.keys()[0]].toObject();
	QString name = libraryObjects::QVariantConverter::
		deserialize(obj[JSON_PROPERTIES_NAME].toObject()[JSON_OBJECT_NAME].toString()).toString();
	QJsonArray childs = obj[JSON_CHILD_NAME].toArray();
	QJsonObject props = obj[JSON_PROPERTIES_NAME].toObject();

	if (name.isNull())
		name = data.keys()[0]; // TODO FIXME HACK Tempory hack
	if (name.isNull() || obj.empty())
		throw "Malformed Json";
	for (unsigned int i = 0; i < tabWidth; i++)
		tabs += "\t";
	stream << tabs << "_" << name << " = new " << data.keys()[0];
	if (parent) {
		QJsonObject p_obj = (*parent)[parent->keys()[0]].toObject();
		QString p_name = libraryObjects::QVariantConverter::
			deserialize(p_obj[JSON_PROPERTIES_NAME].toObject()[JSON_OBJECT_NAME].toString()).toString();

		if (p_name.isNull())
			p_name = parent->keys()[0]; // TODO FIXME HACK Tempory hack
		if (p_name.isNull() || p_obj.empty())
			throw "Malformed Json";
		if (parent->keys()[0].contains(QRegExp("[Ll]ayout"))) {
			stream << ";" << Qt::endl;
			if (data.keys()[0].contains(QRegExp("[Ll]ayout")))
				stream << tabs << "_" << name << "->addLayout(_" << p_name << ");" << Qt::endl;
			else
				stream << tabs << "_" << name << "->setLayout(_" << p_name << ");" << Qt::endl;
		} else
			stream << "(_" << p_name << ");" << Qt::endl;
	} else
		stream << ";" << Qt::endl;
	stream << Qt::endl;
	if (!childs.empty()) {
		stream << tabs << "{" << Qt::endl;
		for (const auto &iter : childs)
			writeObjectSource(stream, vars, iter.toObject(), tabWidth + 1, &data);
		stream << tabs << "}" << Qt::endl << Qt::endl;
	}
	if (!props.empty()) {
		for (auto key : props.keys()) {
			auto value = libraryObjects::QVariantConverter::deserialize(props[key]);

			if (!value.isNull())
				stream << tabs << "_" << name << "->setProperty(\"" << key << "\", " <<
					libraryObjects::QVariantConverter::toString(value) << ");" << Qt::endl;
		}
	}
	vars.append(QPair<QString, QString>(data.keys()[0], name));
}

void libraryObjects::Exporter::writeClass(QString source, QString header, QJsonObject obj)
{
	QFile sourceFile(source);
	QFile headerFile(header);
	QJsonObject data = obj["Engine"].toObject();
	QMap<types::ClassType::Type, QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>>> functions;

	if (data.keys().size() != 1)
		throw "Malformed Json";
	if (!sourceFile.open(QIODevice::WriteOnly | QIODevice::Truncate) ||
		!headerFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
		throw "Error opening files";
	QTextStream stream(&sourceFile);
	QTextStream headerStream(&headerFile);
	QList<QPair<QString, QString>> vars;
	QString className = QFileInfo(sourceFile).baseName();
	QString type = data.keys()[0];

	// Export functions
	{
		QJsonObject jsonClass = obj["Class"].toObject();
		// auto constructors = jsonClass["constructors"].toArray();
		auto slot = jsonClass["slots"].toArray();
		auto signal = jsonClass["signals"].toArray();
		auto methods = jsonClass["methods"].toArray();
		// auto properties = jsonClass["properties"].toArray();

		auto fun = [&functions](const QJsonArray &array, types::ClassType::Type type) {
			for (auto iter : array) {
				ClassTypeExporter *exporter = new ClassTypeExporter(iter.toObject());

				functions[type][exporter->classType()->access()].push_back(std::shared_ptr<ClassTypeExporter>(exporter));
			}
		};
		// fun(constructors, types::ClassType::Type::CONSTRUCTOR);
		fun(slot, types::ClassType::Type::SLOT);
		fun(signal, types::ClassType::Type::SIGNAL);
		fun(methods, types::ClassType::Type::METHOD);
	}

	// source file
	stream << QT_ENGINE_HEADER << Qt::endl << Qt::endl;
	headerStream << QT_ENGINE_HEADER << Qt::endl << Qt::endl;
	stream << "#include \"" << QFileInfo(headerFile).fileName() << "\"" << Qt::endl << Qt::endl;
	stream << EXPORT_NAMESPACE << "::" << className << "::" <<
		className << "(QWidget *parent) : " << type << "(parent)" << Qt::endl;
	stream << "{" << Qt::endl;
	writeObjectSource(stream, vars, data, 1);
	stream << "}" << Qt::endl << Qt::endl;
	// TODO add constructors here
	stream << EXPORT_NAMESPACE << "::" << className << "::~" <<
		className << "()" << Qt::endl << "{" << Qt::endl;
	for (const auto &key : vars)
		stream << "\t" << "delete _" << key.second << ";" << Qt::endl;
	stream << "}" << Qt::endl;
	for (const auto &funs : functions[types::ClassType::Type::METHOD]) {
		for (const auto &iter : funs) {
			auto a = dynamic_cast<types::Method *>(iter->classType());
			QString sep = "";
			QString tabs = "\t";
			auto body = iter->body().split("\n");

			if (!a)
				throw "Dynamic cast to 'types::Method *' failed";
			stream << Qt::endl << (a->isStatic() ? "static " : "") << a->returnType() << " " <<
				EXPORT_NAMESPACE << "::" << className << "::" << a->name() << "(";
			for (const auto &params : a->parameters()) {
				stream << sep << params.first << (params.second.isEmpty() ? "" : " " + params.second);
				sep = ", ";
			}
			stream << ")" << (a->isConst() ? " const" : "") << Qt::endl << "{" << Qt::endl;
			for (const auto &line : body) {
				if (line.isEmpty())
					continue;
				if (line[0] == '}')
					tabs.remove(0, 1);
				if (line[line.size() - 1] == '{')
					tabs += "\t";
				stream << tabs << line << Qt::endl;
			}
			stream << "}" << Qt::endl;
		}
	}
	for (const auto &funs : functions[types::ClassType::Type::SLOT]) {
		for (const auto &iter : funs) {
			auto a = dynamic_cast<types::Slot *>(iter->classType());
			QString sep = "";
			QString tabs = "\t";
			auto body = iter->body().split("\n");

			if (!a)
				throw "Dynamic cast to 'types::Slot *' failed";
			stream << Qt::endl << "void " << EXPORT_NAMESPACE << "::" <<
				className << "::" << a->name() << "(";
			for (const auto &params : a->parameters()) {
				stream << sep << params.first << (params.second.isEmpty() ? "" : " " + params.second);
				sep = ", ";
			}
			stream << ")" << (a->isConst() ? " const" : "") << Qt::endl << "{" << Qt::endl;
			for (const auto &line : body) {
				if (line.isEmpty())
					continue;
				if (line[0] == '}')
					tabs.remove(0, 1);
				if (line[line.size() - 1] == '{')
					tabs += "\t";
				stream << tabs << line << Qt::endl;
			}
			stream << "}" << Qt::endl;
		}
	}
	stream.flush();

	// header file
	for (const auto &key : vars)
		headerStream << "#include <QtWidgets/" << key.first << ">" << Qt::endl; // TODO FIXME HACK This only work if the widget is from QT's base classes
	headerStream << "#include <QtCore/QVariant>" << Qt::endl;
	headerStream << "#include <QtCore/QLocale>" << Qt::endl << Qt::endl;
	headerStream << "namespace " << EXPORT_NAMESPACE << " {" << Qt::endl;
	headerStream << "\tclass " << className << " : public " << type << " {" << Qt::endl;
	headerStream << "\t\tQ_OBJECT" << Qt::endl << Qt::endl;
	headerStream << "\t\tpublic:" << Qt::endl;
	headerStream << "\t\t\t" << className << "(QWidget *parent = nullptr);" << Qt::endl;
	headerStream << "\t\t\t~" << className << "();" << Qt::endl << Qt::endl;
	for (const auto &funs : functions) {
		for (const auto &iter : funs[QMetaMethod::Access::Public]) {
			headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
		}
	}
	headerStream << "\t\tprivate:" << Qt::endl;
	for (const auto &funs : functions) {
		for (const auto &iter : funs[QMetaMethod::Access::Private]) {
			headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
		}
	}
	for (const auto &key : vars)
		headerStream << "\t\t\t" << key.first << " *_" << key.second << ";" << Qt::endl;
	headerStream << "\t\tprotected:" << Qt::endl;
	for (const auto &funs : functions) {
		for (const auto &iter : funs[QMetaMethod::Access::Protected]) {
			headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
		}
	}
	headerStream << "\t};" << Qt::endl;
	headerStream << "}" << Qt::endl;
	sourceFile.close();
	headerFile.close();
}

void libraryObjects::Exporter::run()
{
	QDir dir;

	qDebug() << "Export" << _views << "to" << _exportedDirPath << (_generateMain ? "with main" : "without main");
	if (!dir.exists(_exportedDirPath))
		if (!dir.mkpath(_exportedDirPath))
			return (void)(qCritical() << "Error, could not create directory : " << _exportedDirPath);
	try {
		for (int i = 0; i < _views.count(); i += 1) {
			QString baseName = QFileInfo(_views[i]).baseName();
			QJsonObject json = loadJson(QFileInfo(_views[i]).filePath());

			writeClass(_exportedDirPath + "/" + baseName + ".cpp", _exportedDirPath + "/" + baseName + ".hpp", json);
			emit currentViewExportedChanged(i);
		}
	} catch (const char *e) {
		qCritical() << "Error:" << e;
		emit error(e);
	} catch (const QString &e) {
		qCritical() << "Error:" << e;
		emit error(e);
	}
}
