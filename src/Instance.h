#ifndef INSTANCE_H
#define INSTANCE_H

#pragma once
#include "Point.h"
#include "Term.h"
#include "Cell.h"
#include "Shape.h"

namespace Netlist {

    class Instance{
        private:
            Cell*                       owner_;
            Cell*                       masterCell_;
            std::string                 name_;
            std::vector<Term*>          terms_;
            Point                       position_;

        public:
                                        Instance        ( Cell* owner, Cell* model, const std::string& );
                                       ~Instance        ();

            // Accesseurs
            const std::string&          getName         () const;
            Cell*                       getMasterCell   () const;
            Cell*                       getCell         () const;
            const std::vector<Term*>&   getTerms        () const;
            Term*                       getTerm         (const std::string&) const;
            Point                       getPosition     () const;

            // Modifieurs
            bool                        connect         (const std::string& name, Net*);
            void                        add             (Term*);
            void                        remove          (Term*);
            void                        setPosition     (const Point&);
            void                        setPosition     (int x, int y);

            // Fonctions XML
            void                        toXml           (std::ostream&);
    static  Instance*                   fromXml         (Cell* cell, xmlTextReaderPtr reader ); // renvoie NULL en cas d'erreur
    };
}

#endif