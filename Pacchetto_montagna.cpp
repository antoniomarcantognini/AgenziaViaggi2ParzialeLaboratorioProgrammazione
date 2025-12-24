#include "Pacchetto_montagna.h"
#include "Utils_enum.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

// Metodo Privato di Validazione 
bool Pacchetto_montagna::valida_dati() const {
    if (get_prezzo_base() < 0) {
        throw runtime_error("Il prezzo base non può essere negativo.");
    }
    if (get_durata_giorni() <= 0) {
        throw runtime_error("La durata del viaggio deve essere positiva.");
    }
    if (get_numero_escursioni() < 0) {
        throw runtime_error("Il numero di escursioni non può essere negativo.");
    }
    if (get_difficolta() == Categoria_difficolta::Unknown) {
        throw runtime_error("Categoria difficoltà non valida (Unknown).");
    }

    // Se arriviamo qui, è tutto valido
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
    // Chiamiamo la validazione interna
    valida_dati();

    cout << "Costruito Pacchetto Montagna: " << codice << endl;
}

// Factory Method Statico
shared_ptr<Pacchetto_montagna> Pacchetto_montagna::crea_pacchetto(string codice, string dest, int giorni, double prezzo,
                                                                  bool skipass, int num_escursioni, Categoria_difficolta diff) {
    try {
        // Proviamo a creare l'oggetto e usiamo new perché il costruttore è privato
        return shared_ptr<Pacchetto_montagna>(new Pacchetto_montagna(codice, dest, giorni, prezzo, skipass, num_escursioni, diff));

    } catch (const runtime_error& e) {
        // Gestione dell'errore
        cerr << "Errore creazione Pacchetto Montagna (" << codice << "): " << e.what() << endl;
        return nullptr;
    }
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
    if (difficolta == Categoria_difficolta::Difficile) {
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

// Metodi statici di conversione usando il template
string Pacchetto_montagna::difficolta_to_string(Categoria_difficolta diff) {
    return Utils_enum::to_string(diff);
}

Categoria_difficolta Pacchetto_montagna::string_to_difficolta(string diff) {
    return Utils_enum::from_string<Categoria_difficolta>(diff);
}
