/*
www.sourceforge.net/projects/tinyxml
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/
#ifndef __TXML_H__
#define __TXML_H__

#pragma warning(disable : 4530)
#pragma warning(disable : 4786)

#include <stdio.h>
#include <string.h>

class TXmlDocument;
class TXmlElement;
class TXmlComment;
class TXmlUnknown;
class TXmlAttribute;
class TXmlText;
class TXmlDeclaration;

// Help out windows:
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUG) && defined(_MSC_VER)
#include <windows.h>
//	#include <crtdbg.h>
#define TXMLTRACE	OutputDebugString
#define TXMLASSERT()
#else
#include <assert.h>
#define TXMLTRACE	printf
#define TXMLASSERT	assert
#endif

#include "txmlstr.h"
#include "txmlstrm.h"

/** TXmlBase is a base class for every class in TXml.
	It does little except to establish that TXml classes
	can be printed and provide some utility functions.

	In XML, the document and elements can contain
	other elements and other types of nodes.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration(leaf)

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	A Decleration contains: Attributes (not on tree)
	@endverbatim
*/
class TXmlBase
{
    friend class TXmlNode;
    friend class TXmlElement;
    friend class TXmlDocument;

public:
    TXmlBase()			{}
    virtual ~TXmlBase()	{}

    virtual LPCTSTR Parse(LPCTSTR p) = 0;

    /**	All TXml classes can print themselves to a filestream.
    	This is a formatted print, and will insert tabs and newlines.
    */

    virtual void Print(TXmlOutStream &out, int depth) const = 0;

    // [internal]
    virtual void StreamOut(TXmlOutStream &out) const = 0;

    /**	The world does not agree on whether white space should be kept or
    	not. In order to make everyone happy, these global, static functions
    	are provided to set whether or not TXml will condense all white space
    	into a single space or not. The default is to condense. Note changing these
    	values is not thread safe.
    */
    static void SetCondenseWhiteSpace(bool condense)
    {
        condenseWhiteSpace = condense;
    }

    /// Return the current white space setting.
    static bool IsWhiteSpaceCondensed()
    {
        return condenseWhiteSpace;
    }

protected:
    static LPCTSTR SkipWhiteSpace(LPCTSTR);
    static bool StreamWhiteSpace(TXmlInStream &in, TXmlString &tag);
    static bool IsWhiteSpace(const TCHAR c)
    {
        return (_istspace(c) || c == '\n' || c == '\r');
    }

    /*	Read to the specified character.
    	Returns true if the character found and no error.
    */
    static bool StreamTo(TXmlInStream &in, int character, TXmlString &tag);

    /*	Reads an XML name into the string provided. Returns
    	a pointer just past the last character of the name,
    	or 0 if the function has an error.
    */
    static LPCTSTR ReadName(LPCTSTR, TXmlString &name);

    /*	Reads text. Returns a pointer past the given end tag.
    	Wickedly complex options, but it keeps the (sensitive) code in one place.
    */
    static LPCTSTR ReadText(LPCTSTR in,				// where to start
                            TXmlString &text,			// the string read
                            bool ignoreWhiteSpace,		// whether to keep the white space
                            LPCTSTR endTag,			// what ends this text
                            bool ignoreCase);			// whether to ignore case in the end tag

    // If an entity has been found, transform it into a character.
    static LPCTSTR GetEntity(LPCTSTR in, TCHAR &value);

    // Get a character, while interpreting entities.
    inline static LPCTSTR GetChar(LPCTSTR p, TCHAR &value)
    {
        TXMLASSERT(p);

        if (*p == '&')
        {
            return GetEntity(p, value);
        }
        else
        {
            value = *p;
            return p + 1;
        }
    }

    // Puts a string to a stream, expanding entities as it goes.
    // Note this should not contian the '<', '>', etc, or they will be transformed into entities!
    static void PutString(const TXmlString &str, TXmlOutStream &stream);

    // Return true if the next characters in the stream are any of the endTag sequences.
    bool static StringEqual(LPCTSTR p,
                            LPCTSTR endTag,
                            bool ignoreCase);

    enum
    {
        TIXML_NO_ERROR = 0,
        TIXML_ERROR,
        TIXML_ERROR_OPENING_FILE,
        TIXML_ERROR_OUT_OF_MEMORY,
        TIXML_ERROR_PARSING_ELEMENT,
        TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
        TIXML_ERROR_READING_ELEMENT_VALUE,
        TIXML_ERROR_READING_ATTRIBUTES,
        TIXML_ERROR_PARSING_EMPTY,
        TIXML_ERROR_READING_END_TAG,
        TIXML_ERROR_PARSING_UNKNOWN,
        TIXML_ERROR_PARSING_COMMENT,
        TIXML_ERROR_PARSING_DECLARATION,
        TIXML_ERROR_DOCUMENT_EMPTY,

        TIXML_ERROR_STRING_COUNT
    };
    static LPCTSTR errorString[TIXML_ERROR_STRING_COUNT];

private:
    struct Entity
    {
        LPCTSTR     str;
        unsigned int	strLength;
        int			    chr;
    };
    enum
    {
        NUM_ENTITY = 5,
        MAX_ENTITY_LENGTH = 6

    };
    static Entity entity[NUM_ENTITY];
    static bool condenseWhiteSpace;
};


/** The parent class for everything in the Document Object Model.
	(Except for attributes, which are contained in elements.)
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a TXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class TXmlNode : public TXmlBase
{
public:

    /** The types of XML nodes supported by TXml. (All the
    	unsupported types are picked up by UNKNOWN.)
    */
    enum NodeType
    {
        NODE_DOCUMENT,
        NODE_ELEMENT,
        NODE_COMMENT,
        NODE_UNKNOWN,
        NODE_TEXT,
        NODE_DECLARATION,
        NODE_TYPECOUNT
    };

    virtual ~TXmlNode();

    /** The meaning of 'value' changes for the specific type of
    	TXmlNode.
    	@verbatim
    	Document:	filename of the xml file
    	Element:	name of the element
    	Comment:	the comment text
    	Unknown:	the tag contents
    	Text:		the text string
    	@endverbatim

    	The subclasses will wrap this function.
    */
    LPCTSTR Value()	const
    {
        return (LPCTSTR)value;
    }

    /** Changes the value of the node. Defined as:
    	@verbatim
    	Document:	filename of the xml file
    	Element:	name of the element
    	Comment:	the comment text
    	Unknown:	the tag contents
    	Text:		the text string
    	@endverbatim
    */
    void SetValue(LPCTSTR _value)
    {
        value = _value;
    }

    /// Delete all the children of this node. Does not affect 'this'.
    void Clear();

    /// One step up the DOM.
    TXmlNode *Parent() const
    {
        return parent;
    }

    TXmlNode *FirstChild()	const
    {
        return firstChild;    ///< The first child of this node. Will be null if there are no children.
    }
    TXmlNode *FirstChild(LPCTSTR value) const;				///< The first child of this node with the matching 'value'. Will be null if none found.

    TXmlNode *LastChild() const
    {
        return lastChild;    /// The last child of this node. Will be null if there are no children.
    }
    TXmlNode *LastChild(LPCTSTR value) const;				/// The last child of this node matching 'value'. Will be null if there are no children.

    /** An alternate way to walk the children of a node.
    	One way to iterate over nodes is:
    	@verbatim
    		for (child = parent->FirstChild(); child; child = child->NextSibling())
    	@endverbatim

    	IterateChildren does the same thing with the syntax:
    	@verbatim
    		child = 0;
    		while (child = parent->IterateChildren(child))
    	@endverbatim

    	IterateChildren takes the previous child as input and finds
    	the next one. If the previous child is null, it returns the
    	first. IterateChildren will return null when done.
    */
    TXmlNode *IterateChildren(TXmlNode *previous) const;

    /// This flavor of IterateChildren searches for children with a particular 'value'
    TXmlNode *IterateChildren(LPCTSTR value, TXmlNode *previous) const;

    /** Add a new node related to this. Adds a child past the LastChild.
    	Returns a pointer to the new object or NULL if an error occured.
    */
    TXmlNode *InsertEndChild(const TXmlNode &addThis);

    /** Add a new node related to this. Adds a child before the specified child.
    	Returns a pointer to the new object or NULL if an error occured.
    */
    TXmlNode *InsertBeforeChild(TXmlNode *beforeThis, const TXmlNode &addThis);

    /** Add a new node related to this. Adds a child after the specified child.
    	Returns a pointer to the new object or NULL if an error occured.
    */
    TXmlNode *InsertAfterChild(TXmlNode *afterThis, const TXmlNode &addThis);

    /** Replace a child of this node.
    	Returns a pointer to the new object or NULL if an error occured.
    */
    TXmlNode *ReplaceChild(TXmlNode *replaceThis, const TXmlNode &withThis);

    /// Delete a child of this node.
    bool RemoveChild(TXmlNode *removeThis);

    /// Navigate to a sibling node.
    TXmlNode *PreviousSibling() const
    {
        return prev;
    }

    /// Navigate to a sibling node.
    TXmlNode *PreviousSibling(LPCTSTR value) const;

    /// Navigate to a sibling node.
    TXmlNode *NextSibling() const
    {
        return next;
    }

    /// Navigate to a sibling node with the given 'value'.
    TXmlNode *NextSibling(LPCTSTR value) const;

    /** Convenience function to get through elements.
    	Calls NextSibling and ToElement. Will skip all non-Element
    	nodes. Returns 0 if there is not another element.
    */
    TXmlElement *NextSiblingElement() const;

    /** Convenience function to get through elements.
    	Calls NextSibling and ToElement. Will skip all non-Element
    	nodes. Returns 0 if there is not another element.
    */
    TXmlElement *NextSiblingElement(LPCTSTR value) const;

    /// Convenience function to get through elements.
    TXmlElement *FirstChildElement()	const;

    /// Convenience function to get through elements.
    TXmlElement *FirstChildElement(LPCTSTR value, bool create = false) const;

    /// Convenience function to get through elements.
    void SetFirstChildText(LPCTSTR init_value = NULL) const;

    /// Query the type (as an enumerated value, above) of this node.
    virtual int Type() const
    {
        return type;
    }

    /** Return a pointer to the Document this node lives in.
    	Returns null if not in a document.
    */
    TXmlDocument *GetDocument() const;

    /// Returns true if this node has no children.
    bool HasChildren() const
    {
        return firstChild != NULL;
    }

    TXmlDocument		*ToDocument() const
    {
        return (this && type == NODE_DOCUMENT) ? (TXmlDocument *) this : NULL;    ///< Cast to a more defined type. Will return null not of the requested type.
    }
    TXmlElement		*ToElement() const
    {
        return (this && type == NODE_ELEMENT) ? (TXmlElement *)  this : NULL;    ///< Cast to a more defined type. Will return null not of the requested type.
    }
    TXmlComment		*ToComment() const
    {
        return (this && type == NODE_COMMENT) ? (TXmlComment *)  this : NULL;    ///< Cast to a more defined type. Will return null not of the requested type.
    }
    TXmlUnknown		*ToUnknown() const
    {
        return (this && type == NODE_UNKNOWN) ? (TXmlUnknown *)  this : NULL;    ///< Cast to a more defined type. Will return null not of the requested type.
    }
    TXmlText			*ToText() const
    {
        return (this && type == NODE_TEXT) ? (TXmlText *)     this : NULL;    ///< Cast to a more defined type. Will return null not of the requested type.
    }
    TXmlDeclaration	*ToDeclaration() const
    {
        return (this && type == NODE_DECLARATION) ? (TXmlDeclaration *) this : NULL;    ///< Cast to a more defined type. Will return null not of the requested type.
    }

    virtual TXmlNode *Clone() const = 0;

    // The real work of the input operator.
    virtual void StreamIn(TXmlInStream &in, TXmlString &tag) = 0;

protected:
    TXmlNode(NodeType type);

    // The node is passed in by ownership. This object will delete it.
    TXmlNode *LinkEndChild(TXmlNode *addThis);

    // Figure out what is at *p, and parse it. Returns null if it is not an xml node.
    TXmlNode *Identify(LPCTSTR start);

    void CopyToClone(TXmlNode *target) const
    {
        target->value = value;
    }

    TXmlNode	*parent;
    NodeType	type;

    TXmlNode	*firstChild;
    TXmlNode	*lastChild;

    TXmlString	value;

    TXmlNode	*prev;
    TXmlNode	*next;
};


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not TXmlNodes, since they are not
		  part of the TXML document object model. There are other
		  suggested ways to look at this problem.

	@note Attributes have a parent
*/
class TXmlAttribute : public TXmlBase
{
    friend class TXmlAttributeSet;

public:
    /// Construct an empty attribute.
    TXmlAttribute() : prev(NULL), next(NULL), document(NULL)	{}

    /// Construct an attribute with a name and value.
    TXmlAttribute(LPCTSTR _name, LPCTSTR _value) : name(_name), value(_value), prev(NULL), next(NULL), document(NULL) {}

    LPCTSTR Name()  const
    {
        return name;    ///< Return the name of this attribute.
    }
    LPCTSTR Value() const
    {
        return value;    ///< Return the value of this attribute.
    }

    void SetName(LPCTSTR _name)
    {
        name = _name;    ///< Set the name of this attribute.
    }
    void SetValue(LPCTSTR _value)
    {
        value = _value;    ///< Set the value.
    }

    /// Get the next sibling attribute in the DOM. Returns null at end.
    TXmlAttribute *Next() const;
    /// Get the previous sibling attribute in the DOM. Returns null at beginning.
    TXmlAttribute *Previous() const;

    bool operator==(const TXmlAttribute &rhs) const
    {
        return rhs.name == name;
    }
    bool operator<(const TXmlAttribute &rhs) const
    {
        return name < rhs.name;
    }
    bool operator>(const TXmlAttribute &rhs) const
    {
        return name > rhs.name;
    }

    /*	[internal use]
    	Attribtue parsing starts: first letter of the name
    	returns: the next char after the value end quote
    */
    virtual LPCTSTR Parse(LPCTSTR p);

    // [internal use]
    virtual void Print(TXmlOutStream &out, int depth) const;

    // [internal use]
    virtual void StreamOut(TXmlOutStream &out) const;

    // [internal use]
    // Set the document pointer so the attribute can report errors.
    void SetDocument(TXmlDocument *doc)
    {
        document = doc;
    }

private:
    TXmlDocument	*document;	// A pointer back to a document, for error reporting.
    TXmlString		name;
    TXmlString		value;

    TXmlAttribute	*prev;
    TXmlAttribute	*next;
};


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.

	The set can be changed transparent to the Element and Declaration
	classes that use it, but NOT transparent to the Attribute
	which has to implement a next() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class TXmlAttributeSet
{
public:
    TXmlAttributeSet();
    ~TXmlAttributeSet();

    void Add(TXmlAttribute *attribute);
    void Remove(TXmlAttribute *attribute);

    TXmlAttribute *First() const
    {
        return (sentinel.next == &sentinel) ? NULL : sentinel.next;
    }
    TXmlAttribute *Last()  const
    {
        return (sentinel.prev == &sentinel) ? NULL : sentinel.prev;
    }

    TXmlAttribute *Find(LPCTSTR name) const;

private:
    TXmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class TXmlElement : public TXmlNode
{
public:
    /// Construct an element.
    TXmlElement(LPCTSTR value);

    virtual ~TXmlElement();

    /** Given an attribute name, attribute returns the value
    	for the attribute of that name, or null if none exists.
    */
    LPCTSTR Attribute(LPCTSTR name) const;

    /** Sets an attribute of name to a given value. The attribute
    	will be created if it does not exist, or changed if it does.
    */
    void SetAttribute(LPCTSTR name, LPCTSTR value);

    /** Deletes an attribute with the given name.
    */
    void RemoveAttribute(LPCTSTR name);

    TXmlAttribute *FirstAttribute() const
    {
        return attributeSet.First();    ///< Access the first attribute in this element.
    }
    TXmlAttribute *LastAttribute() const
    {
        return attributeSet.Last();    ///< Access the last attribute in this element.
    }

    /*	[internal use]
    	Attribtue parsing starts: next char past '<'
    	returns: next char past '>'
    */
    virtual LPCTSTR Parse(LPCTSTR p);

    // [internal use] Creates a new Element and returs it.
    virtual TXmlNode *Clone() const;
    // [internal use]
    virtual void Print(TXmlOutStream &out, int depth) const;
    // [internal use]
    virtual void StreamOut(TXmlOutStream &out) const;
    // [internal use]
    virtual void StreamIn(TXmlInStream &in, TXmlString &tag);


protected:

    /*	[internal use]
    	Reads the "value" of the element -- another element, or text.
    	This should terminate with the current end tag.
    */
    LPCTSTR ReadValue(LPCTSTR in);
    bool ReadValue(TXmlInStream &in);

private:
    TXmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class TXmlComment : public TXmlNode
{
public:
    /// Constructs an empty comment.
    TXmlComment() : TXmlNode(TXmlNode::NODE_COMMENT) {}
    virtual ~TXmlComment()	{}

    /*	[internal use]
    	Attribtue parsing starts: at the ! of the !--
    	returns: next char past '>'
    */
    virtual LPCTSTR Parse(LPCTSTR p);

    // [internal use] Creates a new Element and returs it.
    virtual TXmlNode *Clone() const;
    // [internal use]
    virtual void Print(TXmlOutStream &out, int depth) const;
    // [internal use]
    virtual void StreamOut(TXmlOutStream &out) const;
    // [internal use]
    virtual void StreamIn(TXmlInStream &in, TXmlString &tag);
};


/** XML text. Contained in an element.
*/
class TXmlText : public TXmlNode
{
public:
    TXmlText(LPCTSTR initValue)  : TXmlNode(TXmlNode::NODE_TEXT)
    {
        SetValue(initValue);
    }
    virtual ~TXmlText() {}

    // [internal use] Creates a new Element and returns it.
    virtual TXmlNode *Clone() const;
    // [internal use]
    virtual void Print(TXmlOutStream &out, int depth) const;
    // [internal use]
    virtual void StreamOut(TXmlOutStream &out) const;
    // [internal use]
    bool IsBlank() const;	// returns true if all white space and new lines
    /*	[internal use]
    	Attribtue parsing starts: First char of the text
    	returns: next char past '>'
    */
    virtual LPCTSTR Parse(LPCTSTR p);
    // [internal use]
    virtual void StreamIn(TXmlInStream &in, TXmlString &tag);
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration:
	version, encoding, and standalone.

	Note: In this version of the code, the attributes are
	handled as special cases, not generic attributes, simply
	because there can only be at most 3 and they are always the same.
*/
class TXmlDeclaration : public TXmlNode
{
public:
    /// Construct an empty declaration.
    TXmlDeclaration() : TXmlNode(TXmlNode::NODE_DECLARATION) {}

    /// Construct.
    TXmlDeclaration(LPCTSTR version,
                    LPCTSTR encoding,
                    LPCTSTR standalone);

    virtual ~TXmlDeclaration()	{}

    /// Version. Will return empty if none was found.
    LPCTSTR Version() const
    {
        return version;
    }
    /// Encoding. Will return empty if none was found.
    LPCTSTR Encoding() const
    {
        return encoding;
    }
    /// Is this a standalone document?
    LPCTSTR Standalone() const
    {
        return standalone;
    }

    //	[internal use]
    //	Attribtue parsing starts: next char past '<'
    //	returns: next char past '>'

    virtual LPCTSTR Parse(LPCTSTR p);

    // [internal use] Creates a new Element and returs it.
    virtual TXmlNode *Clone() const;
    // [internal use]
    virtual void Print(TXmlOutStream &out, int depth) const;
    // [internal use]
    virtual void StreamOut(TXmlOutStream &out) const;
    // [internal use]
    virtual void StreamIn(TXmlInStream &in, TXmlString &tag);

private:
    TXmlString	version;
    TXmlString	encoding;
    TXmlString	standalone;
};


/** Any tag that TXml doesn't recognize is save as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.
*/
class TXmlUnknown : public TXmlNode
{
public:
    TXmlUnknown() : TXmlNode(TXmlNode::NODE_UNKNOWN) {}
    virtual ~TXmlUnknown() {}

    /*	[internal use]
    	Attribute parsing starts: First char of the text
    	returns: next char past '>'
    */
    virtual LPCTSTR Parse(LPCTSTR p);

    // [internal use]
    virtual TXmlNode *Clone() const;
    // [internal use]
    virtual void Print(TXmlOutStream &out, int depth) const;
    // [internal use]
    virtual void StreamOut(TXmlOutStream &out) const;
    // [internal use]
    virtual void StreamIn(TXmlInStream &in, TXmlString &tag);
};


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class TXmlDocument : public TXmlNode
{
public:
    /// Create an empty document, that has no name.
    TXmlDocument();
    /// Create a document with a name. The name of the document is also the filename of the xml.
    TXmlDocument(LPCTSTR documentName);

    virtual ~TXmlDocument() {}

    /** Load a file using the current document value.
    	Returns true if successful. Will delete any existing
    	document data before loading.
    */
    bool LoadFile();
    /// Save a file using the current document value. Returns true if successful.
    bool SaveFile() const;
    /// Load a file using the given filename. Returns true if successful.
    bool LoadFile(LPCTSTR filename);
    /// Save a file using the given filename. Returns true if successful.
    bool SaveFile(LPCTSTR filename) const;

    /// Parse the given null terminated block of xml data.
    virtual LPCTSTR Parse(LPCTSTR p);

    /** Get the root element -- the only top level element -- of the document.
    	In well formed XML, there should only be one. TXml is tolerant of
    	multiple elements at the document level.
    */
    TXmlElement *RootElement() const
    {
        return FirstChildElement();
    }

    /// If, during parsing, a error occurs, Error will be set to true.
    bool Error() const
    {
        return error;
    }

    /// Contains a textual (english) description of the error if one occurs.
    LPCTSTR ErrorDesc() const
    {
        return errorDesc;
    }

    /** Generally, you probably want the error string (ErrorDesc()). But if you
    	prefer the ErrorId, this function will fetch it.
    */
    int ErrorId()	const
    {
        return errorId;
    }

    /// If you have handled the error, it can be reset with this call.
    void ClearError()
    {
        error = false;
        errorId = 0;
        errorDesc = "";
    }

    // [internal use]
    virtual void Print(TXmlOutStream &out, int depth = 0) const;
    // [internal use]
    virtual void StreamOut(TXmlOutStream &out) const;
    // [internal use]
    virtual TXmlNode *Clone() const;
    // [internal use]
    void SetError(int err)
    {
        TXMLASSERT(err > 0 && err < TIXML_ERROR_STRING_COUNT);
        error   = true;
        errorId = err;
        errorDesc = errorString[errorId];
    }
    // [internal use]
    virtual void StreamIn(TXmlInStream &in, TXmlString &tag);

private:
    bool		error;
    int			errorId;
    TXmlString	errorDesc;
};

#endif

