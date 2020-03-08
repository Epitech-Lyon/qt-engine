/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Generator
*/

#include "Generator.hpp"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

engineGenerator::Generator::Generator(const QString &qtDir, const QString &engineDir)
	: _qtDir(qtDir)
	, _engineDir(engineDir)
{
}

int engineGenerator::Generator::generate(const QString &qtDir, const QString &engineDir)
{
	Generator(qtDir, engineDir).parseDir(engineDir);
	return 0;
}

void engineGenerator::Generator::parseDir(const QString &dirPath)
{
	QDir dir(dirPath);
	if (!dir.exists()) return (void)(qDebug() << dirPath << "doesn't exists !");

	for (auto fileInfo : dir.entryInfoList(QDir::Files))
		parseFile(fileInfo.absoluteFilePath());
	for (auto fileInfo : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
		parseDir(fileInfo.absoluteFilePath());
}

void engineGenerator::Generator::parseFile(const QString &filePath)
{
	QFile file(filePath);
	if (!file.exists()) { qDebug() << filePath << "doesn't exists !"; return; }
	if (!file.open(QIODevice::ReadOnly)) { qDebug() << "Cannot open" << filePath; return; }

	auto content = file.readAll();
	file.close();

	int begin = content.indexOf(E_BEGIN_GENERATE);
	if (begin == -1) { qDebug() << "Cannot found E_BEGIN_GENERATE in" << filePath; return; }
	begin += QString(QString(E_BEGIN_GENERATE) + "(").size();

	int end = content.indexOf(E_END_GENERATE);
	if (end == -1) { qDebug() << "Cannot found E_END_GENERATE in" << filePath; return; }

	QString key(content.mid(begin, content.indexOf(")", begin) - begin));
	QString fileIncluded = key.split(", ").front();
	QTime lastGeneration = QTime::fromMSecsSinceStartOfDay(key.split(", ").last().toInt());
	QFileInfo qtFileInfo(_qtDir + getKey(fileIncluded));
	QTime qtFileTime = qtFileInfo.fileTime(QFileDevice::FileModificationTime).time();

	if (lastGeneration.isNull() || qtFileTime > lastGeneration) {
		int beginReplace = content.indexOf(key.split(", ").last(), begin);
		int lengthReplace = end - beginReplace; for (; content[beginReplace + lengthReplace - 1] != '\n'; lengthReplace -= 1);
		QByteArray generationTime = QString(QString::number(qtFileTime.msecsSinceStartOfDay()) + ")\n").toUtf8();
		QByteArray indent = content.mid(beginReplace + lengthReplace, end - beginReplace - lengthReplace);
		auto contentGenerated = generateFor(indent, qtFileInfo.absoluteFilePath(), fileIncluded);

		if (!contentGenerated.first) { return; }
		content.replace(beginReplace, lengthReplace, generationTime + contentGenerated.second);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) { qDebug() << "Cannot open" << filePath; return; }
		file.write(content);
		file.close();
	}
}

QString engineGenerator::Generator::getKey(const QString &malformedKey)
{
	auto keys = malformedKey.split("/");
	auto className = keys.takeAt(keys.size() - 1);

	return keys.join("/") + (keys.isEmpty() ? "" : "/") + className.toLower() + ".h";
}

QPair<bool, QByteArray> engineGenerator::Generator::generateFor(const QByteArray &indent, const QString &qtFilePath, const QString &className)
{
	auto returnValue = [](const QByteArray &byteArray = QByteArray()) {
		return QPair<bool, QByteArray>(!byteArray.isEmpty(), byteArray);
	};

	QByteArray ret;
	QFile qtFile(qtFilePath);
	if (!qtFile.open(QIODevice::ReadOnly)) { qDebug() << "Cannot open" << qtFilePath; return returnValue(); }

	QList<QByteArray> lines;
	while (!qtFile.atEnd())
		lines << qtFile.readLine();
	qtFile.close();

	bool beginClassFound = false;
	bool endClassFound = false;
	for (auto line : lines) {
		auto beginClass = findBeginClass(line, className);

		beginClassFound = !beginClassFound ? beginClass.first : beginClassFound;
		if (beginClass.first && !beginClass.second.isEmpty()) {
			QString fileIncluded = findIncludeSuperClass(lines, getKey(beginClass.second));
			QFileInfo qtFileInfoSuperClass(_qtDir + fileIncluded);
			auto contentGeneratedSuperClass = generateFor(indent, qtFileInfoSuperClass.absoluteFilePath(), beginClass.second);

			if (!contentGeneratedSuperClass.first) { return returnValue(); }
			ret += contentGeneratedSuperClass.second;
		} else if (!beginClass.first && beginClassFound) {

		}
	}
	return returnValue(indent + "//TEST\n" + indent + "//" + qtFilePath.toUtf8() + "\n");
}

QPair<bool, QString> engineGenerator::Generator::findBeginClass(const QByteArray &line, const QString &className)
{
}

QString engineGenerator::Generator::findIncludeSuperClass(const QList<QByteArray> &lines, const QString &classNameKey)
{
}

QByteArray engineGenerator::Generator::parseLine(const QByteArray &line)
{
}
