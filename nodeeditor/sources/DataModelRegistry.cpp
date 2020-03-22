#include "DataModelRegistry.hpp"
#include <algorithm>

using QtNodes::DataModelRegistry;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::TypeConverter;

void DataModelRegistry::unregisterModel(const QString &modelName)
{
	auto it = _registeredModelsCategory.find(modelName);

	if (it != _registeredModelsCategory.end()) {
		_registeredItemCreators.erase(it->first);
		_registeredModelsCategory.erase(it->first);
	}
}

void DataModelRegistry::addPrefix(const QString &prefix)
{
	RegisteredModelCreatorsMap registeredItemCreators;
	RegisteredModelsCreatorsMap registeredModelsCategory;

	for (auto &tmp : _registeredItemCreators)
		registeredItemCreators[prefix + tmp.first] = std::move(tmp.second);
	_registeredItemCreators = registeredItemCreators;
	for (auto &tmp : _registeredModelsCategory)
		registeredModelsCategory[prefix + tmp.first] = { prefix + tmp.second.first, tmp.second.second};
	_registeredModelsCategory = registeredModelsCategory;
}

void DataModelRegistry::concatenate(DataModelRegistry *registry)
{
	for (auto tmp : registry->_registeredItemCreators)
		_registeredItemCreators[tmp.first] = tmp.second;
	for (auto tmp : registry->_registeredModelsCategory)
		_registeredModelsCategory[tmp.first] = tmp.second;
	for (auto tmp : registry->_registeredTypeConverters)
		_registeredTypeConverters[tmp.first] = tmp.second;
}

std::unique_ptr<NodeDataModel> DataModelRegistry::create(QString const &modelName)
{
	auto itCreator = _registeredItemCreators.find(modelName);

	if (itCreator != _registeredItemCreators.end())
		return itCreator->second();
	return nullptr;
}

TypeConverter DataModelRegistry::getTypeConverter(NodeDataType const &d1, NodeDataType const &d2) const
{
	TypeConverterId converterId = std::make_pair(d1, d2);
	auto it = _registeredTypeConverters.find(converterId);

	if (it != _registeredTypeConverters.end())
		return it->second;
	return TypeConverter{};
}
