/****************************************************************************

				Cambridge Silicon Radio Ltd, 2006
 	 				
				All rights reserved

FILE:			generic_xml_error.h

DESCRIPTION:	Generic XML error

REVISION:		$Revision: #1 $

****************************************************************************/
#ifndef _GENERIC_XML_ERROR_H
#define _GENERIC_XML_ERROR_H

#include <msxml2.h>
#include "generic_object.h"

#define GENERIC_XML_DOCUMENT_ERROR_REPORT		TEXT("XML Parsing error:\n-------------------------------------------------------------------------\nFilename  : %s\nLine      : %u\nLinepos   : %u\nSourceText: %s\nErrorcode : 0x%08x\nReason    :\n%s\n-------------------------------------------------------------------------\n")

#define GENERIC_XML_ERROR_INFO_DESCRIPTION		TEXT("Description : %s\n")
#define GENERIC_XML_ERROR_INFO_GUID				TEXT("GUID        : %s\n")
#define GENERIC_XML_ERROR_INFO_HELPFILE			TEXT("Help File   : %s\n")
#define GENERIC_XML_ERROR_INFO_HELP_CONTEXT		TEXT("Help Context: %u\n")
#define GENERIC_XML_ERROR_INFO_COMPONENT		TEXT("Component   : %s\n")

#define GENERIC_XML_ERROR_PARSE_ERROR_CODE		TEXT("Error code   : 0x%08x\n")
#define GENERIC_XML_ERROR_PARSE_FILE_POS		TEXT("File position: %u\n")
#define GENERIC_XML_ERROR_PARSE_LINE			TEXT("Line number  : %u\n")
#define GENERIC_XML_ERROR_PARSE_LINE_POS		TEXT("Line position: %u\n")
#define GENERIC_XML_ERROR_PARSE_REASON			TEXT("Reason       : %s\n")
#define GENERIC_XML_ERROR_PARSE_SOURCE			TEXT("Source       : %s\n")
#define GENERIC_XML_ERROR_PARSE_URL				TEXT("URL          : %s\n")

#define GENERIC_XML_ERROR_NUMBER_SIZE			(10 * sizeof(TCHAR))

class GenericXMLError: public GenericObject
{
	private:
		IErrorInfo*				errorInfo;
		IXMLDOMParseError*		xmlParseError;

		HRESULT		GenerateStringFromErrorInfo(TCHAR** str);
		HRESULT		GenerateStringFromXMLDOMParseError(TCHAR** str);
	public:
		GenericXMLError(IXMLDOMParseError* xmlParseError = NULL);
		~GenericXMLError();

		HRESULT		ToString(TCHAR** str);
};

#endif
