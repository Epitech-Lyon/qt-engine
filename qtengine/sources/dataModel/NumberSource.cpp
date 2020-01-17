/*
** CODING JOURNEY
** workflow
** NumberSource
** LHUILE Léo
*/

#include "moc_NumberSource.cpp"
#include "NumberSource.hpp"
#include "Number.hpp"

#include <QtCore/QJsonValue>

codingJourney::workflow::dataModel::NumberSource::NumberSource()
	: _numberEdit(new QSpinBox)
	, _number(std::make_shared<data::Number>())
{
	_numberEdit->setMaximumSize(_numberEdit->sizeHint());

	connect(_numberEdit, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
		_number = std::make_shared<data::Number>(value);
		emit dataUpdated(0);
	});
}

QJsonObject codingJourney::workflow::dataModel::NumberSource::save() const
{
	QJsonObject modelJson = NodeDataModel::save();

	modelJson["number"] = _number->value();
	return modelJson;
}

void codingJourney::workflow::dataModel::NumberSource::restore(QJsonObject const &json)
{
	QJsonValue jsonValue = json["number"];

	if (jsonValue != QJsonValue::Undefined) {
		auto value = jsonValue.toInt(0);

		_number = std::make_shared<data::Number>(value);
		_numberEdit->setValue(value);
	}
}

unsigned int codingJourney::workflow::dataModel::NumberSource::nPorts(PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		break;
	case PortType::Out:
		ret = 1;
		break;
	}
	return ret;
}

NodeDataType codingJourney::workflow::dataModel::NumberSource::dataType(PortType, PortIndex) const
{
	return data::Number().type();
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::NumberSource::outData(PortIndex)
{
	return _number;
}
