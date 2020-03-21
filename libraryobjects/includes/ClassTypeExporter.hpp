/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeExporter
*/

#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include <QtCore/QUuid>
#include <QtCore/QRegExp>

namespace types {
	class ClassType;
}

namespace libraryObjects {
	class ClassTypeExporter {
	// General
	public:
		ClassTypeExporter(const QJsonObject &json);
		~ClassTypeExporter();

		QString signature() const;

		bool hasBody() const { return _body.size() == 0; }
		QString body() const { return _body; }

	private:
		types::ClassType *_classType;
		QString _body;

	// Parsing
	private:
		typedef struct Connection {
			Connection() : isNull(true), receiverIdx(-1) {}
			Connection(const QUuid &receiverBlockId, int receiverIdx)
				: isNull(false), receiverBlockId(receiverBlockId), receiverIdx(receiverIdx) {}
			Connection(const Connection &other)
				: isNull(false), receiverBlockId(other.receiverBlockId), receiverIdx(other.receiverIdx) {}
			Connection &operator=(const Connection &other)
			{
				isNull = other.isNull;
				receiverBlockId = other.receiverBlockId;
				receiverIdx = other.receiverIdx;
				return *this;
			}

			bool isNull;
			QUuid receiverBlockId;
			int receiverIdx;
		} Connection;
		typedef std::function<QString (const QString &)> RegExpCallback;
		typedef std::function<QString (const Connection &)> ConnectionCallback;

		void throwMessage(const QString &errorMessage) const;
		QUuid findBlock(const QString &blockName) const;
		void parse();
		QString parseBlock(const QUuid &blockId) const;
		QString parseObjClassName(const QUuid &blockId, const QString &code) const;
		QString parseObjName(const QUuid &blockId, const QString &code) const;
		QString parseSlotName(const QUuid &blockId, const QString &code) const;
		QString parseVar(const QUuid &blockId, const QString &code) const;
		QString parseUseVar(const QUuid &blockId, const QString &code) const;
		QString parseCode(const QUuid &blockId, const QString &code) const;
		QString parseSkipCode(const QUuid &blockId, const QString &code) const;
		QString parseRegexOn(const QRegExp &regex, const QString &code, RegExpCallback callBack) const;
		QString parseRegexOnConnection(const QRegExp &regex, const QString &code, const QUuid &blockId, ConnectionCallback callBack) const;

		QMap<QUuid, QJsonObject> _blocks;
		QMap<QUuid, QVector<Connection>> _outConnections;
		QMap<QUuid, QVector<Connection>> _inConnections;
		QMap<QUuid, QString> _varNames;
		QMap<QString, QString> _varValues;
	};
}
