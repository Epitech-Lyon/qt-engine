/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ContentPanelWorkflow
*/

#include "moc_ContentPanelWorkflow.cpp"
#include "ContentPanelWorkflow.hpp"

#include "types/includes/Method.hpp"
#include "DialogMethodSettings.hpp"
#include "types/includes/Property.hpp"
#include "DialogPropertySettings.hpp"

#include "FlowView.hpp"
#include "FlowScene.hpp"
#include "Method.hpp"
#include "Property.hpp"

#include "Manager.hpp"
#include "ViewManager.hpp"
#include "AObject.hpp"
#include "MainWindow.hpp"

#include <QtWidgets/QSplitter>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QGraphicsSceneDragDropEvent>
#include <QtCore/QDebug>

qtengine::ListWidget::ListWidget(QWidget *parent)
	: QListWidget(parent)
{
	viewport()->setAutoFillBackground(false);
	setFrameShape(QFrame::NoFrame);
}

qtengine::FlowScene::FlowScene(QObject *parent)
	: QtNodes::FlowScene(parent)
{
}

void qtengine::FlowScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	QtNodes::FlowScene::dragEnterEvent(event);
}

#include "LibraryObjectMimeData.hpp"
void qtengine::FlowScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
//	QtNodes::FlowScene::dragMoveEvent(event);
	qDebug() << event->mimeData() << dynamic_cast<const libraryObjects::LibraryObjectMimeData*>(event->mimeData());
	event->accept();
}

void qtengine::FlowScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	QtNodes::FlowScene::dropEvent(event);
}

qtengine::ContentPanelWorkflow::ContentPanelWorkflow(QWidget *parent)
	: ContentPanelBase("Workflow", parent)
{
}

void qtengine::ContentPanelWorkflow::init()
{
	auto splitter = new QSplitter(this);
	_mainLayout->addWidget(splitter);

	auto menuWidget = new QWidget(splitter);
	auto menuLayout = new QVBoxLayout(menuWidget);
	menuLayout->setMargin(0);
	menuLayout->setSpacing(16);
	menuWidget->setLayout(menuLayout);

	auto createMenuFor = [menuLayout](const QString &lblText, std::function<void (void)> onAdd) {
		auto widget = new QWidget(menuLayout->parentWidget());
		widget->setLayout(new QVBoxLayout(widget));
		widget->layout()->setMargin(0);
		menuLayout->addWidget(widget);

		auto btnAdd = new QPushButton("+");
		btnAdd->setFixedSize(btnAdd->minimumSizeHint());
		connect(btnAdd, &QPushButton::clicked, onAdd);

		auto toolbar = new QWidget(widget);
		auto toolbarLayout = new QHBoxLayout(toolbar);
		toolbar->setLayout(toolbarLayout);
		toolbarLayout->addWidget(new QLabel(lblText, widget));
		toolbarLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
		toolbarLayout->addWidget(btnAdd);
		widget->layout()->addWidget(toolbar);

		auto list = new ListWidget(widget);
		widget->layout()->addWidget(list);
		return list;
	};

	_listMethod = createMenuFor("Method", std::bind(&ContentPanelWorkflow::onAddMethod, this));
	_listSignal = createMenuFor("Signal", std::bind(&ContentPanelWorkflow::onAddSignal, this));
	_listSlot = createMenuFor("Slot", std::bind(&ContentPanelWorkflow::onAddSlot, this));
	_listProperty = createMenuFor("Property", std::bind(&ContentPanelWorkflow::onAddProperty, this));

	_scene = new FlowScene(this);
	_view = new QtNodes::FlowView(_scene, splitter);

	splitter->addWidget(menuWidget);
	splitter->addWidget(_view);
	splitter->setSizes({menuWidget->sizeHint().width(), menuWidget->sizeHint().width() * 4});

	onViewObjectChanged(Manager::instance()->viewManager()->viewObject());
	connect(Manager::instance()->viewManager(), &ViewManager::viewObjectChanged, this, &ContentPanelWorkflow::onViewObjectChanged);
}

std::shared_ptr<QtNodes::DataModelRegistry> qtengine::ContentPanelWorkflow::generateRegistry(const QMetaObject *metaObject, QMetaMethod::Access minimumAccess)
{
	auto registry = std::make_shared<QtNodes::DataModelRegistry>();
	QMap<QMetaMethod::MethodType, QString> typeToString = {
		{ QMetaMethod::Method, "Method" },
		{ QMetaMethod::Signal, "Signal" },
		{ QMetaMethod::Slot, "Slot" }};

	for (int idx = 0; idx < metaObject->methodCount(); idx += 1) {
		auto metaMethod = metaObject->method(idx);

		if (metaMethod.access() >= minimumAccess && typeToString.contains(metaMethod.methodType()))
			registry->registerModel<Method>(typeToString[metaMethod.methodType()], [=]() { return std::unique_ptr<Method>(new Method(metaMethod)); });
	}
	for (int idx = 0; idx < metaObject->propertyCount(); idx += 1) {
		auto metaProperty = metaObject->property(idx);

		registry->registerModel<Property>("Property", [=]() { return std::unique_ptr<Property>(new Property(metaProperty)); });
	}
	return registry;
}

void qtengine::ContentPanelWorkflow::onViewObjectChanged(libraryObjects::AObject *viewObject)
{
	_listMethod->clear();
	_listSignal->clear();
	_listSlot->clear();
	_listProperty->clear();
	_scene->clearScene();
	if (!viewObject) { return; }
	_viewRegistry = generateRegistry(viewObject->object()->metaObject(), QMetaMethod::Protected);

	_scene->setRegistry(_viewRegistry);
}

void qtengine::ContentPanelWorkflow::onAddMethod()
{
	DialogMethodSettings dialogMethodSettings;

	if (dialogMethodSettings.exec() == QDialog::Accepted) {
		qDebug() << dialogMethodSettings.method();
	}
}

void qtengine::ContentPanelWorkflow::onAddSignal()
{
	DialogMethodSettings dialogMethodSettings;

	if (dialogMethodSettings.exec() == QDialog::Accepted) {
		qDebug() << dialogMethodSettings.method();
	}
}

void qtengine::ContentPanelWorkflow::onAddSlot()
{
	DialogMethodSettings dialogMethodSettings;

	if (dialogMethodSettings.exec() == QDialog::Accepted) {
		qDebug() << dialogMethodSettings.method();
	}
}

void qtengine::ContentPanelWorkflow::onAddProperty()
{
	DialogPropertySettings dialogPropertySettings;

	if (dialogPropertySettings.exec() == QDialog::Accepted) {
		qDebug() << dialogPropertySettings.property();
	}
}
