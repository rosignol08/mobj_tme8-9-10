#include "Term.h"
#include "Net.h"
#include "Node.h"

using namespace std;

namespace Netlist{

    Term::Term ( Cell* cell, const std :: string & name , Direction direction)
        :   owner_      (cell), 
            name_       (name),
            direction_  (direction),
            type_       (External),
            net_        (),
            node_       (this, Netlist::Net::noid)
    {
        cell->add(this);
    }

    Term::Term ( Instance* inst, const Term* modelTerm)
        :   owner_      (inst),
            name_       (modelTerm->getName()),
            direction_  (modelTerm->getDirection()),
            type_       (Internal),
            net_        (modelTerm->getNet()),
            node_       (this, (size_t) Net::noid)
    {
        inst->add(this);
    }

    Term::~Term() {
        if (net_ != NULL){
            net_ = NULL;
        }
    }

    //constructeur pour un terminal interne
    bool                    Term::isInternal      ()const{
        if(this->type_ == 1){
            return true;
        }else if(this->type_ == 2)
        {
            return false;
        }
        return false;//par defaut
    }
    bool                    Term::isExternal      ()const{
     if(this->type_ == 2){
            return true;
        }else if(this->type_ == 1)
        {
            return false;
        }
        return false;//par defaut

    }
    const std::string &   Term::getName         () const{
        return this->name_;
    }
    NodeTerm*                  Term::getNode         (){
        return &this->node_;
    }
    Net*                   Term::getNet          ()const{
        return this->net_;
    }
    Cell* Term::getCell () const{
        return (type_ == External) ? static_cast<Cell*>(owner_) : NULL;
    }
    Instance* Term::getInstance () const{
        return (type_ == Internal) ? static_cast<Instance*>(owner_) : NULL;

    }
    Cell* Term::getOwnerCell ()const{
        //getOwnerCell() renvoie la Cell dans laquelle l'objet se trouve, ce qui, dans le cas d'un Term d'instance est la Cell possédant celle-ci.
        if(isExternal()){
            return static_cast<Cell*>(owner_); //obligé de cast
        }
        //sinon il a pas d'owner cell donc on renvoie nullptr
        return nullptr;
    }
    Term::Direction              Term::getDirection    ()const{
        return this->direction_;
    }
    Point                   Term::getPosition     ()const{
        return node_.getPosition();
    }
    Term::Type              Term::getType               () const{
        return this->type_;
    }
    void                    Term::setNet          ( Net * net ){
        net->add(&node_);
        this->net_ = net;
    }
    void                    Term::setNet          ( const std::string& netnom){
        net_ = getCell()->getNet(netnom);
        net_->add(&node_);
    }
    void                    Term::setPosition     ( const Point& point){
        node_.setPosition(point);
    }
    void                    Term::setPosition     ( int x , int y ){
        node_.setPosition(x,y);
    }
    void                    Term::setDirection    ( Direction direction_ ){
        this->direction_ = direction_;
    }

    void Term::toXml( std::ostream& stream){
        stream << indent << "<term name=\"" << name_;
        stream << "\" direction=\"";
        switch(direction_){
            case In:
                stream << "In";
                break;
            case Out:
                stream << "Out";
                break;
            case Inout:
                stream << "Inout";
            case Tristate:
                stream << "Tristate";
                break;
            case Transcv:
                stream << "Transcv";
                break;
            case Unknown:
                stream << "Unknown";
                break;
        }
        stream << "\" x=\"" << node_.getPosition().getX() << "\" y=\"" << node_.getPosition().getY() << "\"/>\n";
    }

    Term* Term::fromXml (Cell* cell, xmlTextReaderPtr reader ) {

        const xmlChar* termTag = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
        Term* term = NULL;

        // Lit et crée l'objet term
        const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
        if(termTag == nodeName){
            string termName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
            string dirString = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );
            Direction dir;
            if (dirString == "In")              dir = In;
            else if (dirString == "Out")        dir = Out;
            else if (dirString == "Inout")      dir = Inout;
            else if (dirString == "Tristate")   dir = Tristate;
            else if (dirString == "Transcv")    dir = Transcv;
            else                                dir = Unknown;
            if (!termName.empty()) term = new Term ( cell, termName, dir );
            else cerr << "[ERROR] Term::fromXml(): Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
            string xpos = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"x"));
            string ypos = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"y"));
            if (!xpos.empty() && !ypos.empty()) {
            term->setPosition(stoi(xpos), stoi(ypos));
            }
        }
        return term;
    }

}