#ifndef VUEPARTIE_H
#define VUEPARTIE_H

#include <QWidget>
#include <vector>
#include <set>
#include "set.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QProgressBar;
class QLCDNumber;

class VueCarte;

/**
 * @brief The `VuePartie` class represents a widget that displays and manages the components of a game.
 *
 * This class inherits from the `QWidget` class and provides functionality to handle signals and slots.
 * It contains various attributes such as layouts, labels, progress bars, and objects of type `VueCarte`.
 * The `controleur` attribute is used to manage the different components of a game using an object of type `Set::Controleur`.
 * The `selectionCartes` attribute stores the set of selected cards, and the `scoreValue` attribute represents the current score.
 *
 * The `VuePartie` class provides a constructor to initialize the widget and dynamically allocate the required widgets and layouts.
 * It also defines a slot `carteClique` to handle clicks on the cards.
 */
class VuePartie : public QWidget
{
    // La macro Q_Object permet de gérer les signaux et slots dans une classe.
    Q_OBJECT

public:
    explicit VuePartie(QWidget *parent = nullptr);

private:

    /* L'attribut "controleur" permet d'utiliser un objet Set::Controleur
     * pour gérer les différents composants d'une partie.
     */
    Set::Controleur controleur; // controleur de la partie

    /* Les attributs de type QVBoxLayout*, QHBoxLayout*, QGridLayout*, QLCDNumber*, QLabel* et
       QProgressBar* contiendront les adresses des widgets et layouts utilisés sur la fenêtre
       et qui seront alloués dynamiquement dans le constructeur de la classe.
     *
     *
     *
     */
    QLabel* score; // texte "Score"
    QLCDNumber* scoreJoueur; // affichage du score
    QLabel* pioche; // texte "Pioche"
    QProgressBar* nbCartesPioche; // progression de la pioche
    QHBoxLayout* layoutInformations;
    QGridLayout* layoutCartes; // grille des cartes
    QVBoxLayout* couche;

    /* L'attribut "vuecartes" contiendra les adresses de composants VueCarte
     * dont la définition vous est fournie en ressource.
     */
    vector<VueCarte*> vuecartes; // adresses des objets VueCarte

    std::set<const Set::Carte*> selectionCartes; // ensemble des cartes sélectionnées
    int scoreValue=0; // score courant (nb de sets trouvés)

private slots:
    // slots qui gère les clics sur les cartes
    void carteClique(VueCarte* vc);
}; // FIN de class VuePartie : public QWidget


#endif // VUEPARTIE_H
