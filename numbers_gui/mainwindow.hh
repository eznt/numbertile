/*
 * MainWindow -luokan header tiedosto. Luokka on itse käyttöliittymä ikkuna.
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <gameboard.hh>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <QSpinBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QColor>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


const int MARGIN = 45;
const int OBJECTHEIGHT = 30;
const int BUTTON_WIDTH = 60;
const int SPINBOX_WIDTH = 100;
const int LABEL_WIDTH = 120;
const int BOARD_SIZE = 680;
const int NUMBERTILE_WIDTH = 170;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Start -näppäimen slot. Aloittaa pelin.
    void start_game();

    // Reset -näppäimen slot. Lopettaa pelin.
    void restart_game();

private:
    Ui::MainWindow *ui;

    // Pelin toimintapohja.
    GameBoard* gameboard_;

    // Tietorakenne, joka sisältää pelilaudan graafiset ruudut.
    std::vector<std::vector<QGraphicsRectItem*>> tiles_;

    // Pelin reset -näppäin.
    QPushButton *reset_button_;

    // Pelin start -näppäin.
    QPushButton* start_button_;

    // Pelin siemenluvun sisältävä laatikko.
    QSpinBox* seed_value_spinbox_;

    // Pelin maaliluvun sisältävä laatikko.
    QSpinBox* goal_value_spinbox_;

    // Teksti siemenluku laatikkoa varten. Kertoo myös, jos pelaaja häivää tai
    // voittaa pelin.
    QLabel* seed_value_label_;

    // Teksti maaliluku laatikkoa varten.
    QLabel* goal_value_label_;

    // Teksti, joka kertoo pelaajan pistemäärän.
    QLabel* score_label_;

    // Teksti joka kertoo pelissä saadun ennätyspistemäärän.
    QLabel* highscore_label_;

    // Pelilaudan pääikkuna
    QGraphicsScene* main_scene_;

    // Pelilaudan pääikkunan näkymä.
    QGraphicsView* view_;

    // Alustaa reset näppäimen
    void init_reset_button();

    // Alustaa start näppäimen
    void init_start_button();

    // Alustaa siemen lukuun liittyvät komponentit
    void init_seed_value_query();

    // Alustaa maalilukuun liittyvät komponentit
    void init_goal_value_query();

    // Alustaa pisteluku tekstin.
    void init_score_label();

    // Alustaa pelin pääikkunan
    void init_main_scene();

    // Alustaa ennätyspistemäärä tekstin.
    void init_highscore_label();

    // Päivittää pelilauta näkymän.
    void update_gameboard();

    // Alustaa pelilauta näkymän graafiset ruudut.
    void init_tiles();

    // Ottaa parametrina suunnan, johon pelilaudan ruutuja siirretään.
    // Tarkastaa pelin tilanteen siirron jälkeen ja kutsuu funktiota joka
    // päivittää laudan.
    void move_tiles(Coords dir);

    // Palauttaa ruudun värin sen arvon perusteella.
    QColor get_colour(int value);

    // Rekisteröi näppäimen painalluksen, ja kutsuu pelilaudan siiro funktiota
    // painetun näppäimen perusteella.
    void keyPressEvent(QKeyEvent *event);

    // Päivittää ennätyspistemäärä tekstin.
    void update_highscore_label(int score);

};
#endif // MAINWINDOW_HH
