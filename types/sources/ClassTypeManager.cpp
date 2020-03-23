/*
** EPITECH PROJECT, 2020
** qt-engine
** File description:
** ClassTypeManager
*/

#include "ClassTypeManager.hpp"

types::ClassTypeManager::ClassTypeManager()
{
	///// REGISTER TYPES /////
	registerType(QMetaType::Void);
	registerType(QMetaType::Bool);
	registerType(QMetaType::Int);
	registerType(QMetaType::Double);
	registerType(QMetaType::QString);
	registerType(QMetaType::QDate);
	registerType(QMetaType::QTime);
	registerType(QMetaType::QDateTime);
	registerType(QMetaType::QKeySequence);
	registerType(QMetaType::QChar);
	registerType(QMetaType::QLocale);
	registerType(QMetaType::QPoint);
	registerType(QMetaType::QPointF);
	registerType(QMetaType::QSize);
	registerType(QMetaType::QSizeF);
	registerType(QMetaType::QRect);
	registerType(QMetaType::QRectF);
	registerType(QMetaType::QColor);
	registerType(QMetaType::QSizePolicy);
	registerType(QMetaType::QFont);
	registerType(QMetaType::QCursor);

	///// REGISTER INT FUNCTIONS /////
	registerFunctionOperatorConditionalForType(type(QMetaType::Int), "<");
	registerFunctionOperatorConditionalForType(type(QMetaType::Int), "<=");
	registerFunctionOperatorConditionalForType(type(QMetaType::Int), ">");
	registerFunctionOperatorConditionalForType(type(QMetaType::Int), ">=");
	registerFunctionOperatorForType(type(QMetaType::Int), '+');
	registerFunctionOperatorForType(type(QMetaType::Int), '-');
	registerFunctionOperatorForType(type(QMetaType::Int), '*');
	registerFunctionOperatorForType(type(QMetaType::Int), '/');

	///// REGISTER DOUBLE FUNCTIONS /////
	registerFunctionOperatorConditionalForType(type(QMetaType::Double), "<");
	registerFunctionOperatorConditionalForType(type(QMetaType::Double), "<=");
	registerFunctionOperatorConditionalForType(type(QMetaType::Double), ">");
	registerFunctionOperatorConditionalForType(type(QMetaType::Double), ">=");
	registerFunctionOperatorForType(type(QMetaType::Double), '+');
	registerFunctionOperatorForType(type(QMetaType::Double), '-');
	registerFunctionOperatorForType(type(QMetaType::Double), '*');
	registerFunctionOperatorForType(type(QMetaType::Double), '/');

	///// REGISTER BOOL FUNCTIONS /////
	registerFunctionForType(type(QMetaType::Bool), {
		{type(QMetaType::Bool)},
		type(QMetaType::Bool),
		"operator!",
		type(QMetaType::Bool) + " E_VAR()_E = !E_USEVAR(1)_E;\nE_CODE(0)_E"
	});

	///// REGISTER QSTRING FUNCTIONS /////
	registerFunctionForType(type(QMetaType::QString), {
		{type(QMetaType::QString)},
		type(QMetaType::Bool),
		"isEmpty",
		type(QMetaType::Bool) + " E_VAR()_E = E_USEVAR(1)_E.isEmpty();\nE_CODE(0)_E"
	});
	registerFunctionForType(type(QMetaType::QString), {
		{type(QMetaType::QString), type(QMetaType::QString)},
		type(QMetaType::Bool),
		"contains",
		type(QMetaType::Bool) + " E_VAR()_E = E_USEVAR(1)_E.contains(E_USEVAR(2)_E);\nE_CODE(0)_E"
	});
	registerFunctionForType(type(QMetaType::QString), {
		{type(QMetaType::QString)},
		type(QMetaType::Int),
		"size",
		type(QMetaType::Int) + " E_VAR()_E = E_USEVAR(1)_E.size();\nE_CODE(0)_E"
	});
	registerFunctionForType(type(QMetaType::QString), {
		{type(QMetaType::QString), type(QMetaType::QString)},
		type(QMetaType::Int),
		"indexOf",
		type(QMetaType::Int) + " E_VAR()_E = E_USEVAR(1)_E.indexOf(E_USEVAR(2)_E);\nE_CODE(0)_E"
	});
}

void types::ClassTypeManager::registerFunctionOperatorForType(const QString &type, const QChar &characOperator)
{
	registerFunctionForType(type, {
		{type, type},
		type,
		QString("operator") + characOperator,
		type + " E_VAR()_E = E_USEVAR(1)_E " + characOperator + " E_USEVAR(2)_E;\nE_CODE(0)_E"
	});
}

void types::ClassTypeManager::registerFunctionOperatorConditionalForType(const QString &type, const QString &operatorConditional)
{
	if (!types().contains(this->type(QMetaType::Bool))) { return; }

	registerFunctionForType(type, {
		{type, type},
		this->type(QMetaType::Bool),
		"operator" + operatorConditional,
		this->type(QMetaType::Bool) + " E_USEVAR(1)_E " + operatorConditional + " E_USEVAR(2)_E;\nE_CODE(0)_E"
	});
}

types::ClassTypeManager *types::ClassTypeManager::instance()
{
	static ClassTypeManager classTypeManager;

	return &classTypeManager;
}

QString types::ClassTypeManager::type(QMetaType::Type type) const
{
	return type <= QMetaType::UnknownType ? "" : _types.key(type);
}

void types::ClassTypeManager::registerType(QMetaType::Type type)
{
	if (type <= QMetaType::UnknownType) { return; }

	QString typeName = QMetaType::typeName(type);

	_types[typeName] = type;
	registerFunctionOperatorConditionalForType(typeName, "==");
	registerFunctionOperatorConditionalForType(typeName, "!=");
}

void types::ClassTypeManager::registerType(const QString &type)
{
	_types[type] = QMetaType::UnknownType;
	registerFunctionOperatorConditionalForType(type, "==");
	registerFunctionOperatorConditionalForType(type, "!=");
}

void types::ClassTypeManager::registerFunctionForType(const QString &type, const Function &function)
{
	_functions[type].append(function);
}

void types::ClassTypeManager::unregisterType(const QString &type)
{
	_types.remove(type);
	_functions.remove(type);
}
