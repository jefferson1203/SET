#ifndef VUECARTE_H
#define VUECARTE_H

#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPushButton>
#include "set.h"

/**
 * @brief The VueCarte class represents a custom QPushButton that displays a card.
 *
 * This class inherits from QPushButton and provides additional functionality for displaying and interacting with a card.
 */
class VueCarte : public QPushButton
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a VueCarte object with the given card and parent widget.
     *
     * @param c The card to be displayed.
     * @param parent The parent widget.
     */
    VueCarte(const Set::Carte& c, QWidget *parent = nullptr);

    /**
     * @brief Constructs an empty VueCarte object with the given parent widget.
     *
     * @param parent The parent widget.
     */
    explicit VueCarte(QWidget *parent = nullptr);

    /**
     * @brief Sets a new card to be displayed in the view.
     *
     * @param c The new card to be displayed.
     */
    void setCarte(const Set::Carte& c);

    /**
     * @brief Sets the view to display no card.
     */
    void setNoCarte();

    /**
     * @brief Returns the currently displayed card.
     *
     * @return The currently displayed card.
     */
    const Set::Carte& getCarte() const;

    /**
     * @brief Checks if a card is currently being displayed.
     *
     * @return True if a card is currently being displayed, false otherwise.
     */
    bool cartePresente() const;

protected:
    /**
     * @brief Reimplemented from QPushButton. Paints the view with the current card.
     *
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    const Set::Carte* carte=nullptr; ///< The currently displayed card.
    QPen pen; ///< The pen used for painting.
    QBrush brush; ///< The brush used for painting.

signals:
    /**
     * @brief Signal emitted when the view is clicked.
     *
     * @param vueCarte Pointer to the VueCarte object that emitted the signal.
     */
    void carteClicked(VueCarte* vueCarte);

public slots:
private slots:
    /**
     * @brief Slot called when the view is clicked. Emits the carteClicked signal.
     */
    void clickedEvent();
};
class VueCarte : public QPushButton
{
    Q_OBJECT
public:
    VueCarte(const Set::Carte& c, QWidget *parent = nullptr);
    explicit VueCarte(QWidget *parent = nullptr);
    // affecter une nouvelle carte à la vue
    void setCarte(const Set::Carte& c) { setCheckable(true); setChecked(false); carte=&c; update(); }
    // vue sans carte
    void setNoCarte() { carte=nullptr; setCheckable(false); update(); }
    const Set::Carte& getCarte() const { return *carte; }
    bool cartePresente() const { return carte!=nullptr; }
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    const Set::Carte* carte=nullptr;
    QPen pen;
    QBrush brush;
signals:
    // quand la vude de carte est cliquée, elle émet un signal en transmettant son adresse
    void carteClicked(VueCarte*);
public slots:
private slots:
    void clickedEvent() { emit carteClicked(this); }
};

#endif // VUECARTE_H
