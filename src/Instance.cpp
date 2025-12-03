#include "Instance.h"
#include "Term.h"
#include "TermShape.h"
#include <algorithm>

using namespace std;
using namespace Netlist;

namespace Netlist{

    //constructeur : doit gerer l'ajout de l'instance au niveau de la cell
    Instance::Instance ( Cell* owner, Cell* model, const std::string& name) 
        : owner_(owner), masterCell_(model), name_(name), position_(0,0) {
            //ajout de l'instance à la cell owner
            if(owner_ != nullptr){
                owner_->add(this);
            }
            //duplication des terminaux de la cell model
            if(masterCell_ != nullptr){
                vector<Term*> modelTerms = masterCell_->getTerms();
                for(const Term* term : modelTerms){
                    new Term(this, term);
                }
            }
        }

    //destructeur : doit gerer le retrait de l'instance au niveau de la cell
    Instance::~Instance (){
        //retrait de l'instance de la cell owner
        if(owner_ != nullptr){
            owner_->remove(this);
        }
        //destruction des terminaux
        for(Netlist::Term* term : terms_){
            delete term; //psk on utilise new dans le constructeur
        }
        terms_.clear();
    }

    const std::string& Instance::getName() const {
        return name_;
    }

    //renvoie la cellule maitre
    Cell*                     Instance::getMasterCell () const{
        return this->masterCell_;
    }

    //renvoie la cellule owner_ ? TODO je suis pas sûr
    Cell*                     Instance::getCell       () const{
        return this->owner_;
    }

    //renvoie le vecteur des terminaux
    const std::vector<Term*>& Instance::getTerms      () const{
        return this->terms_;
    }

    //renvoie le terminal de nom name s'il existe
    Term* Instance::getTerm       (const std::string& name) const{
        for (Term* term : terms_) {//parcourt les term du vecteur et on check si celui avec ce nom donné existe
            if (term->getName() == name) {
                return term;
            }
        }
        return nullptr;
    }

    //renvoie la position
    Point                     Instance::getPosition   () const{
        return this->position_;
    }

    //associe le net au terminal de nom name s'il existe
    bool Instance::connect (const std::string & name, Net* net){
        Term* term = this->getTerm(name);
        if(term != nullptr){//il existe on l'associe
            term->setNet(net);
            return true;
        }else{
            return false;
        }
    };

    //ajoute un terminal au vecteur
    void    Instance::add             (Term *terminal){
        this->terms_.push_back(terminal);
        return;
    };

    void    Instance::remove          (Term *terminal){
        auto it = std::find(this->terms_.begin(), this->terms_.end(), terminal);
        if (it != this->terms_.end()) {
            this->terms_.erase(it);
        }
        return;
    };

    void    Instance::setPosition     (int x, int y){
        this->position_.setX(x);
        this->position_.setY(y);

        Symbol* symbol = masterCell_->getSymbol();
        if (symbol) {
            for (Term* term : terms_) {
                TermShape* tshape = symbol->getTermShape(term);
                if (tshape) {
                    term->setPosition(tshape->getX() + x, tshape->getY() + y);
                }
            }
        }
        //for ( vector<Term*>::iterator iterm=terms_.begin() ; iterm != terms_.end(); iterm++){
        //    TermShape* tshape = symbol->getTermShape();
        //    iterm->setPosition(tshape->getX() + x, tshape->getY() + y);
        //}
        return;
    };

    void    Instance::setPosition     (const Point &point){
        setPosition(point.getX(), point.getY());
        return;
    };

    void Instance::toXml(ostream& stream){
        stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"";
        if (masterCell_) stream << masterCell_->getName();
        else stream << "None";
        stream << "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
    }

    Instance* Instance::fromXml (Cell* cell, xmlTextReaderPtr reader ) {

        const xmlChar* instanceTag = xmlTextReaderConstString( reader, (const xmlChar*)"instance" );
        Instance* inst = NULL;

        // Lit et crée l'objet Instance
        const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

        if(instanceTag == nodeName){
            string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
            string masterCellName = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ) );
            string instx = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x"));
            string insty = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y"));

            Cell* mastercell = Cell::find(masterCellName);
            int x = atoi(instx.c_str());
            int y = atoi(insty.c_str());
            
            if (!name.empty() && mastercell) {
                inst = new Instance ( cell, mastercell, name );
                inst->setPosition(x, y);
            }
            else cerr << "[ERROR] Instance::fromXml(): Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
        }

        return inst;
    }
}