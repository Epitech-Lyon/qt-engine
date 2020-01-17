#include "moc_NodeDataModel.cpp"

#include "NodeDataModel.hpp"

#include "StyleCollection.hpp"

using QtNodes::NodeDataModel;
using QtNodes::NodeStyle;

NodeDataModel::NodeDataModel()
	: _nodeStyle(StyleCollection::nodeStyle())
{
}

QJsonObject NodeDataModel::save() const
{
	QJsonObject modelJson;

	modelJson["name"] = name();
	return modelJson;
}

NodeStyle const &NodeDataModel::nodeStyle() const
{
	return _nodeStyle;
}

void NodeDataModel::setNodeStyle(NodeStyle const& style)
{
	_nodeStyle = style;
}
