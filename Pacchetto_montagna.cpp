#include "Pacchetto_montagna.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

// Costruttore
Pacchetto_montagna::Pacchetto_montagna(string codice, string destinazione, int giorni, double prezzo,
                                       bool skipass, int num_escursioni, Categoria_difficolta diff)
    : Pacchetto_viaggio(codice, destinazione, giorni, prezzo),
      skipass_incluso(skipass),
      numero_escursioni(num_escursioni),
      difficolta(diff)
{
    // Validazione input
    if (numero_escursioni < 0) throw invalid_argument("Numero escursioni negativo.");

    cout << "Costruito Pacchetto Montagna: " << codice << endl;
}

// Override di calcola prezzo finale
double Pacchetto_montagna::calcola_prezzo_finale() const {
    double prezzo_totale = get_prezzo_base();

    // Se skipassIncluso: + durataGiorni * 45€
    if (skipass_incluso) {
        // get_durata_giorni() viene dalla classe padre Pacchetto_viaggio
        prezzo_totale += (get_durata_giorni() * 45.0);
    }

    // numeroEscursioni * 30€
    prezzo_totale += (numero_escursioni * 30.0);

    // Se difficolta == "Difficile": +20% sul totale (equipaggiamento specializzato)
    if (difficolta == Categoria_difficolta::DIFFICILE) {
        prezzo_totale += (prezzo_totale * 0.20); // Aumento del 20%
    }

    return prezzo_totale;
}

// Override di stampa dettagli
string Pacchetto_montagna::stampa_dettagli() const {
    stringstream ss;

    ss << "--- Pacchetto Vacanze Montagna ---" << endl;
    ss << "Codice: " << get_codice_pacchetto() << endl;
    ss << "Destinazione: " << get_destinazione() << endl;
    ss << "Durata: " << get_durata_giorni() << " giorni" << endl;
    ss << "Prezzo Base: " << get_prezzo_base() << " EUR" << endl;

    // Dettagli specifici
    ss << "Skipass Incluso: " << (skipass_incluso ? "Sì" : "No") << endl;
    ss << "Numero Escursioni: " << numero_escursioni << endl;
    ss << "Livello Difficoltà: " << difficolta_to_string(difficolta) << endl;

    ss << "Prezzo Finale: " << calcola_prezzo_finale() << " EUR" << endl;

    return ss.str();
}

// Override del metodo get_tipologia
string Pacchetto_montagna::get_tipologia() const {
    return "Turismo Montano";
}

// Getter specifici
bool Pacchetto_montagna::has_skipass_incluso() const {
    return this->skipass_incluso;
}

int Pacchetto_montagna::get_numero_escursioni() const {
    return this->numero_escursioni;
}

Categoria_difficolta Pacchetto_montagna::get_difficolta() const {
    return this->difficolta;
}

// Metodi statici per enum

string Pacchetto_montagna::difficolta_to_string(Categoria_difficolta diff) {
    switch (diff) {
        case Categoria_difficolta::FACILE:    return "Facile";
        case Categoria_difficolta::MEDIA:     return "Media";
        case Categoria_difficolta::DIFFICILE: return "Difficile";
        default: return "Sconosciuto";
    }
}

Categoria_difficolta Pacchetto_montagna::string_to_difficolta(string diff) {
    if (diff == "Facile")    return Categoria_difficolta::FACILE;
    if (diff == "Media")     return Categoria_difficolta::MEDIA;
    if (diff == "Difficile") return Categoria_difficolta::DIFFICILE;
    return Categoria_difficolta::UNKNOWN;
}
