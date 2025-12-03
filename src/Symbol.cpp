#include "Symbol.h"
#include "Cell.h"
#include "Term.h"
#include "Shape.h"
#include "TermShape.h"

namespace Netlist
{
    Symbol::Symbol(Cell *cell)
        : owner_(cell), shapes_() {}

    Symbol::~Symbol()
    {
        //  for (Shape* shape : shapes_) {
        //    delete shape;
        //  }
    }

    Cell *Symbol::getCell() const { return owner_; }

    Box Symbol::getBoundingBox() const
    {
        Box box(0, 0, 0, 0);
        for (std::vector<Shape *>::const_iterator ishape = shapes_.begin(); ishape != shapes_.end(); ishape++)
        {
            // on fusionne toutes les box pour obtenir une box englobant le symbole
            box.merge((*ishape)->getBoundingBox());
        }
        return box;
    }

    Point Symbol::getTermPosition(Term *term) const
    {
        return term->getPosition();
    }

    TermShape *Symbol::getTermShape(Term *term) const
    {
        for (std::vector<Shape *>::const_iterator ishape = shapes_.begin(); ishape != shapes_.end(); ishape++)
        {
            TermShape *iterm = dynamic_cast<TermShape *>(*ishape);
            // if (typeid(*ishape).name() == std::string("TermShape")){
            if (iterm != nullptr)
            {
                // marche
                //  on vérifie s'ils possèdent le même nom
                if (iterm->getTerm()->getName() == term->getName())
                    return iterm;
            }
        }
        return nullptr;
    }

    void Symbol::add(Shape *shape)
    {
        //Symbol* s = owner_->getSymbol();
        if (owner_->getSymbol() != this) {
            owner_->getSymbol()->add(shape);
        }else{
            shapes_.push_back(shape);
        }
        return;
  }
  
    void Symbol::remove(Shape *shape)
    {
        for (std::vector<Shape *>::iterator ishape = shapes_.begin(); ishape != shapes_.end(); ++ishape)
        {
            if (*ishape == shape)
                shapes_.erase(ishape);
        }
    }

    void Symbol::toXml(std::ostream &stream) const{
        stream << indent << "<symbol>\n";
        indent++;
        for (Shape *s : shapes_)
        {
            s->toXml(stream);
        }
        stream << --indent << "</symbol>\n";
    }
    
    Symbol *Symbol::fromXml(Cell *cell, xmlTextReaderPtr reader)
    {

        const xmlChar *symbolTag = xmlTextReaderConstString(reader, (const xmlChar *)"symbol");
        const xmlChar *nodeName = xmlTextReaderConstLocalName(reader);

        Symbol *symbol = new Symbol(cell);

        while (true)
        {
            int status = xmlTextReaderRead(reader);

            // Detects error of reading
            if (status != 1)
            {
                if (status != 0)
                {
                    std::cerr << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << std::endl;
                }
                else
                    std::cerr << "[ERROR] Symbol::fromXml(): </symbol> is missing but parser ended" << std::endl;
                break;
            }

            // Ignore comments and whitespaces
            switch (xmlTextReaderNodeType(reader)){
                case XML_READER_TYPE_COMMENT:
                case XML_READER_TYPE_WHITESPACE:
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                    continue;
            }
            nodeName = xmlTextReaderConstLocalName(reader);
            if (nodeName == symbolTag && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT))
            break;
            else if (Shape::fromXml(symbol, reader))
            continue;
            std::cerr << "[ERROR] Symbol::fromXml(): Unknown or misplaced tag <" << nodeName
           << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            break;
        }

        return symbol;
    }

}