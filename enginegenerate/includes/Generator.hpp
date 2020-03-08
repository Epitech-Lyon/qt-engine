/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** Generator
*/

#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#define E_BEGIN_GENERATE "E_BEGIN_GENERATE"
#define E_END_GENERATE "E_END_GENERATE"

namespace engineGenerator {
	class Generator {
	public:
		static int generate(const QString &qtDir, const QString &engineDir);

	private:
		Generator(const QString &qtDir, const QString &engineDir);
		~Generator() = default;

		void parseDir(const QString &dirPath);
		void parseFile(const QString &filePath);
		QString getKey(const QString &malformedKey);
		QPair<bool, QByteArray> generateFor(const QByteArray &indent, const QString &qtFilePath, const QString &className);
		QPair<bool, QString> findBeginClass(const QByteArray &line, const QString &className);
		QString findIncludeSuperClass(const QList<QByteArray> &lines, const QString &classNameKey);
		QByteArray parseLine(const QByteArray &line);

		QString _qtDir;
		QString _engineDir;
	};
}
