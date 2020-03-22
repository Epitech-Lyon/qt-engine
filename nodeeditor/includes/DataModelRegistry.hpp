#pragma once

#include <memory>
#include <functional>
#include <map>
#include <vector>

#include <QtCore/QString>

#include "NodeDataModel.hpp"
#include "TypeConverter.hpp"
#include "memory.hpp"

namespace QtNodes {
	inline bool operator<(QtNodes::NodeDataType const & d1, QtNodes::NodeDataType const & d2) { return d1.id < d2.id; }

	/// Class uses map for storing models (name, model)
	class DataModelRegistry {
	public:
		using RegistryItemPtr = std::unique_ptr<NodeDataModel>;
		using RegistryItemCreator = std::function<RegistryItemPtr()>;
		using RegisteredModelCreatorsMap = std::map<QString, RegistryItemCreator>;
		using RegisteredModelsCreatorsMap = std::map<QString, std::pair<QString, QString>>;
		using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;

		DataModelRegistry() = default;
		~DataModelRegistry() = default;

		DataModelRegistry(DataModelRegistry const &) = delete;
		DataModelRegistry(DataModelRegistry &&) = default;

		DataModelRegistry &operator=(DataModelRegistry const &) = delete;
		DataModelRegistry &operator=(DataModelRegistry &&) = default;

	public:
		template<typename ModelType> void registerModel(RegistryItemCreator creator, QString const &category = "")
		{
			registerModelImpl<ModelType>(std::move(creator), category);
		}

		template<typename ModelType> void registerModel(QString const &category = "")
		{
			RegistryItemCreator creator = [](){ return std::unique_ptr<ModelType>(new ModelType); };
			registerModelImpl<ModelType>(std::move(creator), category);
		}

		template<typename ModelType> void registerModel(QString const &category, RegistryItemCreator creator)
		{
			registerModelImpl<ModelType>(std::move(creator), category);
		}

		void registerTypeConverter(TypeConverterId const &id, TypeConverter typeConverter)
		{
			_registeredTypeConverters[id] = std::move(typeConverter);
		}

		void unregisterModel(const QString &name);

		void addPrefix(const QString &prefix);

		void concatenate(DataModelRegistry *registry);

		std::unique_ptr<NodeDataModel> create(QString const &modelName);
		RegisteredModelsCreatorsMap const &registeredModelsCategoryAssociation() const { return _registeredModelsCategory; }

		TypeConverter getTypeConverter(NodeDataType const &d1, NodeDataType const &d2) const;

	private:
		RegisteredModelsCreatorsMap _registeredModelsCategory;
		RegisteredModelCreatorsMap _registeredItemCreators;
		RegisteredTypeConvertersMap _registeredTypeConverters;

	private:
		// If the registered ModelType class has the static member method
		//
		//      static QString Name();
		//
		// use it. Otherwise use the non-static method:
		//
		//       virtual QString name() const;
		template <typename T, typename = void> struct HasStaticMethodName : std::false_type {};
		template <typename T> struct HasStaticMethodName<T, typename std::enable_if<std::is_same<decltype(T::Name()), QString>::value>::type> : std::true_type {};
		template<typename ModelType> typename std::enable_if< HasStaticMethodName<ModelType>::value>::type registerModelImpl(RegistryItemCreator creator, QString const &category)
		{
			const QString name = ModelType::Name();
			const QString modelPath = category + name;

			if (_registeredItemCreators.count(modelPath) == 0) {
				_registeredItemCreators[modelPath] = std::move(creator);
				_registeredModelsCategory[modelPath] = { category, name};
			}
		}
		template<typename ModelType> typename std::enable_if< !HasStaticMethodName<ModelType>::value>::type registerModelImpl(RegistryItemCreator creator, QString const &category )
		{
			const QString name = creator()->name();
			const QString modelPath = category + name;

			if (_registeredItemCreators.count(modelPath) == 0) {
				_registeredItemCreators[modelPath] = std::move(creator);
				_registeredModelsCategory[modelPath] = { category, name};
			}
		}
	};
}
