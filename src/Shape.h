#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <libxml/xmlreader.h>
#include "Box.h"

namespace Netlist{

  class Symbol;

  class Shape {

    public:
                Shape                     ( Symbol * );
        virtual ~Shape                    ();
        inline  Symbol*   getSymbol       () const ;
        virtual Box       getBoundingBox  () const = 0;
        virtual void      toXml           (std::ostream&) = 0;
        static  Shape*    fromXml         (Symbol*, xmlTextReaderPtr);

    private :
        Symbol * owner_ ;
  };

  inline Symbol * Shape :: getSymbol () const { return owner_ ; };

}

#endif