/*
** CODING JOURNEY
** workflow
** NumberDisplay
** LHUILE Léo
*/

#include "NumberDisplay.hpp"
#include "Number.hpp"

codingJourney::workflow::dataModel::NumberDisplay::NumberDisplay()
	: _label(new QLabel())
{
	_label->setMargin(3);
}

unsigned int codingJourney::workflow::dataModel::NumberDisplay::nPorts(PortType portType) const
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

NodeDataType codingJourney::workflow::dataModel::NumberDisplay::dataType(PortType, PortIndex) const
{
	return data::Number().type();
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::NumberDisplay::outData(PortIndex)
{
	return std::shared_ptr<NodeData>();
}

void codingJourney::workflow::dataModel::NumberDisplay::setInData(std::shared_ptr<NodeData> data, int)
{
	auto numberData = std::dynamic_pointer_cast<data::Number>(data);

	if (numberData) {
		_modelValidationState = NodeValidationState::Valid;
		_modelValidationError = "";
		_label->setText(QString::number(numberData->value()));
	} else {
		_modelValidationState = NodeValidationState::Warning;
		_modelValidationError = "Missing inputs";
		_label->clear();
	}
	_label->adjustSize();
}

NodeValidationState codingJourney::workflow::dataModel::NumberDisplay::validationState() const
{
	return _modelValidationState;
}

QString codingJourney::workflow::dataModel::NumberDisplay::validationMessage() const
{
	return _modelValidationError;
}
