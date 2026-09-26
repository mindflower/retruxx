/*
Copyright (c) 2000 Lee Thomason (www.grinninglizard.com)

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

// retruxx: the engine's modified TinyXML, as the PDB describes it. Strings are CStr instead of std::string and
// all input and output goes through m3d::fs::IStream instead of FILE* and the std streams; StreamOut takes the
// indentation depth and writes one line per element and attribute.

#ifndef TINYXML_INCLUDED
#define TINYXML_INCLUDED

#include <stdio.h>
#include <assert.h>
#include <core/stringm3d.h>

namespace m3d
{
	namespace fs
	{
		class IStream;
	}
}

class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;


/** TiXmlBase is a base class for every class in TinyXml.
	It does little except to establish that TinyXml classes
	can be printed and provide some utility functions.
*/
class TiXmlBase
{
	friend class TiXmlNode;
	friend class TiXmlElement;
	friend class TiXmlDocument;

  public:
	TiXmlBase()								{}
	virtual ~TiXmlBase()					{}

	/**	All TinyXml classes can print themselves to a stream.
		This is a formatted print, and will insert tabs and newlines.
	*/
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const = 0;

	// [internal] Writes the node, indented by depth tabs.
	virtual void StreamOut( m3d::fs::IStream* stream, int depth ) const = 0;

	/**	The world does not agree on whether white space should be kept or
		not. In order to make everyone happy, these global, static functions
		are provided to set whether or not TinyXml will condense all white space
		into a single space or not. The default is to condense. Note changing these
		values is not thread safe.
	*/
	static void SetCondenseWhiteSpace( bool condense )		{ condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool IsWhiteSpaceCondensed()						{ return condenseWhiteSpace; }

  protected:
	static const char* SkipWhiteSpace( const char* p );
	static bool StreamWhiteSpace( m3d::fs::IStream* in, CStr* tag );
	static bool IsWhiteSpace( int c );

	/*	Read to the specified character.
		Returns true if the character found and no error.
	*/
	static bool StreamTo( m3d::fs::IStream* in, int character, CStr* tag );

	/*	Reads an XML name into the string provided. Returns
		a pointer just past the last character of the name,
		or 0 if the function has an error.
	*/
	static const char* ReadName( const char* p, CStr* name );

	/*	Reads text. Returns a pointer past the given end tag.
		Wickedly complex options, but it keeps the (sensitive) code in one place.
	*/
	static const char* ReadText(	const char* p,				// where to start
									CStr* text,					// the string read
									bool trimWhiteSpace,		// whether to keep the white space
									const char* endTag,			// what ends this text
									bool caseInsensitive );		// whether to ignore case in the end tag

	virtual const char* Parse( const char* p ) = 0;

	// If an entity has been found, transform it into a character.
	static const char* GetEntity( const char* p, char* value );

	// Get a character, while interpreting entities.
	static const char* GetChar( const char* p, char* value );

	// Puts a string to a stream, expanding entities as it goes.
	static void PutString( const CStr& str, m3d::fs::IStream* stream );

	// Return true if the next characters in the stream are any of the endTag sequences.
	static bool StringEqual(	const char* p,
								const char* tag,
								bool ignoreCase );

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
	static const char* errorString[ TIXML_ERROR_STRING_COUNT ];

	struct Entity
	{
		/* 0x0000 */ const char*	str;
		/* 0x0004 */ unsigned int	strLength;
		/* 0x0008 */ int			chr;
	}; /* size: 0x000c */

  private:
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};
	static Entity entity[ NUM_ENTITY ];
	static bool condenseWhiteSpace;
}; /* size: 0x0004 */


/** The parent class for everything in the Document Object Model.
	(Except for attributes, which are contained in elements.)
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a TiXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class TiXmlNode : public TiXmlBase
{
  public:
	/** The types of XML nodes supported by TinyXml. (All the
		unsupported types are picked up by UNKNOWN.)
	*/
	enum NodeType
	{
		DOCUMENT = 0,
		ELEMENT = 1,
		COMMENT = 2,
		UNKNOWN = 3,
		TEXT = 4,
		DECLARATION = 5,
		TYPECOUNT = 6
	};

	virtual ~TiXmlNode();

	/** The meaning of 'value' changes for the specific type of
		TiXmlNode.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	const CStr& Value() const					{ return value; }

	/// Changes the value of the node.
	void SetValue( const CStr& _value )			{ value = _value; }

	/// Delete all the children of this node. Does not affect 'this'.
	void Clear();

	/// One step up the DOM.
	TiXmlNode* Parent() const					{ return parent; }

	TiXmlNode* FirstChild( const CStr& value ) const;	///< The first child of this node with the matching 'value'. Will be null if none found.
	TiXmlNode* FirstChild()	const	{ return firstChild; }		///< The first child of this node. Will be null if there are no children.

	TiXmlNode* LastChild( const CStr& value ) const;	/// The last child of this node matching 'value'. Will be null if there are no children.
	TiXmlNode* LastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.

	/// This flavor of IterateChildren searches for children with a particular 'value'
	TiXmlNode* IterateChildren( const CStr& val, TiXmlNode* previous ) const;

	/** An alternate way to walk the children of a node.
		IterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. IterateChildren will return null when done.
	*/
	TiXmlNode* IterateChildren( TiXmlNode* previous ) const;

	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertEndChild( const TiXmlNode& addThis );

	// The node is passed in by ownership. This object will delete it.
	TiXmlNode* LinkEndChild( TiXmlNode* node );

	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis );

	TiXmlNode* LinkBeforeChild( TiXmlNode* beforeThis, TiXmlNode* node );

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertAfterChild( TiXmlNode* afterThis, const TiXmlNode& addThis );

	TiXmlNode* LinkAfterChild( TiXmlNode* afterThis, TiXmlNode* node );

	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis );

	/// Delete a child of this node.
	bool RemoveChild( TiXmlNode* removeThis );

	/// Navigate to a sibling node.
	TiXmlNode* PreviousSibling( const CStr& value ) const;

	/// Navigate to a sibling node.
	TiXmlNode* PreviousSibling() const			{ return prev; }

	/// Navigate to a sibling node with the given 'value'.
	TiXmlNode* NextSibling( const CStr& value ) const;

	/// Navigate to a sibling node.
	TiXmlNode* NextSibling() const				{ return next; }

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	TiXmlElement* NextSiblingElement( const CStr& value ) const;

	/** Convenience function to get through elements.
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	TiXmlElement* NextSiblingElement() const;

	/// Convenience function to get through elements.
	TiXmlElement* FirstChildElement( const CStr& value ) const;

	/// Convenience function to get through elements.
	TiXmlElement* FirstChildElement() const;

	/// Query the type (as an enumerated value, above) of this node.
	virtual NodeType Type() const				{ return type; }

	/** Return a pointer to the Document this node lives in.
		Returns null if not in a document.
	*/
	TiXmlDocument* GetDocument() const;

	/// Returns true if this node has no children.
	bool NoChildren() const						{ return !firstChild; }

	TiXmlDocument* ToDocument()	const		{ return ( this && type == DOCUMENT ) ? (TiXmlDocument*) this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlElement*  ToElement() const		{ return ( this && type == ELEMENT  ) ? (TiXmlElement*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlComment*  ToComment() const		{ return ( this && type == COMMENT  ) ? (TiXmlComment*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlUnknown*  ToUnknown() const		{ return ( this && type == UNKNOWN  ) ? (TiXmlUnknown*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlText*	   ToText()    const		{ return ( this && type == TEXT     ) ? (TiXmlText*)     this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlDeclaration* ToDeclaration() const	{ return ( this && type == DECLARATION ) ? (TiXmlDeclaration*) this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.

	virtual TiXmlNode* Clone() const = 0;

	// The real work of the input operator.
	virtual void StreamIn( m3d::fs::IStream* in, CStr* tag ) = 0;

  protected:
	TiXmlNode( NodeType _type );

	// Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	TiXmlNode* Identify( const char* p );

	void CopyToClone( TiXmlNode* target ) const	{ target->value = value; }

	/* 0x0004 */ TiXmlNode*		parent;
	/* 0x0008 */ NodeType		type;
	/* 0x000c */ TiXmlNode*		firstChild;
	/* 0x0010 */ TiXmlNode*		lastChild;
	/* 0x0014 */ CStr			value;
	/* 0x0020 */ TiXmlNode*		prev;
	/* 0x0024 */ TiXmlNode*		next;
}; /* size: 0x0028 */


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not TiXmlNodes, since they are not
		  part of the tinyXML document object model. There are other
		  suggested ways to look at this problem.
*/
class TiXmlAttribute : public TiXmlBase
{
	friend class TiXmlAttributeSet;

  public:
	/// Construct an attribute with a name and value.
	TiXmlAttribute( const CStr& _name, const CStr& _value )	: name( _name ), value( _value ), prev( 0 ), next( 0 ) {}

	/// Construct an empty attribute.
	TiXmlAttribute() : prev( 0 ), next( 0 )	{}

	const CStr& Name()  const { return name; }		///< Return the name of this attribute.
	const CStr& Value() const { return value; }		///< Return the value of this attribute.
	const int          IntValue() const;			///< Return the value of this attribute, converted to an integer.
	const double	   DoubleValue() const;			///< Return the value of this attribute, converted to a double.

	void SetName( const CStr& _name )	{ name = _name; }		///< Set the name of this attribute.
	void SetValue( const CStr& _value )	{ value = _value; }		///< Set the value.
	void SetIntValue( int value );								///< Set the value from an integer.
	void SetDoubleValue( double value );						///< Set the value from a double.

	/// Get the next sibling attribute in the DOM. Returns null at end.
	TiXmlAttribute* Next() const;
	/// Get the previous sibling attribute in the DOM. Returns null at beginning.
	TiXmlAttribute* Previous() const;

	bool operator==( const TiXmlAttribute& rhs ) const { return rhs.name == name; }
	bool operator<( const TiXmlAttribute& rhs )	 const { return name < rhs.name; }
	bool operator>( const TiXmlAttribute& rhs )  const { return name > rhs.name; }

	/*	[internal use]
		Attribtue parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/
	virtual const char* Parse( const char* p );

	// [internal use]
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const;

	// [internal use]
	virtual void StreamOut( m3d::fs::IStream* stream, int depth ) const;

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void SetDocument( TiXmlDocument* doc )	{ document = doc; }

  private:
	/* 0x0004 */ TiXmlDocument*		document;	// A pointer back to a document, for error reporting.
	/* 0x0008 */ CStr				name;
	/* 0x0014 */ CStr				value;
	/* 0x0020 */ TiXmlAttribute*	prev;
	/* 0x0024 */ TiXmlAttribute*	next;
}; /* size: 0x0028 */


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class TiXmlAttributeSet
{
  public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add( TiXmlAttribute* addMe );
	void Remove( TiXmlAttribute* removeMe );

	TiXmlAttribute* First() const	{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }

	TiXmlAttribute*	Find( const CStr& name ) const;

  private:
	/* 0x0000 */ TiXmlAttribute sentinel;
}; /* size: 0x0028 */


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class TiXmlElement : public TiXmlNode
{
  public:
	/// Construct an element.
	TiXmlElement( const CStr& _value );

	virtual ~TiXmlElement();

	/** Given an attribute name, attribute returns the value
		for the attribute of that name, or null if none exists.
	*/
	const CStr* Attribute( const CStr& name, int* i ) const;

	/** Given an attribute name, attribute returns the value
		for the attribute of that name, or null if none exists.
	*/
	const CStr* Attribute( const CStr& name ) const;

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const CStr& name, int val );

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const CStr& name, const CStr& value );

	/** Deletes an attribute with the given name.
	*/
	void RemoveAttribute( const CStr& name );

	TiXmlAttribute* FirstAttribute() const	{ return attributeSet.First(); }		///< Access the first attribute in this element.

	// retruxx: the engine's addition, a lookup by C string.
	TiXmlAttribute* FindAttribute( const char* name ) const;

	// [internal use] Creates a new Element and returs it.
	virtual TiXmlNode* Clone() const;
	// [internal use]
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const;
	// [internal use]
	virtual void StreamOut( m3d::fs::IStream* stream, int depth ) const;
	// [internal use]
	virtual void StreamIn( m3d::fs::IStream* in, CStr* tag );

  protected:
	/*	[internal use]
		Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p );

	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	bool ReadValue( m3d::fs::IStream* in );
	const char* ReadValue( const char* p );

  private:
	/* 0x0028 */ TiXmlAttributeSet attributeSet;
}; /* size: 0x0050 */


/**	An XML comment.
*/
class TiXmlComment : public TiXmlNode
{
  public:
	/// Constructs an empty comment.
	TiXmlComment() : TiXmlNode( TiXmlNode::COMMENT ) {}
	virtual ~TiXmlComment()	{}

	// [internal use] Creates a new Element and returs it.
	virtual TiXmlNode* Clone() const;
	// [internal use]
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const;
	// [internal use]
	virtual void StreamOut( m3d::fs::IStream* stream, int depth ) const;
	// [internal use]
	virtual void StreamIn( m3d::fs::IStream* in, CStr* tag );

  protected:
	/*	[internal use]
		Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p );
}; /* size: 0x0028 */


/** XML text. Contained in an element.
*/
class TiXmlText : public TiXmlNode
{
  public:
	TiXmlText( const CStr& initValue )  : TiXmlNode( TiXmlNode::TEXT ) { SetValue( initValue ); }
	virtual ~TiXmlText() {}

	// [internal use] Creates a new Element and returns it.
	virtual TiXmlNode* Clone() const;
	// [internal use]
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const;
	// [internal use]
	virtual void StreamOut( m3d::fs::IStream* stream, int depth ) const;
	// [internal use]
	bool Blank() const;	// returns true if all white space and new lines
	/*	[internal use]
		Attribtue parsing starts: First char of the text
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p );
	// [internal use]
	virtual void StreamIn( m3d::fs::IStream* in, CStr* tag );
}; /* size: 0x0028 */


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration:
	version, encoding, and standalone.
*/
class TiXmlDeclaration : public TiXmlNode
{
  public:
	/// Construct.
	TiXmlDeclaration( const CStr& _version,
					  const CStr& _encoding,
					  const CStr& _standalone );

	/// Construct an empty declaration.
	TiXmlDeclaration()   : TiXmlNode( TiXmlNode::DECLARATION ) {}

	virtual ~TiXmlDeclaration()	{}

	/// Version. Will return empty if none was found.
	const CStr& Version() const			{ return version; }
	/// Encoding. Will return empty if none was found.
	const CStr& Encoding() const		{ return encoding; }
	/// Is this a standalone document?
	const CStr& Standalone() const		{ return standalone; }
	void SetVersion( const CStr& v )	{ version = v; }
	void SetEncoding( const CStr& v )	{ encoding = v; }
	void SetStandalone( const CStr& v )	{ standalone = v; }

	// [internal use] Creates a new Element and returs it.
	virtual TiXmlNode* Clone() const;
	// [internal use]
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const;
	// [internal use]
	virtual void StreamOut( m3d::fs::IStream* stream, int depth ) const;
	// [internal use]
	virtual void StreamIn( m3d::fs::IStream* in, CStr* tag );

  protected:
	//	[internal use]
	//	Attribtue parsing starts: next char past '<'
	//					 returns: next char past '>'
	virtual const char* Parse( const char* p );

  private:
	/* 0x0028 */ CStr version;
	/* 0x0034 */ CStr encoding;
	/* 0x0040 */ CStr standalone;
}; /* size: 0x004c */


/** Any tag that tinyXml doesn't recognize is save as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.
*/
class TiXmlUnknown : public TiXmlNode
{
  public:
	TiXmlUnknown() : TiXmlNode( TiXmlNode::UNKNOWN ) {}
	virtual ~TiXmlUnknown() {}

	// [internal use]
	virtual TiXmlNode* Clone() const;
	// [internal use]
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const;
	// [internal use]
	virtual void StreamOut( m3d::fs::IStream* stream, int depth ) const;
	// [internal use]
	virtual void StreamIn( m3d::fs::IStream* in, CStr* tag );

  protected:
	/*	[internal use]
		Attribute parsing starts: First char of the text
						 returns: next char past '>'
	*/
	virtual const char* Parse( const char* p );
}; /* size: 0x0028 */


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class TiXmlDocument : public TiXmlNode
{
  public:
	/// Create a document with a name. The name of the document is also the filename of the xml.
	TiXmlDocument( const CStr& documentName );
	/// Create an empty document, that has no name.
	TiXmlDocument();

	virtual ~TiXmlDocument() {}

	/// Load a file using the given filename. Returns true if successful.
	bool LoadFile( const CStr& filename );
	/** Load a file using the current document value.
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool LoadFile();
	/// Save a file using the given filename. Not supported: asserts.
	bool SaveFile( const CStr& filename ) const;
	/// Save a file using the current document value. Not supported: asserts.
	bool SaveFile() const;

	/// Parse the given null terminated block of xml data.
	virtual const char* Parse( const char* p );

	/** Get the root element -- the only top level element -- of the document.
		In well formed XML, there should only be one. TinyXml is tolerant of
		multiple elements at the document level.
	*/
	TiXmlElement* RootElement() const		{ return FirstChildElement(); }

	/// If, during parsing, a error occurs, Error will be set to true.
	bool Error() const						{ return error; }

	/// Contains a textual (english) description of the error if one occurs.
	const CStr& ErrorDesc() const			{ return errorDesc; }

	/** Generally, you probably want the error string ( ErrorDesc() ). But if you
		prefer the ErrorId, this function will fetch it.
	*/
	const int ErrorId()	const				{ return errorId; }

	/// If you have handled the error, it can be reset with this call.
	void ClearError()						{ error = false; errorId = 0; errorDesc = ""; }

	// [internal use]
	virtual void Print( m3d::fs::IStream* cfile, int depth ) const;
	// [internal use]
	virtual void StreamOut( m3d::fs::IStream* out, int depth ) const;
	// [internal use]
	virtual TiXmlNode* Clone() const;
	// [internal use]
	void SetError( int err ) {		assert( err > 0 && err < TIXML_ERROR_STRING_COUNT );
									error   = true;
									errorId = err;
									errorDesc = errorString[ errorId ]; }
	// [internal use]
	virtual void StreamIn( m3d::fs::IStream* in, CStr* tag );

  private:
	/* 0x0028 */ bool error;
	/* 0x002c */ int  errorId;
	/* 0x0030 */ CStr errorDesc;
}; /* size: 0x003c */

static_assert(sizeof(TiXmlNode) == 0x28);
static_assert(sizeof(TiXmlAttribute) == 0x28);
static_assert(sizeof(TiXmlAttributeSet) == 0x28);
static_assert(sizeof(TiXmlElement) == 0x50);
static_assert(sizeof(TiXmlDeclaration) == 0x4c);
static_assert(sizeof(TiXmlDocument) == 0x3c);

#endif
