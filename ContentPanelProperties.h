#ifndef CONTENTPANELPROPERTIES_H
#define CONTENTPANELPROPERTIES_H

#include "ContentPanelBase.h"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

namespace nvr 
{

	class ContentPanelProperties : public ContentPanelBase
	{
		Q_OBJECT

	public:
		ContentPanelProperties(QWidget *parent = 0);
		~ContentPanelProperties(void);

	public slots:

	private:
		QtVariantPropertyManager * propertyManager;
		QtVariantEditorFactory *propertyFactory;
		QtTreePropertyBrowser *propertyEditor;
	};

} // END NAMESPACE NVR

#endif // CONTENTPANELPROPERTIES_H
