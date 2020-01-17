/*
** CODING JOURNEY
** workflow
** OperatorAND
** LHUILE Léo
*/

#include "OperatorAND.hpp"
#include "Boolean.hpp"

codingJourney::workflow::dataModel::OperatorAND::OperatorAND()
	: _label(new QLabel("AND"))
{
	_inputs[0] = std::make_pair(false, std::make_shared<data::Boolean>(false));
	_inputs[1] = std::make_pair(false, std::make_shared<data::Boolean>(false));

	_label->setAttribute(Qt::WA_TranslucentBackground);
	_label->setStyleSheet("QLabel { color : white; font: 18pt; }");
}

unsigned int codingJourney::workflow::dataModel::OperatorAND::nPorts(PortType portType) const
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

NodeDataType codingJourney::workflow::dataModel::OperatorAND::dataType(PortType, PortIndex) const
{
	return data::Boolean().type();
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::OperatorAND::outData(PortIndex)
{
	return std::make_shared<data::Boolean>(_inputs[0].second->value() && _inputs[1].second->value());
}

void codingJourney::workflow::dataModel::OperatorAND::setInData(std::shared_ptr<NodeData> data, int inputNbr)
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
