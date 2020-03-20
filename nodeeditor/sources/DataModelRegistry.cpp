#include "DataModelRegistry.hpp"

using QtNodes::DataModelRegistry;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::TypeConverter;

void DataModelRegistry::unregisterModel(const QString &name)
{
	if (_registeredItemCreators.count(name) == 0) { return; }

	_registeredModelsCategory.erase(_registeredModelsCategory[name]);
	_registeredModelsCategory.erase(name);
	_registeredItemCreators.erase(name);
	for (auto category : _registeredModelsCategory)
		_categories.insert(category.second);
}

void DataModelRegistry::addPrefix(const QString &prefix)
{
	for (auto &tmp : _registeredModelsCategory) {
		_categories.erase(tmp.second);
		tmp.second = prefix + tmp.second;
		_categories.insert(tmp.second);
	}
}

void DataModelRegistry::concatenate(DataModelRegistry *registry)
{
	for (auto tmp : registry->_registeredModelsCategory)
		_registeredModelsCategory[tmp.first] = tmp.second;
	for (auto tmp : registry->_categories)
		_categories.insert(tmp);
	for (auto tmp : registry->_registeredItemCreators)
		_registeredItemCreators[tmp.first] = tmp.second;
	for (auto tmp : registry->_registeredTypeConverters)
		_registeredTypeConverters[tmp.first] = tmp.second;
}

std::unique_ptr<NodeDataModel> DataModelRegistry::create(QString const &modelName)
{
	auto it = _registeredItemCreators.find(modelName);

	if (it != _registeredItemCreators.end())
		return it->second();
	return nullptr;
}

DataModelRegistry::RegisteredModelCreatorsMap const &DataModelRegistry::registeredModelCreators() const
{
	return _registeredItemCreators;
}

DataModelRegistry::RegisteredModelsCategoryMap const &DataModelRegistry::registeredModelsCategoryAssociation() const
{
	return _registeredModelsCategory;
}

DataModelRegistry::CategoriesSet const &DataModelRegistry::categories() const
{
	return _categories;
}

TypeConverter DataModelRegistry::getTypeConverter(NodeDataType const &d1, NodeDataType const &d2) const
{
	TypeConverterId converterId = std::make_pair(d1, d2);
	auto it = _registeredTypeConverters.find(converterId);

	if (it != _registeredTypeConverters.end())
		return it->second;
	return TypeConverter{};
}
