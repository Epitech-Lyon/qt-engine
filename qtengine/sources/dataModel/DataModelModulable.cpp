/*
** CODING JOURNEY
** workflow
** Modulable
** LHUILE Léo
*/

#include "moc_DataModelModulable.cpp"
#include "DataModelModulable.hpp"

#include "DataModulable.hpp"

codingJourney::workflow::dataModel::Modulable::Modulable()
{
}

unsigned int codingJourney::workflow::dataModel::Modulable::nPorts(PortType portType) const
{
	int ret = 0;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
        ret = _inputs.size();
		break;
	case PortType::Out:
		ret = _outputs.size();
		break;
	}
	return ret;
}

NodeDataType codingJourney::workflow::dataModel::Modulable::dataType(PortType portType, PortIndex portIndex) const
{
	NodeDataType ret;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
        ret = data::Modulable::getDataTypeOf(_inputs[portIndex].second);
		break;
	case PortType::Out:
        ret = data::Modulable::getDataTypeOf(_outputs[portIndex].second);
		break;
	}
	return ret;
}

QString codingJourney::workflow::dataModel::Modulable::portCaption(PortType portType, PortIndex portIndex) const
{
	QString ret;

	switch (portType) {
	case PortType::None:
		break;
	case PortType::In:
        ret = _inputs[portIndex].first;
		break;
	case PortType::Out:
		ret = _outputs[portIndex].first;
		break;
	}
	return ret;
}

std::shared_ptr<NodeData> codingJourney::workflow::dataModel::Modulable::outData(PortIndex)
{
	return std::shared_ptr<NodeData>();
}

void codingJourney::workflow::dataModel::Modulable::setInData(std::shared_ptr<NodeData>, int)
{
//	auto booleanData = std::dynamic_pointer_cast<data::Boolean>(data);
//
//	if (booleanData) {
//		_modelValidationState = NodeValidationState::Valid;
//		_modelValidationError = "";
//	} else {
//		_modelValidationState = NodeValidationState::Warning;
//		_modelValidationError = "Missing inputs";
//	}
}

NodeValidationState codingJourney::workflow::dataModel::Modulable::validationState() const
{
	return _modelValidationState;
}

QString codingJourney::workflow::dataModel::Modulable::validationMessage() const
{
	return _modelValidationError;
}
