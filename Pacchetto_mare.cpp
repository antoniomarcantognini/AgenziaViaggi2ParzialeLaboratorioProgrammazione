#include "Pacchetto_mare.h"
#include "magic_enum.hpp"
#include "Utils_enum.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace magic_enum;

bool Pacchetto_mare::valida_dati() const {
    // Validazione ereditata: controlli base su prezzo e giorni
    if (get_prezzo_base() < 0) {
        throw runtime_error("Il prezzo base non può essere negativo.");
    }
    if (get_durata_giorni() <= 0) {
        throw runtime_error("La durata del viaggio deve essere positiva.");
    }
    if (get_categoria_pensione() == Categoria_pensione::Unknown) {
        throw runtime_error("Tipologia pensione non valida.");
    }

    // Se arriviamo qui, è tutto valido
    return true;
}

// Costruttore Privato
Pacchetto_mare::Pacchetto_mare(string codice, string dest, int giorni, double prezzo,
                               bool ombrellone, bool attrezzatura, Categoria_pensione tipo)
    : Pacchetto_viaggio(codice, dest, giorni, prezzo),
      ombrellone_incluso(ombrellone),
      attrezzatura_sportiva(attrezzatura),
      tipologia(tipo)
{   
    // Chiamiamo la validazione interna
    valida_dati();

    cout << "Costruito Pacchetto Mare: " << codice << endl;
}

// Factory Method Statico 
shared_ptr<Pacchetto_mare> Pacchetto_mare::crea_pacchetto(string codice, string dest, int giorni, double prezzo,
                                                          bool ombrellone, bool attrezzatura, Categoria_pensione tipo) {
    try {
        // Proviamo a creare l'oggetto e usiamo new perché il costruttore è privato
        return shared_ptr<Pacchetto_mare>(new Pacchetto_mare(codice, dest, giorni, prezzo, ombrellone, attrezzatura, tipo));

    } catch (const runtime_error& e) {
        // Gestione dell'errore
        cerr << "Errore creazione Pacchetto Mare (" << codice << "): " << e.what() << endl;
        return nullptr;
    }
}

// Override di calcola prezzo finale
double Pacchetto_mare::calcola_prezzo_finale() const {
    double prezzo_totale = get_prezzo_base();

    // Se ombrelloneIncluso > 0 : +100€
    if(ombrellone_incluso) prezzo_totale += 100.0;

    // Se attrezzaturaSportiva == true : +150€
    if(attrezzatura_sportiva){
        prezzo_totale += 150.0;
    }

    // Maggiorazione per Tipologia Pensione
    switch (tipologia) {
        case Categoria_pensione::Pensione_completa:
            prezzo_totale += 200.0;
            break;
        case Categoria_pensione::Mezza_pensione:
            prezzo_totale += 100.0;
            break;
        case Categoria_pensione::Solo_colazione: // nessuna maggiorazione
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
bool Pacchetto_mare::has_ombrellone_incluso() const {
    return this->ombrellone_incluso;
}

bool Pacchetto_mare::has_attrezzatura_sportiva() const {
    return this->attrezzatura_sportiva;
}

Categoria_pensione Pacchetto_mare::get_categoria_pensione() const {
    return this->tipologia;
}

// Metodi di conversione usando il template
string Pacchetto_mare::pensione_to_string(Categoria_pensione tipo) {
    return Utils_enum::to_string(tipo); 
}

Categoria_pensione Pacchetto_mare::string_to_pensione(string tipo) {
    return Utils_enum::from_string<Categoria_pensione>(tipo);
}

// Override del metodo di salvataggio su file
bool Pacchetto_mare::salva_dati_su_file(ofstream& file) const {
    this->salva_dati_su_file();
    file << (this->ombrellone_incluso ? "Con Ombrellone" : "Senza Ombrellone") << ";"
         << (this->attrezzatura_sportiva ? "Con Attrezzatura" : "Senza Attrezzatura") << ";"
         << etos(this->tipologia) << endl;
    cout << "Pacchetto Mare " << this->codice << " salvato correttamente." << endl;
    return true;
}
