#include "ContentPanelProperties.h"

#include <QtCore/QDate>

using namespace nvr;

ContentPanelProperties::ContentPanelProperties(QWidget *parent)
{
	_panelName = tr("Properties");

	propertyManager = new QtVariantPropertyManager();

	int i = 0;
	QtProperty *topItem = propertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), QString::number(i++) + QLatin1String(" Group Property"));

	QtVariantProperty *item = propertyManager->addProperty(QVariant::Bool, QString::number(i++) + QLatin1String(" Bool Property"));
	item->setValue(true);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Int, QString::number(i++) + QLatin1String(" Int Property"));
	item->setValue(20);
	item->setAttribute(QLatin1String("minimum"), 0);
	item->setAttribute(QLatin1String("maximum"), 100);
	item->setAttribute(QLatin1String("singleStep"), 10);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Int, QString::number(i++) + QLatin1String(" Int Property (ReadOnly)"));
	item->setValue(20);
	item->setAttribute(QLatin1String("minimum"), 0);
	item->setAttribute(QLatin1String("maximum"), 100);
	item->setAttribute(QLatin1String("singleStep"), 10);
	item->setAttribute(QLatin1String("readOnly"), true);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Double, QString::number(i++) + QLatin1String(" Double Property"));
	item->setValue(1.2345);
	item->setAttribute(QLatin1String("singleStep"), 0.1);
	item->setAttribute(QLatin1String("decimals"), 3);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Double, QString::number(i++) + QLatin1String(" Double Property (ReadOnly)"));
	item->setValue(1.23456);
	item->setAttribute(QLatin1String("singleStep"), 0.1);
	item->setAttribute(QLatin1String("decimals"), 5);
	item->setAttribute(QLatin1String("readOnly"), true);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::String, QString::number(i++) + QLatin1String(" String Property"));
	item->setValue("Value");
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::String, QString::number(i++) + QLatin1String(" String Property (Password)"));
	item->setAttribute(QLatin1String("echoMode"), QLineEdit::Password);
	item->setValue("Password");
	topItem->addSubProperty(item);

	// Readonly String Property
	item = propertyManager->addProperty(QVariant::String, QString::number(i++) + QLatin1String(" String Property (ReadOnly)"));
	item->setAttribute(QLatin1String("readOnly"), true);
	item->setValue("readonly text");
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Date, QString::number(i++) + QLatin1String(" Date Property"));
	item->setValue(QDate::currentDate().addDays(2));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Time, QString::number(i++) + QLatin1String(" Time Property"));
	item->setValue(QTime::currentTime());
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::DateTime, QString::number(i++) + QLatin1String(" DateTime Property"));
	item->setValue(QDateTime::currentDateTime());
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::KeySequence, QString::number(i++) + QLatin1String(" KeySequence Property"));
	item->setValue(QKeySequence(Qt::ControlModifier | Qt::Key_Q));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Char, QString::number(i++) + QLatin1String(" Char Property"));
	item->setValue(QChar(386));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Locale, QString::number(i++) + QLatin1String(" Locale Property"));
	item->setValue(QLocale(QLocale::Polish, QLocale::Poland));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Point, QString::number(i++) + QLatin1String(" Point Property"));
	item->setValue(QPoint(10, 10));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::PointF, QString::number(i++) + QLatin1String(" PointF Property"));
	item->setValue(QPointF(1.2345, -1.23451));
	item->setAttribute(QLatin1String("decimals"), 3);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Size, QString::number(i++) + QLatin1String(" Size Property"));
	item->setValue(QSize(20, 20));
	item->setAttribute(QLatin1String("minimum"), QSize(10, 10));
	item->setAttribute(QLatin1String("maximum"), QSize(30, 30));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::SizeF, QString::number(i++) + QLatin1String(" SizeF Property"));
	item->setValue(QSizeF(1.2345, 1.2345));
	item->setAttribute(QLatin1String("decimals"), 3);
	item->setAttribute(QLatin1String("minimum"), QSizeF(0.12, 0.34));
	item->setAttribute(QLatin1String("maximum"), QSizeF(20.56, 20.78));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Rect, QString::number(i++) + QLatin1String(" Rect Property"));
	item->setValue(QRect(10, 10, 20, 20));
	topItem->addSubProperty(item);
	item->setAttribute(QLatin1String("constraint"), QRect(0, 0, 50, 50));

	item = propertyManager->addProperty(QVariant::RectF, QString::number(i++) + QLatin1String(" RectF Property"));
	item->setValue(QRectF(1.2345, 1.2345, 1.2345, 1.2345));
	topItem->addSubProperty(item);
	item->setAttribute(QLatin1String("constraint"), QRectF(0, 0, 50, 50));
	item->setAttribute(QLatin1String("decimals"), 3);

	item = propertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), QString::number(i++) + QLatin1String(" Enum Property"));
	QStringList enumNames;
	enumNames << "Enum0" << "Enum1" << "Enum2";
	item->setAttribute(QLatin1String("enumNames"), enumNames);
	item->setValue(1);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QtVariantPropertyManager::flagTypeId(), QString::number(i++) + QLatin1String(" Flag Property"));
	QStringList flagNames;
	flagNames << "Flag0" << "Flag1" << "Flag2";
	item->setAttribute(QLatin1String("flagNames"), flagNames);
	item->setValue(5);
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::SizePolicy, QString::number(i++) + QLatin1String(" SizePolicy Property"));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Font, QString::number(i++) + QLatin1String(" Font Property"));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Cursor, QString::number(i++) + QLatin1String(" Cursor Property"));
	topItem->addSubProperty(item);

	item = propertyManager->addProperty(QVariant::Color, QString::number(i++) + QLatin1String(" Color Property"));
	topItem->addSubProperty(item);

	propertyFactory = new QtVariantEditorFactory();

	propertyEditor = new QtTreePropertyBrowser();
	propertyEditor->setFactoryForManager(propertyManager, propertyFactory);
	propertyEditor->addProperty(topItem);
	propertyEditor->setPropertiesWithoutValueMarked(true);
	propertyEditor->setRootIsDecorated(false);

	layout->addWidget(propertyEditor);
}

ContentPanelProperties::~ContentPanelProperties(void)
{
}
