/*
** EPITECH PROJECT, 2019
** qt-engine
** File description:
** NodeDataModel
*/

#pragma once

#include <QtWidgets/QWidget>

#include "PortType.hpp"
#include "NodeData.hpp"
#include "Serializable.hpp"
#include "NodeGeometry.hpp"
#include "NodeStyle.hpp"
#include "NodePainterDelegate.hpp"
#include "memory.hpp"

namespace QtNodes {
	enum class NodeValidationState
	{
		Valid,
		Warning,
		Error
	};

	class Connection;
	class StyleCollection;

	class NodeDataModel : public QObject, public Serializable {
		Q_OBJECT

	public:
		enum class ConnectionPolicy {
			One,
			Many,
		};

		NodeDataModel();
		virtual ~NodeDataModel() = default;

		virtual bool canBeRemoved() const { return true; }

		QJsonObject save() const override;

		/// Name makes this model unique
		virtual QString name() const = 0;

		/// Caption is used in GUI
		virtual QString caption() const = 0;

		/// It is possible to hide caption in GUI
		virtual bool captionVisible() const { return true; }

		/// Port caption is used in GUI to label individual ports
		virtual QString portCaption(PortType, PortIndex) const { return QString(); }

		/// It is possible to hide port caption in GUI
		virtual bool portCaptionVisible(PortType, PortIndex) const { return false; }

		/// Triggers the algorithm
		virtual void setInData(std::shared_ptr<NodeData>, PortIndex) {}
		virtual std::shared_ptr<NodeData> outData(PortIndex) { return std::shared_ptr<NodeData>(); }

		virtual unsigned int nPorts(PortType portType) const = 0;
		virtual std::shared_ptr<NodeData> data(PortType portType, PortIndex portIndex) const = 0;

		virtual ConnectionPolicy portOutConnectionPolicy(PortIndex) const { return ConnectionPolicy::One; }

		NodeStyle const &nodeStyle() const;
		void setNodeStyle(NodeStyle const &style);

		virtual QWidget *embeddedWidget() { return nullptr; }
		virtual bool resizable() const { return false; }

		void setValidationState(NodeValidationState validationState) { _validationState = validationState; emit validationStateUpdated(); }
		NodeValidationState validationState() const { return _validationState; }
		void setValidationMessage(const QString &validationMessage) { _validationMessage = validationMessage; emit validationMessageUpdated(); }
		QString validationMessage() const { return _validationMessage; }

		virtual NodePainterDelegate *painterDelegate() const { return nullptr; }

	public Q_SLOTS:
		virtual void inputConnectionCreated(Connection const &) {}
		virtual void inputConnectionDeleted(Connection const &) {}
		virtual void outputConnectionCreated(Connection const &) {}
		virtual void outputConnectionDeleted(Connection const &) {}

	Q_SIGNALS:
		void dataUpdated(PortIndex index);
		void dataInvalidated(PortIndex index);
		void computingStarted();
		void computingFinished();
		void embeddedWidgetChanged();
		void embeddedWidgetSizeUpdated();
		void validationStateUpdated();
		void validationMessageUpdated();
		void nodePortUpdated();

	private:
		NodeStyle _nodeStyle;
		NodeValidationState _validationState = NodeValidationState::Valid;
		QString _validationMessage;
	};
}
