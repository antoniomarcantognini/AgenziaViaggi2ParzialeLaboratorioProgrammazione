#include "Pacchetto_mare.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

// Costruttore
Pacchetto_mare::Pacchetto_mare(string codice, string destinazione, int giorni, double prezzo,
                               int ombrellone, bool attrezzatura, Categoria_pensione tipo)
    : Pacchetto_viaggio(codice, destinazione, giorni, prezzo),
      ombrellone_incluso(ombrellone),
      attrezzatura_sportiva(attrezzatura),
      tipologia(tipo)
{
    if(ombrellone_incluso < 0) throw invalid_argument("Valore ombrellone non valido.");
    
    cout << "Costruito Pacchetto Mare: " << codice << endl;
}

// Override di calcola prezzo finale
double Pacchetto_mare::calcola_prezzo_finale() const {
    double prezzo_totale = get_prezzo_base();

    // Se ombrelloneIncluso > 0 : +100€
    if(ombrellone_incluso > 0){
        prezzo_totale += 100.0;
    }

    // Se attrezzaturaSportiva == true : +150€
    if(attrezzatura_sportiva){
        prezzo_totale += 150.0;
    }

    // Maggiorazione per Tipologia Pensione
    switch (tipologia) {
        case Categoria_pensione::PENSIONE_COMPLETA:
            prezzo_totale += 200.0;
            break;
        case Categoria_pensione::MEZZA_PENSIONE:
            prezzo_totale += 100.0;
            break;
        case Categoria_pensione::SOLO_COLAZIONE: // nessuna maggiorazione
            break;
        default:
            break;
    }
    
    return prezzo_totale;
}

// Override di stampa dettagli
string Pacchetto_mare::stampa_dettagli() const {
    stringstream ss; 
    
    ss << "--- Pacchetto Vacanze Mare ---" << endl;
    ss << "Codice: " << get_codice_pacchetto() << endl;
    ss << "Destinazione: " << get_destinazione() << endl;
    ss << "Durata: " << get_durata_giorni() << " giorni" << endl;
    ss << "Prezzo Base: " << get_prezzo_base() << " EUR" << endl;
    
    // Dettagli specifici
    ss << "Ombrellone incluso: " << (ombrellone_incluso ? "Sì" : "No") << endl;
    ss << "Attrezzatura Sportiva: " << (attrezzatura_sportiva ? "Sì" : "No") << endl; 
    ss << "Trattamento: " << pensione_to_string(tipologia) << endl;
    
    ss << "Prezzo Finale: " << calcola_prezzo_finale() << " EUR" << endl;

    return ss.str();
}

// Override del metodo get_tipologia
string Pacchetto_mare::get_tipologia() const {
    return "Turismo Balneare";
}

// Getter specifici
int Pacchetto_mare::get_ombrellone_incluso() const {
    return this->ombrellone_incluso;
}

bool Pacchetto_mare::has_attrezzatura_sportiva() const {
    return this->attrezzatura_sportiva;
}

Categoria_pensione Pacchetto_mare::get_categoria_pensione() const {
    return this->tipologia;
}

// Metodi statici per enum

string Pacchetto_mare::pensione_to_string(Categoria_pensione tipo) {
    switch (tipo) {
        case Categoria_pensione::SOLO_COLAZIONE:    return "Solo Colazione";
        case Categoria_pensione::MEZZA_PENSIONE:    return "Mezza Pensione";
        case Categoria_pensione::PENSIONE_COMPLETA: return "Pensione Completa";
        default: return "Sconosciuto";
    }
}

Categoria_pensione Pacchetto_mare::string_to_pensione(string tipo) {
    if (tipo == "Solo Colazione")    return Categoria_pensione::SOLO_COLAZIONE;
    if (tipo == "Mezza Pensione")    return Categoria_pensione::MEZZA_PENSIONE;
    if (tipo == "Pensione Completa") return Categoria_pensione::PENSIONE_COMPLETA;
    return Categoria_pensione::UNKNOWN;
}
