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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tinyxml.h"
#include <file/i_stream.h>


bool TiXmlBase::condenseWhiteSpace = true;

namespace
{
	void WriteString( m3d::fs::IStream* stream, const CStr& str )
	{
		stream->WriteBytes( str.c_str(), str.length() );
	}

	// "\n" and depth tabs: attributes and elements each start a new line.
	CStr NewLine( int depth )
	{
		return CStr( "\n" ) + CStr( '\t', depth );
	}
}


void TiXmlBase::PutString( const CStr& str, m3d::fs::IStream* stream )
{
	// RVA 0x8D3F70 - only '&' is turned into an entity; the other special characters are written as they are.
	// NOTE: CStr::find returns the position of the '&' relative to i, which is used below as if it were absolute.
	// The first '&' is handled right, but at a later one the text before it is not written and the character at
	// the relative position is examined instead; once that points back at a handled '&' the loop never ends.
	int i = 0;
	while ( i < str.length() )
	{
		int const next = str.find( '&', i );
		if ( next == CStr_npos )
		{
			stream->WriteBytes( str.c_str() + i, str.length() - i );
			return;
		}

		if ( next - i > 0 )
			stream->WriteBytes( str.c_str() + i, next - i );

		const char* const p = str.c_str() + next;
		// Check for the special "&#x" entitity
		if ( next < str.length() - 2 && p[0] == '&' && p[1] == '#' && p[2] == 'x' )
		{
			stream->WriteBytes( p, 1 );
		}
		else
		{
			int j;
			for ( j = 0; j < NUM_ENTITY; ++j )
			{
				if ( *p == entity[j].chr )
				{
					stream->WriteBytes( entity[j].str, entity[j].strLength );
					break;
				}
			}
			if ( j == NUM_ENTITY )
				stream->WriteBytes( str.c_str() + next, 1 );
		}
		i = next + 1;
	}
}


TiXmlNode::TiXmlNode( NodeType _type )
{
	// RVA 0x8D40D0
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


TiXmlNode::~TiXmlNode()
{
	// RVA 0x8D4100
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}
}


void TiXmlNode::Clear()
{
	// RVA 0x8D39B0
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = 0;
	lastChild = 0;
}


TiXmlNode* TiXmlNode::LinkEndChild( TiXmlNode* node )
{
	// RVA 0x8D39E0
	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertEndChild( const TiXmlNode& addThis )
{
	// RVA 0x8D3A10
	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}


TiXmlNode* TiXmlNode::InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis )
{
	// RVA 0x8D4150
	if ( beforeThis->parent != this )
		return 0;

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	return LinkBeforeChild( beforeThis, node );
}


TiXmlNode* TiXmlNode::LinkBeforeChild( TiXmlNode* beforeThis, TiXmlNode* node )
{
	// RVA 0x8D3A50
	node->parent = this;
	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis == firstChild )
	{
		firstChild->prev = node;
		firstChild = node;
	}
	else
	{
		beforeThis->prev->next = node;
		beforeThis->prev = node;
	}
	return node;
}


TiXmlNode* TiXmlNode::InsertAfterChild( TiXmlNode* afterThis, const TiXmlNode& addThis )
{
	// RVA 0x8D41A0
	if ( afterThis->parent != this )
		return 0;

	TiXmlNode* node = addThis.Clone();
	if ( !node )
		return 0;
	return LinkAfterChild( afterThis, node );
}


TiXmlNode* TiXmlNode::LinkAfterChild( TiXmlNode* afterThis, TiXmlNode* node )
{
	// RVA 0x8D3A90
	// NOTE: afterThis must not be the last child: its next is dereferenced and lastChild is not updated.
	node->parent = this;
	node->prev = afterThis;
	node->next = afterThis->next;
	afterThis->next->prev = node;
	afterThis->next = node;
	return node;
}


TiXmlNode* TiXmlNode::ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis )
{
	// RVA 0x8D3AB0
	if ( replaceThis->parent != this )
		return 0;

	TiXmlNode* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool TiXmlNode::RemoveChild( TiXmlNode* removeThis )
{
	// RVA 0x8D3B30 - a node that is not a child only asserts; it is unlinked anyway.
	if ( removeThis->parent != this )
	{
		assert( 0 );
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}


TiXmlNode* TiXmlNode::FirstChild( const CStr& value ) const
{
	// RVA 0x8D41E0
	TiXmlNode* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( node->Value() == value )
			return node;
	}
	return 0;
}


TiXmlNode* TiXmlNode::LastChild( const CStr& value ) const
{
	// RVA 0x8D4220
	TiXmlNode* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( node->Value() == value )
			return node;
	}
	return 0;
}


TiXmlNode* TiXmlNode::IterateChildren( TiXmlNode* previous ) const
{
	// RVA 0x8D3BA0
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling();
	}
}


TiXmlNode* TiXmlNode::IterateChildren( const CStr& val, TiXmlNode* previous ) const
{
	// RVA 0x8D4EA0
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		assert( previous->parent == this );
		return previous->NextSibling( val );
	}
}


TiXmlNode* TiXmlNode::NextSibling( const CStr& value ) const
{
	// RVA 0x8D4260
	TiXmlNode* node;
	for ( node = next; node; node = node->next )
	{
		if ( node->Value() == value )
			return node;
	}
	return 0;
}


TiXmlNode* TiXmlNode::PreviousSibling( const CStr& value ) const
{
	// RVA 0x8D42A0
	TiXmlNode* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( node->Value() == value )
			return node;
	}
	return 0;
}


void TiXmlElement::RemoveAttribute( const CStr& name )
{
	// RVA 0x8D4EF0
	TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
	{
		attributeSet.Remove( node );
		delete node;
	}
}


TiXmlElement* TiXmlNode::FirstChildElement() const
{
	// RVA 0x8D3BE0
	TiXmlNode* node;

	for (	node = FirstChild();
	node;
	node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


TiXmlElement* TiXmlNode::FirstChildElement( const CStr& value ) const
{
	// RVA 0x8D42E0
	TiXmlNode* node;

	for (	node = FirstChild( value );
	node;
	node = node->NextSibling( value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


TiXmlElement* TiXmlNode::NextSiblingElement() const
{
	// RVA 0x8D3C10
	TiXmlNode* node;

	for (	node = NextSibling();
	node;
	node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


TiXmlElement* TiXmlNode::NextSiblingElement( const CStr& value ) const
{
	// RVA 0x8D4340
	TiXmlNode* node;

	for (	node = NextSibling( value );
	node;
	node = node->NextSibling( value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}



TiXmlDocument* TiXmlNode::GetDocument() const
{
	// RVA 0x8D3C40
	const TiXmlNode* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}


TiXmlElement::TiXmlElement( const CStr& _value )
	: TiXmlNode( TiXmlNode::ELEMENT )
{
	// RVA 0x8D4F20
	firstChild = lastChild = 0;
	value = _value;
}


TiXmlElement::~TiXmlElement()
{
	// RVA 0x8D4F90
	while( attributeSet.First() )
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}


const CStr* TiXmlElement::Attribute( const CStr& name ) const
{
	// RVA 0x8D5060
	TiXmlAttribute* node = attributeSet.Find( name );

	if ( node )
		return &(node->Value() );

	return 0;
}


const CStr* TiXmlElement::Attribute( const CStr& name, int* i ) const
{
	// RVA 0x8D5080
	const CStr* s = Attribute( name );
	if ( s )
		*i = atoi( s->c_str() );
	else
		*i = 0;
	return s;
}


void TiXmlElement::SetAttribute( const CStr& name, int val )
{
	// RVA 0x8D6300
	char buf[64];
	sprintf( buf, "%d", val );

	CStr v = buf;

	SetAttribute( name, v );
}


void TiXmlElement::SetAttribute( const CStr& name, const CStr& value )
{
	// RVA 0x8D6120
	TiXmlAttribute* node = attributeSet.Find( name );
	if ( node )
	{
		node->SetValue( value );
		return;
	}

	TiXmlAttribute* attrib = new TiXmlAttribute( name, value );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		TiXmlDocument* document = GetDocument();
		if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY );
	}
}


TiXmlAttribute* TiXmlElement::FindAttribute( const char* name ) const
{
	// RVA 0x749500
	return attributeSet.Find( CStr( name ) );
}


void TiXmlElement::Print( m3d::fs::IStream* cfile, int depth ) const
{
	// RVA 0x8D50D0
	// NOTE: the formatted printers were only half converted to streams: they still fprintf, handing the IStream to
	// the C runtime as a FILE. Nothing in the game calls them.
	FILE* const file = reinterpret_cast<FILE*>( cfile );
	int i;
	for ( i=0; i<depth; i++ )
	{
		fprintf( file, "    " );
	}

	fprintf( file, "<%s", value.c_str() );

	TiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( file, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if ( !firstChild )
	{
		fprintf( file, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( file, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( file, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( file, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( file, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( file, "\n" );
		for( i=0; i<depth; ++i )
			fprintf( file, "    " );
		fprintf( file, "</%s>", value.c_str() );
	}
}


void TiXmlElement::StreamOut( m3d::fs::IStream* stream, int depth ) const
{
	// RVA 0x8D5260 - one line per element and per attribute, indented with tabs. Once a text child has been
	// written the closing tag follows it directly, otherwise it goes on its own indented line.
	WriteString( stream, NewLine( depth ) + CStr( "<" ) + value );

	TiXmlAttribute* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		attrib->StreamOut( stream, depth + 1 );
	}

	// If this node has children, give it a closing tag. Else
	// make it an empty tag.
	if ( firstChild )
	{
		WriteString( stream, CStr( ">" ) );

		bool text = false;
		for ( TiXmlNode* node = firstChild; node; node = node->NextSibling() )
		{
			if ( text || node->Type() == TEXT )
				text = true;
			node->StreamOut( stream, depth + 1 );
		}
		if ( text )
			WriteString( stream, CStr( "</" ) + value + CStr( ">" ) );
		else
			WriteString( stream, CStr( '\t', depth ) + CStr( "</" ) + value + CStr( ">" ) );
	}
	else
	{
		WriteString( stream, CStr( " />" ) );
	}
	WriteString( stream, CStr( "\n" ) );
}


TiXmlNode* TiXmlElement::Clone() const
{
	// RVA 0x8D6210
	TiXmlElement* clone = new TiXmlElement( Value() );

	if ( !clone )
		return 0;

	CopyToClone( clone );

	// Clone the attributes, then clone the children.
	TiXmlAttribute* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		clone->SetAttribute( attribute->Name(), attribute->Value() );
	}

	TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		clone->LinkEndChild( node->Clone() );
	}
	return clone;
}


TiXmlDocument::TiXmlDocument() : TiXmlNode( TiXmlNode::DOCUMENT )
{
	// RVA 0x8D43A0
	error = false;
}


TiXmlDocument::TiXmlDocument( const CStr& documentName ) : TiXmlNode( TiXmlNode::DOCUMENT )
{
	// RVA 0x8D43E0
	value = documentName;
	error = false;
}


bool TiXmlDocument::LoadFile()
{
	// RVA 0x8D5A50
	return LoadFile( value );
}


bool TiXmlDocument::SaveFile() const
{
	// RVA 0x8D4430
	return SaveFile( value );
}


bool TiXmlDocument::LoadFile( const CStr& filename )
{
	// RVA 0x8D4450 - reads the file through the C runtime, not the file server.
	// Delete the existing data:
	Clear();
	value = filename;

	FILE* file = fopen( filename.c_str(), "r" );

	if ( file )
	{
		// NOTE: the length is measured but not used, so an empty file goes on to fail as an empty document.
		fseek( file, 0, SEEK_END );
		ftell( file );
		fseek( file, 0, SEEK_SET );

		// If we have a file, assume it is all one big XML file, and read it in.
		// The document parser may decide the document ends sooner than the entire file, however.
		CStr data;

		const int BUF_SIZE = 2048;
		char buf[BUF_SIZE];

		while( fgets( buf, BUF_SIZE, file ) )
		{
			data += CStr( buf );
		}
		fclose( file );

		Parse( data.c_str() );
		if (  !Error() )
		{
			return true;
		}
	}
	SetError( TIXML_ERROR_OPENING_FILE );
	return false;
}


bool TiXmlDocument::SaveFile( const CStr& filename ) const
{
	// RVA 0x8D3CA0 - saving is not supported.
	assert( 0 );
	return false;
}


TiXmlNode* TiXmlDocument::Clone() const
{
	// RVA 0x8D45E0 - the error id is not copied.
	TiXmlDocument* clone = new TiXmlDocument();
	if ( !clone )
		return 0;

	CopyToClone( clone );
	clone->error = error;
	clone->errorDesc = errorDesc;

	TiXmlNode* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		clone->LinkEndChild( node->Clone() );
	}
	return clone;
}


void TiXmlDocument::Print( m3d::fs::IStream* cfile, int depth ) const
{
	// RVA 0x8D3CC0
	// NOTE: fprintf is handed the IStream as a FILE; see TiXmlElement::Print.
	TiXmlNode* node;
	for ( node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( reinterpret_cast<FILE*>( cfile ), "\n" );
	}
}


void TiXmlDocument::StreamOut( m3d::fs::IStream* out, int ) const
{
	// RVA 0x8D3D00
	TiXmlNode* node;
	for ( node=FirstChild(); node; node=node->NextSibling() )
	{
		node->StreamOut( out, 0 );

		// Special rule for streams: stop after the root element.
		// The stream in code will only read one element, so don't
		// write more than one.
		if ( node->ToElement() )
			break;
	}
}


TiXmlAttribute* TiXmlAttribute::Next() const
{
	// RVA 0x8D4690
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}


TiXmlAttribute* TiXmlAttribute::Previous() const
{
	// RVA 0x8D46E0
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}


void TiXmlAttribute::Print( m3d::fs::IStream* cfile, int depth ) const
{
	// RVA 0x8D3D30
	StreamOut( cfile, depth );
}


void TiXmlAttribute::StreamOut( m3d::fs::IStream* stream, int depth ) const
{
	// RVA 0x8D5A60 - each attribute goes on its own line, indented by depth tabs.
	WriteString( stream, NewLine( depth ) );
	PutString( name, stream );
	if ( value.empty() )
	{
		stream->WriteBytes( "=\"\"", strlen( "=\"\"" ) );
	}
	else if ( value.find( '\"' ) != CStr_npos )
	{
		WriteString( stream, CStr( "='" ) );
		PutString( value, stream );
		WriteString( stream, CStr( "'" ) );
	}
	else
	{
		WriteString( stream, CStr( "=\"" ) );
		PutString( value, stream );
		WriteString( stream, CStr( "\"" ) );
	}
}


void TiXmlAttribute::SetIntValue( int value )
{
	// RVA 0x8D4730
	SetValue( CStr( CStr( value ).c_str() ) );
}


void TiXmlAttribute::SetDoubleValue( double value )
{
	// RVA 0x8D47A0 - the value is formatted as a float.
	SetValue( CStr( CStr( static_cast<float>( value ) ).c_str() ) );
}


const int TiXmlAttribute::IntValue() const
{
	// RVA 0x8D4810
	return atoi( value.c_str() );
}


const double TiXmlAttribute::DoubleValue() const
{
	// RVA 0x8D4820
	return atof( value.c_str() );
}


void TiXmlComment::Print( m3d::fs::IStream* cfile, int depth ) const
{
	// RVA 0x8D3D40
	// NOTE: fprintf is handed the IStream as a FILE; see TiXmlElement::Print.
	for ( int i=0; i<depth; i++ )
	{
		fprintf( reinterpret_cast<FILE*>( cfile ), "    " );
	}
	StreamOut( cfile, depth );
}


void TiXmlComment::StreamOut( m3d::fs::IStream* stream, int ) const
{
	// RVA 0x8D4830
	WriteString( stream, CStr( "<!--" ) );
	PutString( value, stream );
	WriteString( stream, CStr( "-->" ) );
}


TiXmlNode* TiXmlComment::Clone() const
{
	// RVA 0x8D5CF0
	TiXmlComment* clone = new TiXmlComment();

	if ( !clone )
		return 0;

	CopyToClone( clone );
	return clone;
}


void TiXmlText::Print( m3d::fs::IStream* cfile, int depth ) const
{
	// RVA 0x8D3D80
	StreamOut( cfile, depth );
}


void TiXmlText::StreamOut( m3d::fs::IStream* stream, int ) const
{
	// RVA 0x8D4900
	PutString( value, stream );
}


TiXmlNode* TiXmlText::Clone() const
{
	// RVA 0x8D5D50
	TiXmlText* clone = 0;
	clone = new TiXmlText( "" );

	if ( !clone )
		return 0;

	CopyToClone( clone );
	return clone;
}


TiXmlDeclaration::TiXmlDeclaration( const CStr& _version,
									const CStr& _encoding,
									const CStr& _standalone )
	: TiXmlNode( TiXmlNode::DECLARATION )
{
	// RVA 0x8D4910
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


void TiXmlDeclaration::Print( m3d::fs::IStream* cfile, int depth ) const
{
	// RVA 0x8D3D90
	StreamOut( cfile, depth );
}


void TiXmlDeclaration::StreamOut( m3d::fs::IStream* stream, int ) const
{
	// RVA 0x8D4990
	WriteString( stream, CStr( "<?xml " ) );

	if ( !version.empty() )
	{
		WriteString( stream, CStr( "version=\"" ) );
		PutString( version, stream );
		WriteString( stream, CStr( "\" " ) );
	}
	if ( !encoding.empty() )
	{
		WriteString( stream, CStr( "encoding=\"" ) );
		PutString( encoding, stream );
		WriteString( stream, CStr( "\" " ) );
	}
	if ( !standalone.empty() )
	{
		WriteString( stream, CStr( "standalone=\"" ) );
		PutString( standalone, stream );
		WriteString( stream, CStr( "\" " ) );
	}
	WriteString( stream, CStr( "?>" ) );
}


TiXmlNode* TiXmlDeclaration::Clone() const
{
	// RVA 0x8D5E10
	TiXmlDeclaration* clone = new TiXmlDeclaration();

	if ( !clone )
		return 0;

	CopyToClone( clone );
	clone->version = version;
	clone->encoding = encoding;
	clone->standalone = standalone;
	return clone;
}


void TiXmlUnknown::Print( m3d::fs::IStream*, int ) const
{
	// RVA 0x8D3DA0 - prints nothing.
}


void TiXmlUnknown::StreamOut( m3d::fs::IStream* stream, int ) const
{
	// RVA 0x8D5ED0
	WriteString( stream, CStr( "<" ) + value + CStr( ">" ) );		// Don't use entities hear! It is unknown.
}


TiXmlNode* TiXmlUnknown::Clone() const
{
	// RVA 0x8D6040
	TiXmlUnknown* clone = new TiXmlUnknown();

	if ( !clone )
		return 0;

	CopyToClone( clone );
	return clone;
}


TiXmlAttributeSet::TiXmlAttributeSet()
{
	// RVA 0x8D4CF0
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


TiXmlAttributeSet::~TiXmlAttributeSet()
{
	// RVA 0x8D4D20
	assert( sentinel.next == &sentinel );
	assert( sentinel.prev == &sentinel );
}


void TiXmlAttributeSet::Add( TiXmlAttribute* addMe )
{
	// RVA 0x8D60A0
	assert( !Find( addMe->Name() ) );	// Shouldn't be multiply adding to the set.

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void TiXmlAttributeSet::Remove( TiXmlAttribute* removeMe )
{
	// RVA 0x8D3DB0
	TiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert( 0 );		// we tried to remove a non-linked attribute.
}


TiXmlAttribute*	TiXmlAttributeSet::Find( const CStr& name ) const
{
	// RVA 0x8D4D70
	TiXmlAttribute* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->Name() == name )
			return node;
	}
	return 0;
}
