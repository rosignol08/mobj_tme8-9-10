#include "Net.h"
#include  <limits>

using namespace std;

namespace Netlist{
    
    const size_t Net::noid = numeric_limits<size_t>::max();

    // Constructeur
    Net::Net ( Cell* cell, const std::string & name , Term::Type dir )
        :   owner_  (cell),
            name_   (name),
            type_   (dir),
            id_     (cell->newNetId()),
            nodes_  ()
    {
        cell->add(this);
    }

    Net::Net ( Instance* inst, const std::string & name , Term::Type dir )
        :   owner_  (inst->getCell()),
            name_   (name),
            type_   (dir),
            id_     (inst->getMasterCell()->newNetId()),
            nodes_  ()
    {
        inst->getMasterCell()->add(this);
    }

    Net::~Net () {

    }

    //retourne le pointeur vers la cellule propriétaire du net
    Cell* Net::getCell()const{
        return this->owner_;
    }

    //retourne le nom du net
    const std::string &Net::getName()const{
        return this->name_;
    }
    
    //retourne l'id du net
    unsigned int Net::getId()const{
        return this->id_;
        
    }
    
    //retourne le type du net
    Term::Type Net::getType()const{
        return this->type_;

    }
    
    //retourne le vecteur de noeuds
    const std::vector<Node*>& Net::getNodes() const{
        return this->nodes_;
    }
    
    Node* Net::getNode (size_t id) const {
        for ( vector<Node*>::const_iterator inode=nodes_.begin(); inode!=nodes_.end(); inode++) {
            if (*inode != nullptr && (*inode)->getId() == id) return *inode;
        }
        
        return nullptr;
    }

    //on doit trouver l'index de la première case libre dans le tableau 
    //si aucune case n'est libre, elle renverra la taille du tableau,
    //c'est à dire l'index situé immédiatement après le dernier élément
    size_t Net::getFreeNodeId() const {
        for (size_t i = 0; i < nodes_.size(); ++i) {
            if (nodes_[i] == nullptr) {
                return i;
            }
        }//pas de case libre return size du tab
        return nodes_.size();
    }

    //ajoute un noeud au net
    void Net::add(Node * node){
        //si ça a dejà un id on l'inserer et redimentionne le vecteur
        if(node->getId() == Netlist::Node::noid || node->getId() == 0){
            size_t id = this->getFreeNodeId();
            node->setId(id); // associe l'id du noeud
            if (id == nodes_.size()){
                this->nodes_.push_back(node);
            }
            else{
                while (id >= nodes_.size() ){
                    this->nodes_.push_back(nullptr);
                }
                this->nodes_[id] = node;
            }
        }else{
            //redimentionement vecteur
            while (node->getId() >= nodes_.size()){
                this->nodes_.push_back(nullptr);
                std::cout << "id noeud : " << node->getId() << std::endl;
                std::cout << "taille vecteur : " << nodes_.size() << std::endl;
                std::cout << "nouvelle capacité : " << nodes_.capacity() << std::endl;
            }
            this->nodes_[node->getId()] = node;
        }
    }

    //enlève un noeud au net
    bool Net::remove(Node * node){
        for(size_t i = 0; i < this->nodes_.size(); i++){
            if(this->nodes_[i] == node){
                this->nodes_.erase(this->nodes_.begin()+i);
                return true;
            }
        }
        return false;
    }

    void  Net::add ( Line* line )
    { if (line) lines_.push_back( line ); }

    bool  Net::remove ( Line* line ) {
        if (line) {
            for ( vector<Line*>::iterator il = lines_.begin()
                ; il != lines_.end() ; ++il ) {
                if (*il == line) {
                    lines_.erase( il );
                    return true;
                }
            }
        }
        return false;
    }

    void Net::toXml (std::ostream& stream){
        stream << indent << "<net name=\"" << name_ << "\" type=\"";
        switch(type_){
            case Term::Type::Internal:
                stream << "Internal";
                break;
            case Term::Type::External:
                stream << "External";
                break;
        }
        stream << "\">\n";
        indent++;
        for (Node* n: nodes_){
            if(n != nullptr)
            {n->toXml(stream);}
        }
        for (Line* l: lines_){
            l->toXml(stream);
        }
        stream << --indent << "</net>\n";
    }

    Net* Net::fromXml(Cell* cell, xmlTextReaderPtr reader){

        const xmlChar* netTag = xmlTextReaderConstString( reader, (const xmlChar*)"net" );
        Net* net = NULL;
        const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
        string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
        string netType = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"type" ) );
        Term::Type type;

        if (!name.empty()) {
            if (netType == "Internal") type = Term::Type::Internal;
            else if (netType == "External") type = Term::Type::External;
            else {
                cerr << "[ERROR] Net::fromXml(): Unexcpected net's type" << endl;
                return net;
            }
            net = new Net(cell, name, type);
        }
        else cerr << "[ERROR] Net::fromXml(): Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

        while(net){
            int status = xmlTextReaderRead(reader);

            if (status != 1){
                if (status != 0) {
                    cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << endl;
                }
                else cerr << "[ERROR] Net::fromXml(): </net> is missing but parser ended" << endl;
                break;
            }
            // Ignore comments and whitespaces
            switch ( xmlTextReaderNodeType(reader) ) {
                case XML_READER_TYPE_COMMENT:
                case XML_READER_TYPE_WHITESPACE:
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                    continue;
            }

            //nodeName = xmlTextReaderConstLocalName( reader );
            if (nodeName == netTag && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)) break;
            else if(Node::fromXml(net, reader)) continue;
            else if(Line::fromXml(net, reader)) continue;
            
            cerr << "[ERROR] Net::fromXml(): Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
            break;
        }

        return net;
    }
}