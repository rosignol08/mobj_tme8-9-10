#include "TermShape.h"
#include "Symbol.h"
#include "Cell.h"
#include "Term.h"
#include "XmlUtil.h"
#include <string>

namespace Netlist{

  TermShape::TermShape ( Symbol * owner , std::string name , int x , int y , NameAlign name_align )
    : Shape ( owner ) , term_ ( NULL ), align_ ( name_align ), x_ ( x ), y_ ( y )
  {
    Cell * cell = owner->getCell();
    term_ = cell->getTerm ( name );
  }

  TermShape::~TermShape () { }

  Box TermShape::getBoundingBox () const
  { return Box ( x_ , y_ , x_ , y_ ); }

  void TermShape::toXml(std::ostream& stream) {
		stream 	<< indent << "<term name=\"" << term_->getName() << "\""
				<< " x1=\"" << x_ << "\""
				<< " y1=\"" << y_ << "\""
				<< " align=\"";
				if (align_ == Netlist::TopRight )   {stream << "top_right";}
				if (align_ == Netlist::TopLeft)     {stream << "top_left"; }
				if (align_ == Netlist::BottomLeft)  {stream << "bottom_left"; }
				if (align_ == Netlist::BottomRight) {stream << "bottom_right"; } 
				stream << "\"/>" << std::endl;
	}

	Shape* TermShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) {
		NameAlign name_align = TopLeft;
		int x1 = 0, y1 = 0;//, x2 = 0, y2 = 0;
		std::string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
		xmlGetIntAttribute(reader, "x1", x1);
		xmlGetIntAttribute(reader, "y1", y1);
		//xmlGetIntAttribute(reader, "x2", x2);
		//xmlGetIntAttribute(reader, "y2", y2);
		std::string align = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ) );
  
		if      (align == "top_right")    { name_align = TopRight; }
		else if (align == "top_left")     { name_align = TopLeft; }
		else if (align == "bottom_left")  { name_align = BottomLeft; }
		else if (align == "bottom_right") { name_align = BottomRight; }

		return new TermShape(owner, name, x1, y1, name_align);
	}
}