/*
** CODING JOURNEY
** workflow
** OperatorArythmetical
** LHUILE Léo
*/

#include "moc_OperatorArythmetical.cpp"

#include "OperatorArythmetical.hpp"
#include "Number.hpp"

codingJourney::workflow::dataModel::OperatorArythmetical::OperatorArythmetical()
	: _operators(new QComboBox)
{
	_inputs[0] = std::make_pair(false, std::make_shared<data::Number>(false));
	_inputs[1] = std::make_pair(false, std::make_shared<data::Number>(false));

	_operators->addItems({"+", "-", "*", "/", "%"});
	connect(_operators, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int) {
		emit dataUpdated(0);
	});
}

QJsonObject codingJourney::workflow::dataModel::OperatorArythmetical::save() const
{
	QJsonObject modelJson = NodeDataModel::save();

	modelJson["operator"] = _operators->currentText();
	return modelJson;
}

void codingJourney::workflow::dataModel::OperatorArythmetical::restore(QJsonObject const &json)
{
	QJsonValue jsonValue = json["operator"];

	if (jsonValue != QJsonValue::Undefined)
		_operators->setCurrentText(jsonValue.toString("+"));
}

unsigned int codingJourney::workflow::dataModel::OperatorArythmetical::nPorts(PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		ret = 2;
		break;
	case PortType::Out:
		ret = 1;
		break;
	}
	return ret;
}

NodeDataType codingJourney::workflow::dataModel::OperatorArythmetical::dataType(PortType, PortIndex) const
{
	return data::Number().type();
}

void codingJourney::workflow::dataModel::OperatorArythmetical::setInData(std::shared_ptr<NodeData> data, int inputNbr)
{
	auto numberData = std::dynamic_pointer_cast<data::Number>(data);

	_inputs[inputNbr] = numberData ? std::make_pair(true, numberData) : std::make_pair(false, std::make_shared<data::Number>(false));
	if (_inputs[0].first && _inputs[1].first) {
		_modelValidationState = NodeValidationState::Valid;
		_modelValidationError = "";
	} else {
		_modelValidationState = NodeValidationState::Warning;
		_modelValidationError = "Missing inputs";
	}
	emit dataUpdated(0);
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::OperatorArythmetical::outData(PortIndex)
{
	int ret = 0;

	if (_operators->currentText() == "+")
		ret = _inputs[0].second->value() + _inputs[1].second->value();
	else if (_operators->currentText() == "-")
		ret = _inputs[0].second->value() - _inputs[1].second->value();
	else if (_operators->currentText() == "*")
		ret = _inputs[0].second->value() * _inputs[1].second->value();
	else if (_operators->currentText() == "/")
		ret = _inputs[0].second->value() / _inputs[1].second->value();
	else if (_operators->currentText() == "%")
		ret = _inputs[0].second->value() % _inputs[1].second->value();
	return std::make_shared<data::Number>(ret);
}
