/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeExporter
*/

#include "ClassTypeExporter.hpp"

#include "ClassType.hpp"

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

	qCritical() << ret;
	throw ret.toLocal8Bit().data();
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

		_outConnections[outBlockId][outIdx] = Connection(inBlockId, inIdx);
		_inConnections[inBlockId][inIdx] = Connection(outBlockId, outIdx);
	}

	auto startBlock = findBlock("START");
	if (startBlock.isNull())
		throwMessage("Cannot find START");

	auto returnBlock = findBlock("RETURN");
	if (returnBlock.isNull())
		throwMessage("Must have at least one RETURN");

	for (auto &inConnections : _inConnections)
		for (auto &inConnection : inConnections)
			if (inConnection.isNull)
				throwMessage("All input must be filled");
	for (auto &outConnections : _outConnections)
		if (outConnections.size() > 0 && outConnections[0].isNull)
			throwMessage("All FlowController's output must be filled");

	parseBlock(_outConnections[startBlock][0].receiverBlockId);
}

void libraryObjects::ClassTypeExporter::parseBlock(const QUuid &blockId)
{
	qDebug() << "Parse block" << _blocks[blockId]["name"].toString() << blockId;
	if (_outConnections[blockId].size() > 0)
		parseBlock(_outConnections[blockId][0].receiverBlockId);
}

QUuid libraryObjects::ClassTypeExporter::findBlock(const QString &blockName) const
{
	for (auto &block : _blocks)
		if (block["name"].toString() == blockName)
			return _blocks.key(block);
	return QUuid();
}
