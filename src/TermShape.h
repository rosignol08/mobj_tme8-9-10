#include <iostream>
#include "Box.h"
#include "Symbol.h"
#include "Shape.h"

namespace Netlist{

  enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight };

  class TermShape : public Shape {
    public :
                        TermShape       ( Symbol* owner, std::string name, int x , int y, NameAlign name_align = TopLeft);
                       ~TermShape       ();
              Box       getBoundingBox  () const ;
      inline  Term *    getTerm         () const ;
      inline  int       getX            () const ;
      inline  int       getY            () const ;
      inline  NameAlign getAlign        () const ;
              void      toXml           (std::ostream&);
      static  Shape*    fromXml         (Symbol*, xmlTextReaderPtr);

    private :
      Term * term_ ;
      NameAlign align_;
      int x_ , y_ ;
  };
  inline Term* TermShape::getTerm() const { return term_; }
  inline int TermShape::getX() const { return x_; }
  inline int TermShape::getY() const { return y_; }
  inline NameAlign TermShape::getAlign() const { return align_; };

}
