#include "LineShape.h"
#include "Box.h"
#include "Symbol.h"
#include "XmlUtil.h"

namespace Netlist{

    LineShape::LineShape ( Symbol * owner , int x1 , int y1 , int x2 , int y2 )
    : Shape ( owner ), x1_ ( x1 ), y1_ ( y1 ), x2_ ( x2 ), y2_ ( y2 )
    { }

    LineShape::~LineShape ()
    { }

    Box LineShape::getBoundingBox () const
    { 
        int xmin = std::min(x1_, x2_);
        int ymin = std::min(y1_, y2_);
        int xmax = std::max(x1_, x2_);
        int ymax = std::max(y1_, y2_);
        return Box(xmin, ymin, xmax, ymax);
    }

    void LineShape::toXml(std::ostream& stream) {
        stream  << indent << "<line x1=\"" << x1_ << "\"" << " y1=\"" << y1_ << "\"" << " x2=\"" << x2_ << "\"" << " y2=\"" << y2_ << "\"/>" << std::endl;
    }

    Shape* LineShape::fromXml (Symbol* owner, xmlTextReaderPtr reader) {
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        xmlGetIntAttribute(reader, "x1", x1);
        xmlGetIntAttribute(reader, "y1", y1);
        xmlGetIntAttribute(reader, "x2", x2);
        xmlGetIntAttribute(reader, "y2", y2);
        return new LineShape(owner, x1, y1, x2, y2);
    }
}