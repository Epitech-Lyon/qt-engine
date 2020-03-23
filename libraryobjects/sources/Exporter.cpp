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
#include "Constructor.hpp"
#include "Property.hpp"
#include "QVariantConverter.hpp"
#include "LibraryObjectManager.hpp"
#include "LibraryObject.hpp"

#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QFileInfo>

libraryObjects::Exporter::Exporter(const QString &exportedDirPath, const QString &generateMainFrom, const QStringList &views)
	: _exportedDirPath(exportedDirPath)
	, _generateMainFrom(generateMainFrom)
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

void libraryObjects::Exporter::writeMain(QString path, QString className)
{
	QFile sourceFile(path);

	if (!sourceFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
		throw QString("Could not open ") + path;
	QTextStream stream(&sourceFile);

	stream << QT_ENGINE_HEADER << Qt::endl;
	stream << "#include <QtWidgets/QApplication>" << Qt::endl;
	stream << "#include \"" + className + ".hpp\"" << Qt::endl << Qt::endl;
	stream << "int main(int ac, char **av)" << Qt::endl << "{" << Qt::endl;
	stream << "\tsetenv(\"FONTCONFIG_PATH\", \"/etc/fonts\", 0);" << Qt::endl << Qt::endl;
	stream << "\tQApplication app(ac, av);" << Qt::endl;
	stream << "\t" << EXPORT_NAMESPACE << "::" << className << " _" << className << ";" << Qt::endl << Qt::endl;
	stream << "\t_" << className << ".show();" << Qt::endl;
	stream << "\treturn app.exec();" << Qt::endl;
	stream << "}" << Qt::endl;
}

void libraryObjects::Exporter::writeObjectSource(QTextStream &stream, QList<QPair<QString, QString>> &vars, const QJsonObject &data, unsigned int tabWidth, const QJsonObject *parent)
{
	QString tabs;
	QJsonObject obj = data[data.keys()[0]].toObject();
	QString name = libraryObjects::QVariantConverter::
		deserialize(obj[JSON_PROPERTIES_NAME].toObject()[JSON_OBJECT_NAME].toString()).toString();
	QJsonArray childs = obj[JSON_CHILD_NAME].toArray();
	QJsonObject props = obj[JSON_PROPERTIES_NAME].toObject();

	if (name.isNull() || obj.empty())
		throw "Malformed Json";
	for (unsigned int i = 0; i < tabWidth; i++)
		tabs += "\t";
	if (parent) {
		QJsonObject p_obj = (*parent)[parent->keys()[0]].toObject();
		QString p_name = libraryObjects::QVariantConverter::
			deserialize(p_obj[JSON_PROPERTIES_NAME].toObject()[JSON_OBJECT_NAME].toString()).toString();

		stream << tabs << name << " = new " << data.keys()[0] << ";" << Qt::endl;
		/*
		if (tabWidth == 2)
			p_name = "this";
		if (p_name.isNull() || p_obj.empty())
			throw "Malformed Json";
		if (parent->keys()[0].contains(QRegExp("[Ll]ayout"))) {
			if (data.keys()[0].contains(QRegExp("[Ll]ayout")))
				stream << tabs << name << "->addLayout(" << p_name << ");" << Qt::endl;
			else
				stream << tabs << name << "->setLayout(" << p_name << ");" << Qt::endl;
		} else
			stream << tabs << name << "->setParent(" << p_name << ");" << Qt::endl;
		*/
	}
	if (!childs.empty()) {
		if (parent)
			stream << tabs << "{" << Qt::endl;
		for (const auto &iter : childs)
			writeObjectSource(stream, vars, iter.toObject(), (parent ? tabWidth + 1 : tabWidth), &data);
		if (parent)
			stream << tabs << "}" << Qt::endl << Qt::endl;
	}
	if (!props.empty()) {
		for (auto key : props.keys()) {
			auto value = libraryObjects::QVariantConverter::deserialize(props[key]);

			if (!value.isNull())
				stream << tabs << (parent ? name : "this") << "->setProperty(\"" << key << "\", " <<
					libraryObjects::QVariantConverter::toString(value) << ");" << Qt::endl;
		}
	}
	for (auto line : obj[JSON_CODE_NAME].toString().split("\n"))
		if (!line.isEmpty())
			stream << tabs << line << Qt::endl;
	if (parent)
		vars.append(QPair<QString, QString>(data.keys()[0], name));
}

void libraryObjects::Exporter::writeConstructors(QTextStream &stream, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &functions, QString className)
{
	for (const auto &funs : functions) {
		for (const auto &iter : funs) {
			auto a = dynamic_cast<types::Constructor *>(iter->classType());
			QString sep = "";
			QString tabs = "\t";
			auto body = iter->body().split("\n");

			if (!a)
				throw "Dynamic cast to 'types::Constructor *' failed";
			stream << Qt::endl << EXPORT_NAMESPACE << "::" << className << "::" << className << "(";
			for (const auto &params : a->parameters()) {
				stream << sep << params.first << (params.second.isEmpty() ? "" : " " + params.second);
				sep = ", ";
			}
			stream << ")" << Qt::endl << "{" << Qt::endl;
			stream << "\t__init__();" << Qt::endl;
			for (const auto &line : body) {
				if (line.isEmpty())
					continue;
				if (line[0] == '}')
					tabs.remove(0, 1);
				stream << tabs << line << Qt::endl;
				if (line[line.size() - 1] == '{')
					tabs += "\t";
			}
			stream << "}" << Qt::endl;
		}
	}
}

void libraryObjects::Exporter::writeMethods(QTextStream &stream, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &functions, QString className)
{
	for (const auto &funs : functions) {
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
}

void libraryObjects::Exporter::writeSlots(QTextStream &stream, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &functions, QString className)
{
	for (const auto &funs : functions) {
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
}

void libraryObjects::Exporter::writeProperties(QTextStream &stream, const QMap<QMetaMethod::Access, QList<std::shared_ptr<ClassTypeExporter>>> &functions, QString className)
{
	for (const auto &funs : functions) {
		for (const auto &iter : funs) {
			auto a = dynamic_cast<types::Property *>(iter->classType());
			QString sep = "";
			QString tabs = "\t";
			QString getter;
			QString setter;
			auto body = iter->body().split("\n");

			if (!a)
				throw "Dynamic cast to 'types::Property *' failed";
			getter = a->getterName();
			setter = a->setterName();
			if (!getter.isEmpty()) {
				stream << Qt::endl << a->type() << " " << EXPORT_NAMESPACE << "::" <<
					className << "::" << getter << "() const" << Qt::endl << "{" << Qt::endl;
				stream << "\treturn " << a->name() << ";" << Qt::endl << "}" << Qt::endl;
			}
			if (!setter.isEmpty()) {
				stream << Qt::endl << "void " << EXPORT_NAMESPACE << "::" <<
					className << "::" << setter << "(const " << a->type() << "& value)" << Qt::endl << "{" << Qt::endl;
				stream << "\t " << a->name() << " = value;" << Qt::endl << "}" << Qt::endl;
			}
		}
	}
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
		auto constructors = jsonClass["constructors"].toArray();
		auto slot = jsonClass["slots"].toArray();
		auto signal = jsonClass["signals"].toArray();
		auto methods = jsonClass["methods"].toArray();
		auto properties = jsonClass["properties"].toArray();

		auto fun = [&functions](const QJsonArray &array, types::ClassType::Type type) {
			for (auto iter : array) {
				ClassTypeExporter *exporter = new ClassTypeExporter(iter.toObject());

				functions[type][exporter->classType()->access()].push_back(std::shared_ptr<ClassTypeExporter>(exporter));
			}
		};
		fun(constructors, types::ClassType::Type::CONSTRUCTOR);
		fun(slot, types::ClassType::Type::SLOT);
		fun(signal, types::ClassType::Type::SIGNAL);
		fun(methods, types::ClassType::Type::METHOD);
		fun(properties, types::ClassType::Type::PROPERTY);
	}

	// source file
	stream << QT_ENGINE_HEADER << Qt::endl << Qt::endl;
	headerStream << QT_ENGINE_HEADER << Qt::endl << Qt::endl;
	headerStream << "#pragma once" << Qt::endl;
	stream << "#include \"" << QFileInfo(headerFile).fileName() << "\"" << Qt::endl << Qt::endl;
	stream << EXPORT_NAMESPACE << "::" << className << "::~" <<
		className << "()" << Qt::endl << "{" << Qt::endl;
	for (const auto &key : vars)
		stream << "\t" << "delete " << key.second << ";" << Qt::endl;
	stream << "}" << Qt::endl;
	writeConstructors(stream, functions[types::ClassType::Type::CONSTRUCTOR], className);
	stream << "void " << EXPORT_NAMESPACE << "::" << className << "::__init__()" << Qt::endl;
	stream << "{" << Qt::endl;
	writeObjectSource(stream, vars, data, 1);
	stream << "}" << Qt::endl << Qt::endl;
	writeMethods(stream, functions[types::ClassType::Type::METHOD], className);
	writeSlots(stream, functions[types::ClassType::Type::SLOT], className);
	writeProperties(stream, functions[types::ClassType::Type::PROPERTY], className);
	stream.flush();

	// header file
	for (const auto &key : vars) {
		auto lib = libraryObjects::LibraryObjectManager::instance()->libraryObjectOfClassName(key.first);

		if (lib)
			headerStream << lib->classIncludePath() << Qt::endl;
	}
	headerStream << "#include <QtCore/QVariant>" << Qt::endl;
	headerStream << "#include <QtCore/QLocale>" << Qt::endl << Qt::endl;
	headerStream << "namespace " << EXPORT_NAMESPACE << " {" << Qt::endl;
	headerStream << "\tclass " << className << " : public " << type << " {" << Qt::endl;
	headerStream << "\t\tQ_OBJECT" << Qt::endl << Qt::endl;
	headerStream << "\t\tpublic:" << Qt::endl;
	headerStream << "\t\t\t~" << className << "();" << Qt::endl << Qt::endl;

	auto putGetter = [&headerStream](types::Property *a) {
			QString getter;
			QString setter;

			if (!a)
				throw "Dynamic cast to 'types::Property *' failed";
			getter = a->getterName();
			setter = a->setterName();
			if (!getter.isEmpty())
				headerStream << Qt::endl << a->type() << " " << getter << "() const;" << Qt::endl;
			if (!setter.isEmpty())
				headerStream << Qt::endl << "void " << setter << "(const " << a->type() << "& value);" << Qt::endl;
	};
	for (const auto &funs : functions) {
		for (const auto &iter : funs[QMetaMethod::Access::Public]) {
			if (iter->classType()->type() == types::ClassType::Type::SLOT ||
				iter->classType()->type() == types::ClassType::Type::SIGNAL)
				continue;
			headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
			if (iter->classType()->type() == types::ClassType::Type::PROPERTY)
				putGetter(dynamic_cast<types::Property *>(iter->classType()));
		}
	}
	for (const auto &key : vars)
		headerStream << "\t\t\t" << key.first << " *" << key.second << ";" << Qt::endl;
	headerStream << "\t\tprivate:" << Qt::endl;
	headerStream << "\t\t\tvoid __init__();" << Qt::endl;
	for (const auto &funs : functions) {
		for (const auto &iter : funs[QMetaMethod::Access::Private]) {
			if (iter->classType()->type() == types::ClassType::Type::SLOT ||
				iter->classType()->type() == types::ClassType::Type::SIGNAL)
				continue;
			headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
			if (iter->classType()->type() == types::ClassType::Type::PROPERTY)
				putGetter(dynamic_cast<types::Property *>(iter->classType()));
		}
	}
	bool test = false;

	for (const auto &funs : functions) {
		for (const auto &iter : funs[QMetaMethod::Access::Protected]) {
			if (iter->classType()->type() == types::ClassType::Type::SLOT ||
				iter->classType()->type() == types::ClassType::Type::SIGNAL)
				continue;
			if (!test)
				headerStream << "\t\tprotected:" << Qt::endl;
			test = true;
			headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
			if (iter->classType()->type() == types::ClassType::Type::PROPERTY)
				putGetter(dynamic_cast<types::Property *>(iter->classType()));
		}
	}
	if (functions[types::ClassType::Type::SIGNAL].size()) {
		headerStream << Qt::endl << "\t\tsignals:" << Qt::endl;
		for (const auto &funs : functions[types::ClassType::Type::SIGNAL]) {
			for (const auto &iter : funs) {
				if (iter->classType()->type() != types::ClassType::Type::SIGNAL)
					continue;
				headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
			}
		}
	}
	if (functions[types::ClassType::Type::SLOT].size()) {
		for (const auto &key : functions[types::ClassType::Type::SLOT].keys()) {
			auto fun = functions[types::ClassType::Type::SLOT][key];
			if (key == QMetaMethod::Access::Public && fun.size())
				headerStream << Qt::endl << "\t\tpublic slots:" << Qt::endl;
			if (key == QMetaMethod::Access::Private && fun.size())
				headerStream << Qt::endl << "\t\tprivate slots:" << Qt::endl;
			if (key == QMetaMethod::Access::Protected && fun.size())
				headerStream << Qt::endl << "\t\tprotected slots:" << Qt::endl;
			for (const auto &iter : functions[types::ClassType::Type::SLOT][key]) {
				if (iter->classType()->type() != types::ClassType::Type::SLOT)
					continue;
				headerStream << "\t\t\t" << iter->signature() << ";" << Qt::endl;
			}
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
		if (!_generateMainFrom.isEmpty())
			writeMain(_exportedDirPath + "/Main.cpp", _generateMainFrom);
	} catch (const char *e) {
		qCritical() << "Error:" << e;
		emit error(e);
	} catch (const QString &e) {
		qCritical() << "Error:" << e;
		emit error(e);
	}
}
