/*
** CODING JOURNEY
** workflow
** OperatorConditional
** LHUILE Léo
*/

#include "moc_OperatorConditional.cpp"

#include "OperatorConditional.hpp"
#include "Boolean.hpp"
#include "FlowController.hpp"

codingJourney::workflow::dataModel::OperatorConditional::OperatorConditional()
	: _inputsEdit(new QSpinBox)
{
	_inputs.resize(1);
	_inputsEdit->setMinimum(1);
	_inputsEdit->setMaximumSize(_inputsEdit->sizeHint());

	connect(_inputsEdit, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
		_inputs.resize(value);
	});
}

unsigned int codingJourney::workflow::dataModel::OperatorConditional::nPorts(PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		ret = _inputs.size();
		break;
	case PortType::Out:
		ret = _inputs.size() + 1;
		break;
	}
	return ret;
}

QString codingJourney::workflow::dataModel::OperatorConditional::portCaption(PortType portType, PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		ret = portIndex == 0 ? "If" : "Else if";
		break;
	case PortType::Out:
		ret = portIndex < static_cast<int>(_inputs.size()) ? "Then" : "Else";
		break;
	}
	return ret;
}

NodeDataType codingJourney::workflow::dataModel::OperatorConditional::dataType(PortType portType, PortIndex) const
{
	NodeDataType ret;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
		data::Boolean().type();
		break;
	case PortType::Out:
		data::FlowController().type();
		break;
	}
	return ret;
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::OperatorConditional::outData(PortIndex)
{
	return std::make_shared<data::FlowController>();
}

void codingJourney::workflow::dataModel::OperatorConditional::setInData(std::shared_ptr<NodeData> data, int inputNbr)
{
	auto booleanData = std::dynamic_pointer_cast<data::Boolean>(data);
	bool allInputAreSet = true;

	_inputs[inputNbr] = booleanData ? true : false;
	for (auto input : _inputs)
		allInputAreSet = allInputAreSet && input;
	if (allInputAreSet) {
		_modelValidationState = NodeValidationState::Valid;
		_modelValidationError = "";
	} else {
		_modelValidationState = NodeValidationState::Warning;
		_modelValidationError = "Missing inputs";
	}
	emit dataUpdated(inputNbr);
}

NodeValidationState codingJourney::workflow::dataModel::OperatorConditional::validationState() const
{
	return _modelValidationState;
}

QString codingJourney::workflow::dataModel::OperatorConditional::validationMessage() const
{
	return _modelValidationError;
}
