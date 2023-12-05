/* COMP.CS.100 Projekti 4: Peli nimeltä 2048
 * Kuvaus:
 *  Peli 2048 -ohjelma lukee instructions.txt nimisestä lähdetiedostosta
 *  riveittäin pelissä aikaisemmin saadut pisteet, ja kirjaa muistiin suurimman
 *  pistemäärän. Jos jossain pelin vaiheessa saavutetaan suurempi pistemäärä
 *  kuin aiempi ennätys, kirjataan se samaan tiedostoon.
 *  Mikäli lähdetiedostoa ei ole, olettaa peli suurimmaksi pistemääräksi 0.
 *  Tällöin ei voida myöskään kirjata pistemäärää ylös mihinkän, vaan kun
 *  suurempi pistemäärä saavutetaan, niin kirjataan se aina pelin lopuksi
 *  vasempaan ylänurkkaan uudeksi ennätyspistemääräksi.
 *
 * Itse pelin peliohjeet löytyvät instructions.txt -tiedostosta.
 *
 * Peli alkaa, kun käyttäjä on valinnut haluamansa maaliluvun ja luvun, jonka
 * perusteella pelilauta generoidaan, ja painaa nappulaa start. Tämä jälkeen
 * pelaaja näkee hänen tavoittelemansa maaliluvun, hänen nykyisen pistemäärän ja
 * resest -näppäin aktivoituu. Peli loppuu, kun pelaaja häivää, voittaa tai
 * painaa reset -näppäintä. Jokaisessa näistä tilanteista pelaajan pistemäärä
 * merkitään uudeksi ennätyspistemäääräksi, mikäli se on suurempi kuin aiempi
 * ennätyspistemäärä. Kun reset -näppäintä on painettu voidaan siemenluku ja
 * maaliluku valita uudestaan, ja peli aloittaa uudestaan.
 *
 * Pelin aikana toimivat näppäintoiminnot:
 * W - siirtää peliruutuja ylöspäin
 * A - siirtää peliruutuja vasemmalle
 * S - siirtää peliruutuja alaspäin
 * D - siirtää peliruutuja oikealle
 *
 * Jos lähdetiedoston rivi sisältää muita kuin numeroita, rivi hylätään.
 * */


#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
