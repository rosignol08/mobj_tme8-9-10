#include <iostream>
#include "Box.h"
#include "Symbol.h"
#include "Shape.h"

namespace Netlist{
    
    class ArcShape : public Shape{
        
        private:
            Box     box_;
            int     start_;
            int     span_;

        public:
                            ArcShape        ( Symbol*, const Box& , int, int);
                            ArcShape        ( Symbol *owner, int x1 , int y1 , int x2 , int y2, int start, int span);
                           ~ArcShape        ();
                    Box     getBoundingBox  () const;
                    void    toXml           (std::ostream&);
            inline  int     getStart        ();
            inline  int     getSpan         ();
            static  Shape*    fromXml         (Symbol*, xmlTextReaderPtr);
    };

  int ArcShape::getStart() {return start_;}
  int ArcShape::getSpan() {return span_;}
}