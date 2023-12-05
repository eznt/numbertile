/*
 * Sisältää tiedot MainWindow -luokan metodeista.
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

// Luokan rakentaja
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Luodaan peliä parten pelilauta ja alustetaan se
    gameboard_ = new GameBoard;
    gameboard_->init_empty();

    // Alustetaa käyttöliittymäkomponentit
    init_reset_button();
    init_start_button();
    init_seed_value_query();
    init_goal_value_query();
    init_main_scene();
    init_score_label();
    init_highscore_label();
    init_tiles();

    this->setStyleSheet("background-color:lightgrey");
}

// Luokan destruktori, poistetaan myös pelilauta
MainWindow::~MainWindow()
{
    delete ui;
    delete gameboard_;
}

// Pelin aloitus -näppäimen slot
void MainWindow::start_game()
{
    // Haetaan maaliluku sen laatikosta
    int goal_val = goal_value_spinbox_->text().toInt();

    // Jos maaliluku on suurempi kuin 4 ja kahden potenssi, aloitetaan peli
    if ( (goal_val & (goal_val - 1))==0 and goal_val>=4)
    {
        // Haetaan pelin luomiseen käytettävä siemenluku
        int seed_value = seed_value_spinbox_->text().toInt()+2;

        // Täytetään pääikkuna siemenluvun perusteella.
        gameboard_->fill(seed_value);

        // Päivitetään pelilaudan täytön jälkeen pelilauta
        update_gameboard();

        // Asetetaan peli pois tauolta
        gameboard_->game_pause(false);

        // Piilotetaan, suljetaan ja muokataan käyttöliittymäkomponentteja
        reset_button_->setDisabled(false);
        start_button_->setDisabled(true);

        seed_value_spinbox_->close();
        goal_value_spinbox_->close();

        seed_value_label_->clear();

        score_label_->setText("Score: 0");


        std::string goal_value_text = "Goal value: " + goal_value_spinbox_->text().toStdString();
        goal_value_label_->setText(QString::fromStdString(goal_value_text));
    }
}

// Pelin uudelleen aloitus -näppäimen slot
void MainWindow::restart_game()
{
    // Muokataan käyttöliittymäkomponenttien sisältöjä
    seed_value_label_->setText("Seed value:");
    goal_value_label_->setText("Goal value:");
    score_label_->setText("");

    reset_button_->setDisabled(true);
    start_button_->setDisabled(false);

    seed_value_spinbox_->show();
    goal_value_spinbox_->show();

    // Tyhjennetään pääikkuna
    main_scene_->clear();

    // Asetetaan peli tauolle
    gameboard_->game_pause(true);

    this->setStyleSheet("background-color:lightgrey");

    // Jos pelissä saatu pistemäärä oli suurempi kuin aiempi ennätyspistemäärä
    // ja peliä ei oltu voitettu tai hävitty, niin asetetaan uusi ennätyspiste-
    // määrä, tallennetaan pistemäärä ja päivitetään ennätyspistemäärä tekstiä
    if ( !gameboard_->is_full() and !gameboard_->is_won() and gameboard_->get_score()>gameboard_->get_highscore_())
    {
        int highscore = gameboard_->get_score();
        gameboard_->set_highscore(highscore);
        gameboard_->save_score(highscore);

        update_highscore_label(highscore);
    }
}

// Reset -näppäimen alustus
void MainWindow::init_reset_button()
{
    reset_button_ = new QPushButton("Reset",this);

    reset_button_->setGeometry(13*MARGIN,0.75*MARGIN,BUTTON_WIDTH,OBJECTHEIGHT);

    connect(reset_button_, &QPushButton::clicked,
            this, &MainWindow::restart_game);

    reset_button_->setDisabled(true);
}

// Start -näppäimen alustus
void MainWindow::init_start_button()
{
    start_button_ = new QPushButton("Start",this);

    start_button_->setGeometry(13*MARGIN,0,BUTTON_WIDTH,OBJECTHEIGHT);

    connect(start_button_, &QPushButton::clicked,
            this, &MainWindow::start_game);
}

// Siemenlukuun liittyvien komponenttien alustus
void MainWindow::init_seed_value_query()
{
    seed_value_spinbox_ = new QSpinBox(this);

    seed_value_spinbox_->setGeometry(9*MARGIN,0,SPINBOX_WIDTH,OBJECTHEIGHT);
    seed_value_spinbox_->setValue(0);
    seed_value_spinbox_->setMaximum(999);

    seed_value_label_ = new QLabel(this);

    seed_value_label_->setText("Seed value:");
    seed_value_label_->setGeometry(6.2*MARGIN,0,LABEL_WIDTH,OBJECTHEIGHT);
}

// Maalilukuun liittyvien komponenttien alustus
void MainWindow::init_goal_value_query()
{
    goal_value_spinbox_ = new QSpinBox(this);

    goal_value_spinbox_->setGeometry(9*MARGIN,0.75*MARGIN,SPINBOX_WIDTH,OBJECTHEIGHT);
    goal_value_spinbox_->setMaximum(2048);
    goal_value_spinbox_->setValue(2048);

    goal_value_label_ = new QLabel(this);

    goal_value_label_->setText("Goal value:");
    goal_value_label_->setGeometry(6.2*MARGIN,0.75*MARGIN,LABEL_WIDTH,OBJECTHEIGHT);
}

// Pistemäärä tekstin alustus
void MainWindow::init_score_label()
{
    score_label_ = new QLabel(this);

    score_label_->setText("");
    score_label_->setGeometry(4*MARGIN,0.75*MARGIN,LABEL_WIDTH*0.8,OBJECTHEIGHT);
}

// Pelin pääikkunan alustus
void MainWindow::init_main_scene()
{
    main_scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(this);

    view_->setScene(main_scene_);
    view_->setGeometry(2*MARGIN, 1.5*MARGIN, BOARD_SIZE+3, BOARD_SIZE+3);

    main_scene_->setSceneRect(10,10,BOARD_SIZE,BOARD_SIZE);
}

// Ennätyspistemäärä tekstin alustus
void MainWindow::init_highscore_label()
{
    highscore_label_ = new QLabel(this);

    update_highscore_label(gameboard_->get_highscore_from_file());

    highscore_label_->setGeometry(2*MARGIN,0,LABEL_WIDTH,OBJECTHEIGHT);
}

// Pelilaudan päivitys pelin pääikkunaan
void MainWindow::update_gameboard()
{
    // Tyhjennetään pääikkuna
    main_scene_->clear();

    // Päivitetään pistemäärä teksti
    std::string score_text = "Score: " + std::to_string(gameboard_->get_score());
    score_label_->setText(QString::fromStdString(score_text));

    for (int x=0; x<SIZE;++x)
    {
        for (int y=0; y<SIZE;++y)
        {
            auto coords = std::make_pair(y,x);

            // Jos koordinaateissa ei ole ruutua, ei luoda siihen graafista
            // ruutua
            if( gameboard_->get_item(coords)->is_empty()==true)
            {
                continue;
            }

            // Jos koordinaateissa oleva ruutu on nullptr, luodaan uusi
            // graafinen ruutu ja lisätään se pääikkunaan ja graafiset ruudut
            // sisältävään tietorakenteeseen
            else if(tiles_.at(y).at(x)==nullptr)
            {
                QGraphicsRectItem* square = new QGraphicsRectItem;

                square->setBrush(get_colour(gameboard_->get_item(coords)->get_value_()));
                square->setRect(NUMBERTILE_WIDTH*x+10,NUMBERTILE_WIDTH*y+10,NUMBERTILE_WIDTH,NUMBERTILE_WIDTH);

                tiles_.at(y).push_back(square);
                main_scene_->addItem(square);
            }

            // Muussa tapauksessa ruutu on jo kohdassa, joten muokataan sen
            // sijaintia ja lisätään se pääikkunaan
            else
            {
                tiles_.at(y).at(x)->setBrush(get_colour(gameboard_->get_item(coords)->get_value_()));
                tiles_.at(y).at(x)->setRect(NUMBERTILE_WIDTH*x+10,NUMBERTILE_WIDTH*y+10,NUMBERTILE_WIDTH,NUMBERTILE_WIDTH);

                main_scene_->addItem(tiles_.at(y).at(x));
            }
        }
    }
}

// Graafisten ruutujen alustus tietorakenteeseen
void MainWindow::init_tiles()
{
    std::vector<QGraphicsRectItem*> row;
    for( int i = 0; i < SIZE; ++i)
    {
        row.push_back(nullptr);
    }
    for( int i = 0; i < SIZE; ++i)
    {
        tiles_.push_back(row);
    }
}

// Metodi, joka palauttaa parametrina annetun arvon perusteella värin
// Parameter: int, value; ruudun arvo
// return: QColor; ruudulle tuleva väri
QColor MainWindow::get_colour(int value)
{
    if(value==2)
    {
        return QColor(232, 223, 211);
    }
    else if (value==4)
    {
        return QColor(237, 211, 175);
    }
    else if (value==8)
    {
        return QColor(230, 169, 85);
    }
    else if (value==16)
    {
       return QColor(232, 128, 53);
    }
    else if (value==32)
    {
        return QColor(237, 112, 74);
    }
    else if (value==64)
    {
        return QColor(232, 65, 14);
    }
    else if (value==128)
    {
        return QColor(235, 227, 157);
    }
    else if (value==256)
    {
        return QColor(230, 218, 117);
    }
    else if (value==512)
    {
        return QColor(232, 207, 79);
    }
    else if (value==1024)
    {
        return QColor(237, 205, 43);
    }
    else
    {
        return QColor(252, 194, 0);
    }
}

// Lukee käyttäjän painamat näppäimet
// Parameter: QKeyEvent*, event; käyttäjän painama näppäin
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Jos peli ei ole käynnissä lopetetaan toiminta
    if (!gameboard_->is_game_ongoing())
    {
        return;
    }

    auto key = event->key();

    // Kutsutaan ruutujen siirto metodia käyttäjän painaman näppäimen perusteella
    if(key==Qt::Key_W){
        move_tiles(UP);
    }
    else if( key==Qt::Key_A)
    {
        move_tiles(LEFT);
    }
    else if( key==Qt::Key_D)
    {
        move_tiles(RIGHT);
    }
    else if( key==Qt::Key_S)
    {
        move_tiles(DOWN);
    }
    else
    {
        return;
    }
}

// Päivittää ennätyspistemäärä tekstin
// Parameter: int, score; uusi ennätyspistemäärä
void MainWindow::update_highscore_label(int score)
{
    std::string highscore_text = "Highscore: ";
    highscore_text = highscore_text + std::to_string(score);

    highscore_label_->setText(QString::fromStdString(highscore_text));
}

// Kutsuu pelilaudan metodia, joka liikuttaa peliruutuja, parametrina annetun
// suunnan perusteella
// Parameter: Coords, dir; suunta johon ruutuja siirretään
void MainWindow::move_tiles(Coords dir)
{
    // Jos peliruutujen siirto metodi palauttaa true, on peli voitettu
    if( gameboard_->move(dir, goal_value_spinbox_->text().toInt()) )
    {
        // Lisätään pelilaudalle uusi ruutu jos mahdollista ja päivitetään
        // graafinen pelilauta näkymä
        gameboard_->new_value(gameboard_->is_empty());
        update_gameboard();

        seed_value_label_->setText("You won");

        this->setStyleSheet("background-color:green");

        // Jos pelin aikana saatu pistemäärä oli suurempi kuin aikaisempi
        // ennätyspistemäärä, asetetaan pistemäärä uudeksi ennätyspistemäärksi,
        // tallennetaan se ja päivitetään ennätyspistemäärä teksti
        if(gameboard_->get_score()>gameboard_->get_highscore_())
        {
            int highscore = gameboard_->get_score();
            gameboard_->set_highscore(highscore);
            gameboard_->save_score(highscore);

            update_highscore_label(highscore);
        }
    }

    else
    {
        gameboard_->new_value(gameboard_->is_empty());
        update_gameboard();

        // Jos pelilauta on täynnä, tarkoittaa se, että peli on hävitty
        if (gameboard_->is_full())
        {
            seed_value_label_->setText("You lost");

            this->setStyleSheet("background-color:red");

            // Jos pelin aikana saatu pistemäärä oli suurempi kuin aikaisempi
            // ennätyspistemäärä, asetetaan pistemäärä uudeksi ennätyspistemäärksi,
            // tallennetaan se ja päivitetään ennätyspistemäärä teksti
            if(gameboard_->get_score()>gameboard_->get_highscore_())
            {
                int highscore = gameboard_->get_score();
                gameboard_->set_highscore(highscore);
                gameboard_->save_score(highscore);

                update_highscore_label(highscore);
            }
        }
    }
}




