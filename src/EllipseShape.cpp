#include "EllipseShape.h"
#include "Box.h"
#include "Symbol.h"
#include "XmlUtil.h"

namespace Netlist{

	EllipseShape::EllipseShape ( Symbol * owner , const Box & box )
	: Shape ( owner ) , box_ ( box )
	{ }

	EllipseShape::EllipseShape ( Symbol * owner , int x1 , int y1 , int x2 , int y2 )
	: Shape ( owner ), box_ ( x1 ,y1 ,x2 , y2 )
	{ }

	EllipseShape::~EllipseShape ()
	{ }

	Box EllipseShape::getBoundingBox () const
	{ return box_ ; }

	void EllipseShape::toXml(std::ostream& stream) {
		stream 	<< indent << "<ellipse x1=\"" << box_.getX1() << "\""
				<< " y1=\"" << box_.getY1() << "\""
				<< " x2=\"" << box_.getX2() << "\""
				<< " y2=\"" << box_.getY2() << "\"/>" << std::endl;
	}

	Shape* EllipseShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) {
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		xmlGetIntAttribute(reader, "x1", x1);
		xmlGetIntAttribute(reader, "y1", y1);
		xmlGetIntAttribute(reader, "x2", x2);
		xmlGetIntAttribute(reader, "y2", y2);
		return new EllipseShape(owner, x1, y1, x2, y2);
	}
}