#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QLCDNumber>
#include <QString>
#include <QMessageBox>
#include "vuepartie.h"
#include "vuecarte.h"

VuePartie::VuePartie(QWidget *parent) : QWidget(parent),vuecartes(20,nullptr)
{
    setWindowTitle("Set !"); // modifier le titre de la fenêtre
    controleur.distribuer();
    pioche= new QLabel("Pioche");
    score=new QLabel("Score");


    // barre de progression de la pioche
    nbCartesPioche=new QProgressBar;
    nbCartesPioche->setRange(0,Set::Jeu::getInstance().getNbCartes());
    nbCartesPioche->setValue(controleur.getPioche().getNbCartes());
    nbCartesPioche->setFixedHeight(30);

    // ne pas afficher le % de la barre de  progression
    nbCartesPioche->setTextVisible(false);

    scoreJoueur=new QLCDNumber;
    scoreJoueur->display(0); // mise à jour du score initial
    scoreJoueur->setFixedHeight(30);

    layoutInformations=new QHBoxLayout;
    layoutCartes=new QGridLayout;;
    couche=new QVBoxLayout;
    layoutInformations->addWidget(pioche);
    layoutInformations->addWidget(nbCartesPioche);
    layoutInformations->addWidget(score);
    layoutInformations->addWidget(scoreJoueur);

    // création des vue des cartes
    for(size_t i=0; i<20; i++) vuecartes[i]=new VueCarte;
    for(size_t i=0; i<20; i++) {
        layoutCartes->addWidget(vuecartes[i],i/4,i%4); // ajout à la grid de cartes
        // connection du slot qui gère les clicks de carte
        connect(vuecartes[i],SIGNAL(carteClicked(VueCarte*)),
                this,
                SLOT(carteClique(VueCarte*)));
    }
    // affectation des cartes actuelles du plateau aux vues des cartes
    size_t i=0;
    for(auto it=controleur.getPlateau().begin();
         it!=controleur.getPlateau().end();
         ++it){
        vuecartes[i]->setCarte(*it);
        i++;
    }

    couche->addLayout(layoutInformations);
    couche->addLayout(layoutCartes);
    setLayout(couche);

}

/**
 * @brief Handles the click event on a card in the game view.
 * 
 * This function is called when a card in the game view is clicked. It performs different actions based on the state of the clicked card.
 * If the clicked card is not present on the game board, it checks if the deck is empty and displays a warning message if it is. Then, it distributes a new card to the game board and updates the card views.
 * If the clicked card is already present on the game board, it checks if the card is selected or deselected. If the card is selected, it adds the card to the set of selected cards and checks if a set combination is formed. If a set is formed, it removes the cards from the game board, updates the score, and updates the card and deck views. If a set is not formed, it displays a warning message and deselects the cards.
 * 
 * @param vc A pointer to the clicked card view.
 */
void VuePartie::carteClique(VueCarte* vc)
{
    // A tester en premier
    if (!vc->cartePresente()){
        //qDebug("ajouter carte !!!\n");
        if (controleur.getPioche().getNbCartes()==0){
            QMessageBox message(QMessageBox::Icon::Warning,
                                "Attention",
                                "La pioche est vide !");
            message.exec();
        }
        controleur.distribuer(); // ajout d’une carte
        // mise à jour des affectations des cartes aux vues des cartes
        size_t i=0;
        for(auto it=controleur.getPlateau().begin();
             it!=controleur.getPlateau().end();
             ++it){
            vuecartes[i]->setCarte(*it);
            i++;
        }
        nbCartesPioche->setValue(controleur.getPioche().getNbCartes());
    }
    else
    { // ICI on ne cherche pas à ajouter une carte au plateau
        if (vc->isChecked()){ // une nouvelle carte est sélectionnée
            selectionCartes.insert(&vc->getCarte()); // on l’insère dans l’ensemble des cartes sélectionnées
            if (selectionCartes.size()==3)
            { // c’est une combinaison qui peut être un set
                // on crée et on teste la combinaison
                vector<const Set::Carte*> c(selectionCartes.begin(),
                                             selectionCartes.end());
                Set::Combinaison comb(*c[0],*c[1],*c[2]);
                if (comb.estUnSET())
                { // c’est un set => il faut mettre à jour le plateau
                    // retirer les cartes du plateau
                    controleur.getPlateau().retirer(*c[0]);
                    controleur.getPlateau().retirer(*c[1]);
                    controleur.getPlateau().retirer(*c[2]);
                    selectionCartes.clear(); // il n’y a plus de cartes sélectionnées
                    // s’il n’y a pas au moins 12 cartes, il faut en remettre
                    if (controleur.getPlateau().getNbCartes()<12)
                        controleur.distribuer();
                    // mise à jour du score
                    scoreValue++;
                    scoreJoueur->display(scoreValue);
                    // mise à jour de la vue du plateau
                    // => nettoyage et mise à jour des vues des cartes
                    for(size_t i=0; i<vuecartes.size();i++)
                        vuecartes[i]->setNoCarte();
                    size_t i=0;
                    for(auto it=controleur.getPlateau().begin();
                         it!=controleur.getPlateau().end();
                         ++it)
                    {
                        vuecartes[i]->setCarte(*it);
                        i++;
                    }
                    // mise à jour de la vue de la pioche
                    nbCartesPioche->setValue(controleur.getPioche().getNbCartes());
                }
                else // else du if (comb.estUnSET())
                {
                    QMessageBox message(QMessageBox::Icon::Warning,
                                        "Attention",
                                        "Ce n’est pas un set !");
                    message.exec();
                    // déselection des cartes
                    for(size_t i=0; i<vuecartes.size();i++)
                        vuecartes[i]->setChecked(false);
                    selectionCartes.clear(); // l’ensemble des cartes sélectionnées est vide
                }
            }
        }
        else // else du if (vc->isChecked())
        {
            // la carte cliquée est déselectionnée
            selectionCartes.erase(&vc->getCarte());
        }

    } // fin de if (!vc->cartePresente())
    update(); // mise à jour de la vue
} // fin de void VuePartie::carteClique(VueCarte* vc)
