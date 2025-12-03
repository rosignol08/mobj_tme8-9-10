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
    };

}