/*
** CODING JOURNEY
** workflow
** BooleanSource
** LHUILE Léo
*/

#include "moc_BooleanSource.cpp"

#include "BooleanSource.hpp"
#include "Boolean.hpp"

codingJourney::workflow::dataModel::BooleanSource::BooleanSource()
	: _button(new QPushButton)
	, _boolean(std::make_shared<data::Boolean>())
{
	_button->setText(_boolean->value() ? "True" : "False");
	connect(_button, &QPushButton::clicked, [this]() {
		_boolean = std::make_shared<data::Boolean>(!_boolean->value());
		_button->setText(_boolean->value() ? "True" : "False");
		emit dataUpdated(0);
	});
}

QJsonObject codingJourney::workflow::dataModel::BooleanSource::save() const
{
	QJsonObject modelJson = NodeDataModel::save();

	modelJson["boolean"] = _boolean->value();
	return modelJson;
}

void codingJourney::workflow::dataModel::BooleanSource::restore(QJsonObject const &json)
{
	QJsonValue jsonValue = json["boolean"];

	if (jsonValue != QJsonValue::Undefined) {
		auto value = jsonValue.toBool(0);

		_boolean = std::make_shared<data::Boolean>(value);
		_button->setText(_boolean->value() ? "True" : "False");
	}
}

unsigned int codingJourney::workflow::dataModel::BooleanSource::nPorts(PortType portType) const
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

NodeDataType codingJourney::workflow::dataModel::BooleanSource::dataType(PortType, PortIndex) const
{
	return data::Boolean().type();
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::BooleanSource::outData(PortIndex)
{
	return _boolean;
}
