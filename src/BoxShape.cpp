#include "BoxShape.h"
#include "Box.h"
#include "Symbol.h"
#include "XmlUtil.h"

namespace Netlist{

	BoxShape::BoxShape ( Symbol * owner , const Box & box )
	: Shape ( owner ) , box_ ( box )
	{ }

	BoxShape::BoxShape ( Symbol * owner , int x1 , int y1 , int x2 , int y2 )
	: Shape ( owner ), box_ ( x1 ,y1 ,x2 , y2 )
	{ }

	BoxShape::~BoxShape ()
	{ }

	Box BoxShape::getBoundingBox () const
	{ return box_ ; }

	void BoxShape::toXml(std::ostream& stream) {
		stream 	<< indent << "<box x1=\"" << box_.getX1() << "\""
				<< " y1=\"" << box_.getY1() << "\""
				<< " x2=\"" << box_.getX2() << "\""
				<< " y2=\"" << box_.getY2() << "\"/>" << std::endl;
	}

	Shape* BoxShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) {
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		xmlGetIntAttribute(reader, "x1", x1);
		xmlGetIntAttribute(reader, "y1", y1);
		xmlGetIntAttribute(reader, "x2", x2);
		xmlGetIntAttribute(reader, "y2", y2);
		return new BoxShape(owner, x1, y1, x2, y2);
	}
}