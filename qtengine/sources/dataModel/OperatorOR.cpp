/*
** CODING JOURNEY
** workflow
** OperatorOR
** LHUILE Léo
*/

#include "OperatorOR.hpp"
#include "Boolean.hpp"

codingJourney::workflow::dataModel::OperatorOR::OperatorOR()
	: _label(new QLabel("OR"))
{
	_inputs[0] = std::make_pair(false, std::make_shared<data::Boolean>(false));
	_inputs[1] = std::make_pair(false, std::make_shared<data::Boolean>(false));

	_label->setAttribute(Qt::WA_TranslucentBackground);
	_label->setStyleSheet("QLabel { color : white; font: 18pt; }");
}

unsigned int codingJourney::workflow::dataModel::OperatorOR::nPorts(PortType portType) const
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

NodeDataType codingJourney::workflow::dataModel::OperatorOR::dataType(PortType, PortIndex) const
{
	return data::Boolean().type();
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::OperatorOR::outData(PortIndex)
{
	return std::make_shared<data::Boolean>(_inputs[0].second->value() || _inputs[1].second->value());
}

void codingJourney::workflow::dataModel::OperatorOR::setInData(std::shared_ptr<NodeData> data, int inputNbr)
{
	auto booleanData = std::dynamic_pointer_cast<data::Boolean>(data);

	_inputs[inputNbr] = booleanData ? std::make_pair(true, booleanData) : std::make_pair(false, std::make_shared<data::Boolean>(false));
	if (_inputs[0].first && _inputs[1].first) {
		_modelValidationState = NodeValidationState::Valid;
		_modelValidationError = "";
	} else {
		_modelValidationState = NodeValidationState::Warning;
		_modelValidationError = "Missing inputs";
	}
	emit dataUpdated(0);
}

NodeValidationState codingJourney::workflow::dataModel::OperatorOR::validationState() const
{
	return _modelValidationState;
}

QString codingJourney::workflow::dataModel::OperatorOR::validationMessage() const
{
	return _modelValidationError;
}
