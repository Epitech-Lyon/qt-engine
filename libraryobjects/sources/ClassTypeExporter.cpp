/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeExporter
*/

#include "ClassTypeExporter.hpp"

#include "ClassType.hpp"
#include "Slot.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QDebug>

libraryObjects::ClassTypeExporter::ClassTypeExporter(const QJsonObject &json)
	: _classType(types::ClassType::construct(static_cast<types::ClassType::Type>(json["type"].toInt())))
{
	_classType->deserialize(json);
	if (!_classType->isValid())
		throwMessage("Invalid content");
	if (!_classType->content().isEmpty())
		parse();
}

libraryObjects::ClassTypeExporter::~ClassTypeExporter()
{
	delete _classType;
}

QString libraryObjects::ClassTypeExporter::signature() const
{
	return _classType->signature();
}

void libraryObjects::ClassTypeExporter::throwMessage(const QString &errorMessage) const
{
	QString ret = _classType->signature() + ": " + errorMessage;

	throw ret.toLocal8Bit().data();
}

QUuid libraryObjects::ClassTypeExporter::findBlock(const QString &blockName) const
{
	for (auto &block : _blocks)
		if (block["name"].toString() == blockName)
			return _blocks.key(block);
	return QUuid();
}

void libraryObjects::ClassTypeExporter::parse()
{
	for (auto blockJsonRef : _classType->content()["nodes"].toArray()) {
		auto blockJson = blockJsonRef.toObject();
		auto blockId = QUuid(blockJson["id"].toString());

		_blocks[blockId] = blockJson["model"].toObject();
		_inConnections[blockId].resize(_blocks[blockId]["nbrInput"].toInt());
		_outConnections[blockId].resize(_blocks[blockId]["nbrOutput"].toInt());
	}
	for (auto connectionJsonRef : _classType->content()["connections"].toArray()) {
		auto connectionJson = connectionJsonRef.toObject();
		QUuid inBlockId = QUuid(connectionJson["in_id"].toString());
		int inIdx = connectionJson["in_index"].toInt();
		QUuid outBlockId = QUuid(connectionJson["out_id"].toString());
		int outIdx = connectionJson["out_index"].toInt();

		_inConnections[inBlockId][inIdx] = Connection(outBlockId, outIdx);
		_outConnections[outBlockId][outIdx] = Connection(inBlockId, inIdx);
	}

	auto startBlock = findBlock("START");
	if (startBlock.isNull())
		throwMessage("Cannot find START");

	auto returnBlock = findBlock("RETURN");
	if (returnBlock.isNull())
		throwMessage("Must have at least one RETURN");

	for (auto &block : _blocks)
		if (!block["isValid"].toBool())
			throwMessage("All input must be filled");
	for (auto &outConnections : _outConnections)
		if (outConnections.size() > 0 && outConnections[0].isNull)
			throwMessage("All FlowController's output must be filled");

//	for (auto inConnectionKey : _inConnections.keys()) {
//		qDebug().nospace() << "INPUT Connection of " << inConnectionKey << ":";
//		for (auto &connection : _inConnections[inConnectionKey])
//			qDebug().nospace() << "\t" << connection.receiverBlockId << " " << connection.receiverIdx;
//		qDebug() << "\n";
//	}
//	qDebug() << "\n";
//	for (auto outConnectionKey : _outConnections.keys()) {
//		qDebug().nospace() << "OUTPUT Connection of " << outConnectionKey << ":";
//		for (auto &connection : _outConnections[outConnectionKey])
//			qDebug().nospace() << "\t" << connection.receiverBlockId << " " << connection.receiverIdx;
//		qDebug() << "\n";
//	}
//	qDebug() << "\n-----------------------------------------------------------------\n\n";

	_body = parseBlock(startBlock);
	qDebug() << "\n=================================================================";
	qDebug().nospace() << "CODE for " << _classType->signature() << ":";
	qDebug() << "-----------------------------------------------------------------";
	qDebug().noquote() << _body;
	qDebug() << "=================================================================\n";
}

QString libraryObjects::ClassTypeExporter::parseBlock(const QUuid &blockId) const
{
	QString code = _blocks[blockId]["code"].toString();

	qDebug() << "BEGIN" << _blocks[blockId]["name"].toString() << blockId;
	qDebug() << code;

	code = parseObjClassName(blockId, code);
	code = parseObjName(blockId, code);
	code = parseSlotName(blockId, code);
//	code = parseVar(blockId, code);
//	code = parseUseVar(blockId, code);
	code = parseCode(blockId, code);
//	code = parseSkipCode(blockId, code);
	qDebug() << "-----------------------------------------------------------------";
	qDebug() << code;
	qDebug() << "END\n";
	return code;
}

QString libraryObjects::ClassTypeExporter::parseObjClassName(const QUuid &blockId, const QString &code) const
{
	return parseRegexOnConnection(QRegExp("E_OBJCLASSNAME\\((I|O)([0-9])\\)_E"), code, blockId, [this](const Connection &connection) {
		return _blocks[connection.receiverBlockId]["objClassName"].toString();
	});
}

QString libraryObjects::ClassTypeExporter::parseObjName(const QUuid &blockId, const QString &code) const
{
	return parseRegexOnConnection(QRegExp("E_OBJNAME\\((I|O)([0-9])\\)_E"), code, blockId, [this](const Connection &connection) {
		return _blocks[connection.receiverBlockId]["objName"].toString();
	});
}

QString libraryObjects::ClassTypeExporter::parseSlotName(const QUuid &blockId, const QString &code) const
{
	return parseRegexOnConnection(QRegExp("E_SLOTNAME\\((I|O)([0-9])\\)_E"), code, blockId, [this](const Connection &connection) {
		auto classTypeJson = _blocks[connection.receiverBlockId]["classType"].toObject();

		if (static_cast<types::ClassType::Type>(classTypeJson["type"].toInt()) != types::ClassType::SLOT)
			throwMessage("Signals must be connected with a slot");
		types::Slot slot;
		slot.deserialize(classTypeJson);
		return slot.name();
	});
}

QString libraryObjects::ClassTypeExporter::parseVar(const QUuid &, const QString &code) const
{
	QRegExp regex("E_VAR\\((\\w*)\\)_E");

	return parseRegexOn(regex, code, [&regex](const QString &code) {
		QString newCode = code;

		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseUseVar(const QUuid &, const QString &code) const
{
	QRegExp regex("E_USEVAR\\(([0-9])\\)_E");

	return parseRegexOn(regex, code, [&regex](const QString &code) {
		QString newCode = code;

		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseCode(const QUuid &blockId, const QString &code) const
{
	return parseRegexOnConnection(QRegExp("E_CODE\\((I|O)([0-9])\\)_E"), code, blockId, [this](const Connection &connection) {
		return parseBlock(connection.receiverBlockId);
	});
}

QString libraryObjects::ClassTypeExporter::parseSkipCode(const QUuid &, const QString &code) const
{
	QRegExp regex("E_SKIPCODE\\((I|O)([0-9])\\)_E");

	return parseRegexOn(regex, code, [](const QString &code) {
		QString newCode = code;

		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseRegexOn(const QRegExp &regex, const QString &code, RegExpCallback callBack) const
{
	QString newCode = code;
	int pos = 0;

	while ((pos = regex.indexIn(newCode)) != -1)
		newCode = callBack(newCode);
	return newCode;
}

QString libraryObjects::ClassTypeExporter::parseRegexOnConnection(const QRegExp &regex, const QString &code, const QUuid &blockId, ConnectionCallback callBack) const
{
	return parseRegexOn(regex, code, [this, &regex, &blockId, callBack](const QString &code) {
		QString newCode = code;
		QString portType = regex.cap(1);
		int portIdx = regex.cap(2).toInt();
		Connection connection = portType == "I"
			? _inConnections[blockId][portIdx]
			: portType == "O"
				? _outConnections[blockId][portIdx]
				: Connection();

		if (connection.isNull)
			newCode.replace(regex.pos(0), regex.matchedLength(), "");
		else
			newCode.replace(regex.pos(0), regex.matchedLength(), callBack(connection));
		return newCode;
	});
}
