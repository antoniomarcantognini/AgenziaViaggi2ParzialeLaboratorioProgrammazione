#include "Pacchetto_montagna.h"
#include "magic_enum.hpp" // Usa magic_enum come negli altri file
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace magic_enum;

// Metodo Privato di Validazione 
bool Pacchetto_montagna::valida_dati() const {
    if (get_prezzo_base() < 0) {
        throw runtime_error("Il prezzo base non può essere negativo.");
    }
    if (get_durata_giorni() <= 0) {
        throw runtime_error("La durata del viaggio deve essere positiva.");
    }
    if (numero_escursioni < 0) { // Usa direttamente la variabile o il getter const
        throw runtime_error("Il numero di escursioni non può essere negativo.");
    }
    // Nota: con enum class strong typed, il controllo su Unknown dipende da come fai il cast, 
    // ma lasciarlo è una buona sicurezza.
    // if (difficolta == Categoria_difficolta::Unknown) throw runtime_error("Categoria difficoltà non valida.");

    return true;
}

// Costruttore Privato
Pacchetto_montagna::Pacchetto_montagna(string codice, string dest, int giorni, double prezzo,
                                       bool skipass, int num_escursioni, Categoria_difficolta diff)
    : Pacchetto_viaggio(codice, dest, giorni, prezzo),
      skipass_incluso(skipass),
      numero_escursioni(num_escursioni),
      difficolta(diff)
{
    valida_dati();
    cout << "Costruito Pacchetto Montagna: " << codice << endl;
}

// Factory Method Statico
shared_ptr<Pacchetto_montagna> Pacchetto_montagna::crea_pacchetto(string codice, string dest, int giorni, double prezzo,
                                                                  bool skipass, int num_escursioni, Categoria_difficolta diff) {
    try {
        return shared_ptr<Pacchetto_montagna>(new Pacchetto_montagna(codice, dest, giorni, prezzo, skipass, num_escursioni, diff));
    } catch (const runtime_error& e) {
        cerr << "Errore creazione Pacchetto Montagna (" << codice << "): " << e.what() << endl;
        return nullptr;
    }
}

// Override di calcola prezzo finale
double Pacchetto_montagna::calcola_prezzo_finale() const {
    double prezzo_totale = get_prezzo_base();

    if (skipass_incluso) {
        prezzo_totale += (get_durata_giorni() * 45.0);
    }

    prezzo_totale += (numero_escursioni * 30.0);

    if (difficolta == Categoria_difficolta::Difficile) {
        prezzo_totale += (prezzo_totale * 0.20); // +20%
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

    ss << "Skipass Incluso: " << (skipass_incluso ? "Sì" : "No") << endl;
    ss << "Numero Escursioni: " << numero_escursioni << endl;
    // Usa magic_enum per stampare
    ss << "Livello Difficoltà: " << enum_name(difficolta) << endl;

    ss << "Prezzo Finale: " << calcola_prezzo_finale() << " EUR" << endl;

    return ss.str();
}

string Pacchetto_montagna::get_tipologia() const {
    return "Turismo Montano";
}

bool Pacchetto_montagna::has_skipass_incluso() const {
    return this->skipass_incluso;
}

int Pacchetto_montagna::get_numero_escursioni() const {
    return this->numero_escursioni;
}

Categoria_difficolta Pacchetto_montagna::get_difficolta() const {
    return this->difficolta;
}

// Salvataggio su file
bool Pacchetto_montagna::salva_dati_su_file(ofstream& file) const {
    // CORREZIONE FONDAMENTALE: Chiama la classe padre, non se stesso!
    Pacchetto_viaggio::salva_dati_su_file(file); 
    
    file << (this->skipass_incluso ? "Con Skipass" : "Senza Skipass") << ";"
         << this->numero_escursioni << ";"
         << enum_name(this->difficolta) << endl; // Usa enum_name di magic_enum
         
    // CORREZIONE NOME GETTER: get_codice_pacchetto() invece di get_codice()
    // cout << "Pacchetto Montagna " << this->get_codice_pacchetto() << " salvato correttamente." << endl;
    return true;
}
