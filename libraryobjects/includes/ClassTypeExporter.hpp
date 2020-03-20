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
		QStringList body() const { return _body; }

	private:
		types::ClassType *_classType;
		QStringList _body;

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

		void throwMessage(const QString &errorMessage) const;
		void parse();
		void parseBlock(const QUuid &blockId);
		QUuid findBlock(const QString &blockName) const;

		QMap<QUuid, QJsonObject> _blocks;
		QMap<QUuid, QVector<Connection>> _outConnections;
		QMap<QUuid, QVector<Connection>> _inConnections;
	};
}
