#include <iostream>
#include "Box.h"
#include "Symbol.h"
#include "Shape.h"

namespace Netlist{
    
    class LineShape : public Shape{
        
        private:
            Symbol * owner_ ;
            int x1_ , y1_ , x2_ , y2_ ;

        public:
                            LineShape       ( Symbol *, int x1 , int y1 , int x2 , int y2 );
                           ~LineShape       ();
                    Box     getBoundingBox  () const;
                    void    toXml           (std::ostream&);
            static  Shape*  fromXml         (Symbol*, xmlTextReaderPtr);
                    //TODO jsp si on a le droit de faire ça mais c'est pratique
            inline  int     getX1           () const;
            inline  int     getY1           () const;
            inline  int     getX2           () const;
            inline  int     getY2           () const;
    };

    int LineShape::getX1() const { return x1_; };
    int LineShape::getY1() const { return y1_; };
    int LineShape::getX2() const { return x2_; };
    int LineShape::getY2() const { return y2_; };

}