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

libraryObjects::ClassTypeExporter::ClassTypeExporter(const QJsonObject &json)
	: _classType(types::ClassType::construct(static_cast<types::ClassType::Type>(json["type"].toInt())))
	, _skipCode(false)
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
	throw _classType->signature() + ": " + errorMessage;
}

QUuid libraryObjects::ClassTypeExporter::findBlock(const QString &blockName) const
{
	for (auto &block : _blocks)
		if (block["name"].toString() == blockName)
			return _blocks.key(block);
	return QUuid();
}

QString libraryObjects::ClassTypeExporter::createVarName() const
{
	int i = 1;
	QString ret = "val_";

	while (_varNames.values().contains(ret + QString::number(i)))
		i += 1;
	return ret + QString::number(i);
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
		_outConnections[outBlockId][outIdx].append(Connection(inBlockId, inIdx));
	}

	auto startBlock = findBlock("START");
	if (startBlock.isNull())
		throwMessage("Cannot find START");

	for (auto &block : _blocks)
		if (!block["isValid"].toBool())
			throwMessage("All input must be filled");
	for (auto &outConnections : _outConnections)
		if (outConnections.size() > 0 && outConnections[0].size() > 0 && outConnections[0][0].isNull)
			throwMessage("All FlowController's output must be filled");

	_body = parseBlock(startBlock);
}

QString libraryObjects::ClassTypeExporter::parseBlock(const QUuid &blockId)
{
	QString code = _blocks[blockId]["code"].toString();

	code = parseObjClassName(blockId, code);
	code = parseObjName(blockId, code);
	code = parseSlotName(blockId, code);
	code = parseVar(blockId, code);
	code = parseUseVar(blockId, code);
	code = parseCode(blockId, code);
	code = parseSkipCode(blockId, code);
	return code;
}

QString libraryObjects::ClassTypeExporter::parseObjClassName(const QUuid &blockId, const QString &code)
{
	QRegExp regex("E_OBJCLASSNAME\\(([0-9])\\)_E");

	return parseRegexOn(regex, code, [this, &regex, &blockId](const QString &code) {
		Connection connection = _outConnections[blockId][regex.cap(1).toInt()].front();
		QString newCode = code;

		newCode.replace(regex.pos(0), regex.matchedLength(), _blocks[connection.receiverBlockId]["objClassName"].toString());
		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseObjName(const QUuid &blockId, const QString &code)
{
	QRegExp regex("E_OBJNAME\\(([0-9])\\)_E");

	return parseRegexOn(regex, code, [this, &regex, &blockId](const QString &code) {
		Connection connection = _outConnections[blockId][regex.cap(1).toInt()].front();
		QString newCode = code;

		newCode.replace(regex.pos(0), regex.matchedLength(), _blocks[connection.receiverBlockId]["objName"].toString());
		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseSlotName(const QUuid &blockId, const QString &code)
{
	QRegExp regex("E_SLOTNAME\\(([0-9])\\)_E");

	return parseRegexOn(regex, code, [this, &regex, &blockId](const QString &code) {
		Connection connection = _outConnections[blockId][regex.cap(1).toInt()].front();

		auto classTypeJson = _blocks[connection.receiverBlockId]["classType"].toObject();
		if (static_cast<types::ClassType::Type>(classTypeJson["type"].toInt()) != types::ClassType::SLOT)
			throwMessage("Signals must be connected with a slot");

		QString newCode = code;
		types::Slot slot;
		slot.deserialize(classTypeJson);
		newCode.replace(regex.pos(0), regex.matchedLength(), slot.name());
		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseVar(const QUuid &blockId, const QString &code)
{
	QRegExp regex("E_VAR\\((\\w*)\\)_E");

	return parseRegexOn(regex, code, [this, &regex, &blockId](const QString &code) {
		QString newCode = code;
		QString varName = regex.cap(1);

		if (varName.isEmpty()) {
			_varNames[blockId] = createVarName();
			newCode.replace(regex.pos(0), regex.matchedLength(), _varNames[blockId]);
		} else {
			_varNames[blockId] = varName;
			newCode.replace(regex.pos(0), regex.matchedLength(), "");
		}
		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseUseVar(const QUuid &blockId, const QString &code)
{
	QRegExp regex("E_USEVAR\\(([0-9])\\)_E");

	return parseRegexOn(regex, code, [this, &regex, &blockId](const QString &code) {
		QString newCode = code;
		int portIdx = regex.cap(1).toInt();
		Connection connection = _inConnections[blockId][portIdx];

		if (connection.isNull)
			newCode.replace(regex.pos(0), regex.matchedLength(), "");
		else if (_varNames.contains(connection.receiverBlockId))
			newCode.replace(regex.pos(0), regex.matchedLength(), _varNames[connection.receiverBlockId]);
		else
			newCode.replace(regex.pos(0), regex.matchedLength(), parseBlock(connection.receiverBlockId));
		return newCode;
	});
}

QString libraryObjects::ClassTypeExporter::parseCode(const QUuid &blockId, const QString &code)
{
	QRegExp regex("E_CODE\\(([0-9])\\)_E");
	bool skipCode = _skipCode;

	_skipCode = false;
	return parseRegexOn(regex, code, [this, &regex, &blockId, skipCode](const QString &code) {
		Connection connection = _outConnections[blockId][regex.cap(1).toInt()].front();
		QString newCode = parseBlock(connection.receiverBlockId);

		return skipCode ? newCode : QString(code).replace(regex.pos(0), regex.matchedLength(), newCode);
	});
}

QString libraryObjects::ClassTypeExporter::parseSkipCode(const QUuid &blockId, const QString &code)
{
	QRegExp regex("E_SKIPCODE\\(([0-9])\\)_E");
	bool skipCode = _skipCode;

	_skipCode = false;
	return parseRegexOn(regex, code, [this, &regex, &blockId, skipCode](const QString &code) {
		Connection connection = _outConnections[blockId][regex.cap(1).toInt()].front();

		_skipCode = true;
		QString newCode = parseBlock(connection.receiverBlockId);

		return skipCode ? newCode : QString(code).replace(regex.pos(0), regex.matchedLength(), newCode);
	});
}

QString libraryObjects::ClassTypeExporter::parseRegexOn(const QRegExp &regex, const QString &code, RegExpCallback callBack)
{
	QString newCode = code;
	int pos = 0;

	while ((pos = regex.indexIn(newCode)) != -1)
		newCode = callBack(newCode);
	return newCode;
}
