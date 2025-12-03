#include <iostream>
#include "Box.h"
#include "Symbol.h"
#include "Shape.h"

namespace Netlist{
    
    class BoxShape : public Shape{
        
        private:
            Box     box_;

        public:
                            BoxShape        ( Symbol*, const Box& );
                            BoxShape        ( Symbol *, int x1 , int y1 , int x2 , int y2 );
                           ~BoxShape        ();
                    Box     getBoundingBox  () const;
                    void    toXml           (std::ostream&);
            static  Shape*    fromXml         (Symbol*, xmlTextReaderPtr);
    };

}