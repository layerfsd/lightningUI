/****************************************************************************

				Cambridge Silicon Radio Ltd, 2006
 	 				
				All rights reserved

FILE:			generic_xml_document.h

DESCRIPTION:	Generic XML document

REVISION:		$Revision: #1 $

****************************************************************************/
#ifndef _GENERIC_XML_DOCUMENT_H
#define _GENERIC_XML_DOCUMENT_H

#include <msxml2.h>
#include "generic_object.h"
#include "generic_xml_error.h"

class GenericXMLDocument: public GenericObject
{
	private:
		IXMLDOMDocument2*			xmlDocument;
		IXMLDOMElement*				xmlDocumentElement;
		BOOL						async;
		BOOL						validateOnParse;
		BOOL						resolveExternals;

		HRESULT				Load(TCHAR* filename, TCHAR* buffer, TCHAR* documentType = NULL, GenericXMLError** xmlError = NULL);
	public:
		GenericXMLDocument(BOOL async = FALSE, BOOL validateOnParse = TRUE, BOOL resolveExternals = TRUE);
		~GenericXMLDocument();

		HRESULT				GetAttributeNameAndValue(IXMLDOMNode* attributeNode, BSTR* name, BSTR* value);
		IXMLDOMElement*		GetRootDocumentElement();
		HRESULT				GetNameAndValidate(IXMLDOMNode* xmlNode, BSTR* name, DOMNodeType nodeType);
		HRESULT				GetNextSibling(IXMLDOMNode** xmlNode);
		HRESULT				GetSingleText(IXMLDOMNode* xmlNode, BSTR* text);
		BOOL				IsElement();
		HRESULT				LoadFromFile(TCHAR* filename, TCHAR* documentType = NULL, GenericXMLError** xmlError = NULL);
		HRESULT				LoadFromBuffer(TCHAR* buffer, TCHAR* documentType = NULL, GenericXMLError** xmlError = NULL);
		HRESULT				StringToVariant(TCHAR* str, LPVARIANT variant);
		HRESULT				TransformDocument(GenericXMLDocument* genericXMLDocument, BSTR* output, GenericXMLError** xmlError = NULL);
};

#endif
