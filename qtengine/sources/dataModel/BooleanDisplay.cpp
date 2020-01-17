/*
** CODING JOURNEY
** workflow
** BooleanDisplay
** LHUILE Léo
*/

#include "BooleanDisplay.hpp"
#include "Boolean.hpp"

codingJourney::workflow::dataModel::BooleanDisplay::BooleanDisplay()
	: _label(new QLabel())
{
	_label->setMargin(3);
}

unsigned int codingJourney::workflow::dataModel::BooleanDisplay::nPorts(PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		ret = 1;
		break;
	case PortType::Out:
		break;
	}
	return ret;
}

NodeDataType codingJourney::workflow::dataModel::BooleanDisplay::dataType(PortType, PortIndex) const
{
	return data::Boolean().type();
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::BooleanDisplay::outData(PortIndex)
{
	return std::shared_ptr<NodeData>();
}

void codingJourney::workflow::dataModel::BooleanDisplay::setInData(std::shared_ptr<NodeData> data, int)
{
	auto booleanData = std::dynamic_pointer_cast<data::Boolean>(data);

	if (booleanData) {
		_modelValidationState = NodeValidationState::Valid;
		_modelValidationError = "";
		_label->setText(booleanData->value() ? "True" : "False");
	} else {
		_modelValidationState = NodeValidationState::Warning;
		_modelValidationError = "Missing inputs";
		_label->clear();
	}
	_label->adjustSize();
}

NodeValidationState codingJourney::workflow::dataModel::BooleanDisplay::validationState() const
{
	return _modelValidationState;
}

QString codingJourney::workflow::dataModel::BooleanDisplay::validationMessage() const
{
	return _modelValidationError;
}
