#include "ArcShape.h"
#include "Box.h"
#include "Symbol.h"
#include "XmlUtil.h"

namespace Netlist{

	ArcShape::ArcShape ( Symbol * owner , const Box & box , int start , int span)
	: Shape ( owner ) , box_ ( box ), start_(start), span_(span)
	{ }

	ArcShape::ArcShape  (Symbol *owner, int x1 , int y1 , int x2 , int y2, int start, int span)
	: Shape ( owner ), box_ ( x1 ,y1 ,x2 , y2 ), start_(start), span_(span)
	{ }

	ArcShape::~ArcShape ()
	{ }

	Box ArcShape::getBoundingBox () const
	{ return box_ ; }

	void ArcShape::toXml(std::ostream& stream) {
		stream 	<< indent << "<arc x1=\"" << box_.getX1() << "\""
											<< " y1=\"" << box_.getY1() << "\""
											<< " x2=\"" << box_.getX2() << "\""
											<< " y2=\"" << box_.getY2() << "\""
                			<< " start=\"" << start_ << "\""
                			<< " span=\"" << span_
                			<< "\"/>" << std::endl;
	}

	Shape* ArcShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) {
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0, start = 0, span = 0;
		xmlGetIntAttribute(reader, "x1", x1);
		xmlGetIntAttribute(reader, "y1", y1);
		xmlGetIntAttribute(reader, "x2", x2);
		xmlGetIntAttribute(reader, "y2", y2);
  	xmlGetIntAttribute(reader, "start", start);
  	xmlGetIntAttribute(reader, "span", span);
		return new ArcShape(owner, x1, y1, x2, y2, start, span);
	}
}