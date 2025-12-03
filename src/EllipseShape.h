#include <iostream>
#include "Box.h"
#include "Symbol.h"
#include "Shape.h"

namespace Netlist{
    
    class EllipseShape : public Shape{
        
        private:
            Box     box_;

        public:
                            EllipseShape        ( Symbol*, const Box& );
                            EllipseShape        ( Symbol *owner, int x1 , int y1 , int x2 , int y2 );
                           ~EllipseShape        ();
                    Box     getBoundingBox  () const;
                    void    toXml           (std::ostream&);
            static  Shape*    fromXml         (Symbol*, xmlTextReaderPtr);
    };

}