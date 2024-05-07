#ifndef _SET_H
#define _SET_H

#include <iostream>
#include <string>
#include <initializer_list>
#include <array>
#include <cstdlib>
#include <ctime>
using namespace std;

/**
 * @namespace Set
 * @brief Namespace for managing a set of cards.
 *
 * The `Set` namespace provides classes and functions for managing a set of cards.
 * It includes classes for handling exceptions, defining card characteristics,
 * converting characteristics to strings, printing possible values of characteristics,
 * creating and manipulating cards, managing a deck of cards, and controlling the game.
 */
namespace Set {
// classe pour g�rer les exceptions dans le set
class SetException {
public:
    SetException(const string& i) :info(i) {}
    string getInfo() const { return info; }
private:
    string info;
};

// caract�ristiques
enum class Couleur { rouge, mauve, vert };
enum class Nombre { un=1, deux=2, trois=3 };
enum class Forme { ovale, vague, losange };
enum class Remplissage { plein, vide, hachure };

// conversion d'une caract�ristique en string
string toString(Couleur c);
string toString(Nombre v);
string toString(Forme f);
string toString(Remplissage v);

// �criture d'une caract�ristique sur un flux ostream
ostream& operator<<(ostream& f, Couleur c);
ostream& operator<<(ostream& f, Nombre v);
ostream& operator<<(ostream& f, Forme x);
ostream& operator<<(ostream& f, Remplissage r);

// listes contenant les valeurs possibles pour chacune des caract�ristiques
extern std::initializer_list<Couleur> Couleurs;
extern std::initializer_list<Nombre> Nombres;
extern std::initializer_list<Forme> Formes;
extern std::initializer_list<Remplissage> Remplissages;

// affichage des valeurs possibles pour chaque caract�ristiques
void printCouleurs(std::ostream& f = cout);
void printNombres(std::ostream& f = cout);
void printFormes(std::ostream& f = cout);
void printRemplissages(std::ostream& f = cout);

class Carte {
public:
    Couleur getCouleur() const { return couleur; }
    Nombre getNombre() const { return nombre; }
    Forme getForme() const { return forme; }
    Remplissage getRemplissage() const { return remplissage; }
    ~Carte() = default; // optionnel
    Carte(const Carte& c) = default; // optionnel
    Carte& operator=(const Carte& c) = default; // optionnel
private:
    Carte(Couleur c, Nombre v, Forme f, Remplissage r) :couleur(c), nombre(v), forme(f), remplissage(r) {}
    friend class Jeu;
    Couleur couleur;
    Nombre nombre;
    Forme forme;
    Remplissage remplissage;
};

ostream& operator<<(ostream& f, const Carte& c);

class Jeu {
public:
    static Jeu& getInstance() { if (handler.instance == nullptr) handler.instance = new Jeu; return *handler.instance; }
    static void libererInstance() { delete handler.instance; handler.instance = nullptr; }
    size_t getNbCartes() const { return 81; }
    void print(std::ostream& f=cout) const;

    class Iterator {
    public:
        void next() { if (isDone()) throw SetException("Iterateur en fin de sequence"); i++; }
        bool isDone() const { return i == Jeu::getInstance().getNbCartes();  }
        const Carte& currentItem() const { if (isDone()) throw SetException("Iterateur en fin de sequence"); return Jeu::getInstance().getCarte(i); }
    private:
        size_t i = 0;
        friend class Jeu;
        Iterator() = default;
    };
    Iterator getIterator() const { return Iterator(); }


    class FormeIterator {
    public:
        void next() {
            if (isDone()) throw SetException("Iterateur en fin de sequence");
            i++;
            while (!isDone() && Jeu::getInstance().getCarte(i).getForme() != forme) i++;

        }
        bool isDone() const { return i == Jeu::getInstance().getNbCartes(); }
        const Carte& currentItem() const { if (isDone()) throw SetException("Iterateur en fin de sequence"); return Jeu::getInstance().getCarte(i); }
    private:
        size_t i = 0;
        friend class Jeu;
        Forme forme;
        FormeIterator(Forme f) :forme(f) {
            while (!isDone() && (Jeu::getInstance().getCarte(i).getForme() != forme)) {
                i++;
            }
        }
    };
    FormeIterator getIterator(Forme f) const { return FormeIterator(f); }

private:
    friend class Iterator;
    friend class FormeIterator;
    const Carte& getCarte(size_t i) const { if (i >= 81) throw SetException("Carte invalide");  return *cartes[i]; }

    struct Handler {
        Jeu* instance;
        Handler() :instance(nullptr) {} // pointeur initialis� � la cr�ation de l'objet Handler
        ~Handler() { delete instance; } // l'instance est lib�r�e au moment de la destruction (automatique) du Handler
    };
    static Handler handler;
    Jeu();
    ~Jeu();
    Jeu(const Jeu& j) = delete;
    Jeu& operator=(const Jeu& j) = delete;
    const Carte* cartes[81];
};

ostream& operator<<(ostream& f, const Jeu& j);

class Pioche {
    // d�signe un paquet de cartes on l'on ne peut que piocher : prendre une carte au hasard
public:
    Pioche();
    bool estVide() const { return nb == 0; }
    size_t getNbCartes() const { return nb; }
    const Carte& piocher();
    ~Pioche();
    Pioche(const Pioche& p) = delete;
    Pioche& operator=(const Pioche& p) = delete;
private:
    const Carte** cartes = nullptr;
    size_t nb = 0;
};

class Plateau {
public:
    Plateau() = default;
    ~Plateau() { delete[] cartes; }
    size_t getNbCartes() const { return nb; }
    void ajouter(const Carte& c);
    void retirer(const Carte& c);
    void print(ostream& f = cout) const;
    Plateau(const Plateau& p);
    Plateau& operator=(const Plateau& p);
    class const_iterator {
    public:
        const_iterator & operator++() { current++;  return *this; }
        const Carte& operator*() const { return **current; }
        bool operator!=(const_iterator it) const { return current != it.current; }
    private:
        const_iterator(const Carte** c) :current(c) {}
        friend class Plateau;
        const Carte** current = nullptr;
    };
    const_iterator begin() const { return const_iterator(cartes); }
    const_iterator end() const { return const_iterator(cartes+nb); }

private:
    const Carte** cartes = nullptr;
    size_t nbMax = 0;
    size_t nb = 0;
};

ostream& operator<<(ostream& f, const Plateau& m);

class Combinaison {
public:
    Combinaison(const Carte& u, const Carte& d, const Carte& t) :c1(&u), c2(&d), c3(&t) {}
    bool estUnSET() const;
    const Carte& getCarte1() const { return *c1; }
    const Carte& getCarte2() const { return *c1; }
    const Carte& getCarte3() const { return *c1; }
    ~Combinaison() = default;
    Combinaison(const Combinaison& c) = default;
    Combinaison& operator=(const Combinaison& c) = default;
private:
    const Carte* c1;
    const Carte* c2;
    const Carte* c3;
};

ostream& operator<<(ostream& f, const Combinaison& c);

class Controleur {
public:
    Controleur();
    void distribuer();
    ~Controleur() { delete pioche; }
    const Plateau& getPlateau() const { return plateau; }
    Plateau& getPlateau() { return plateau; }
    const Pioche& getPioche() const { return *pioche; }
    Controleur(const Controleur& c) = delete;
    Controleur& operator=(const Controleur& c) = delete;
private:
    Pioche* pioche=nullptr;
    Plateau plateau;
};
}
#endif
