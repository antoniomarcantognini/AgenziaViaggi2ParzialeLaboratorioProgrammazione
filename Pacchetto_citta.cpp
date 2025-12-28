#include "Pacchetto_citta.h"
#include "magic_enum.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace magic_enum;

// Metodo Privato di Validazione
bool Pacchetto_citta::valida_dati() const {
    // Validazione ereditata (Prezzo e Giorni)
    if (get_prezzo_base() < 0) {
        throw runtime_error("Il prezzo base non può essere negativo.");
    }
    if (get_durata_giorni() <= 0) {
        throw runtime_error("La durata del viaggio deve essere positiva.");
    }

    // Validazione specifica Città
    if (get_numero_musei() < 0) {
        throw runtime_error("Il numero di musei non può essere negativo.");
    }
    if(get_categoria_hotel() == Categoria_hotel::Unknown){
        throw runtime_error("Categoria hotel non valida");
    }
    return true;
}

// --- Costruttore Privato ---
Pacchetto_citta::Pacchetto_citta(string codice, string dest, int giorni, double prezzo,
                                 int num_musei, bool guida, Categoria_hotel hotel)
    : Pacchetto_viaggio(codice, dest, giorni, prezzo),
      numero_musei(num_musei),
      guida_turistica(guida),
      categoria_hotel(hotel)
{
    // Chiamiamo la validazione
    valida_dati();
    
    cout << "Costruito Pacchetto Città: " << codice << endl;
}

// Factory Method Statico
shared_ptr<Pacchetto_citta> Pacchetto_citta::crea_pacchetto(string codice, string dest, int giorni, double prezzo,
                                                            int num_musei, bool guida, Categoria_hotel hotel) {
    try {
        // Proviamo a creare l'oggetto e usiamo new perché il costruttore è privato
        return shared_ptr<Pacchetto_citta>(new Pacchetto_citta(codice, dest, giorni, prezzo, num_musei, guida, hotel));

    } catch (const runtime_error& e) {
        // Gestione errore
        cerr << "Errore creazione Pacchetto Città (" << codice << "): " << e.what() << endl;
        return nullptr;
    }
}

// Override di calcola prezzo finale
double Pacchetto_citta::calcola_prezzo_finale() const{
    // Prezzo base + (numeroMusei * 15€)
    double prezzo_totale = get_prezzo_base() + (numero_musei * 15.0);

    // se guida_turistica == true allora +10% sul totale
    if(guida_turistica){
        prezzo_totale += (prezzo_totale * 0.10);
    }

    // maggiorazione per categoria Hotel
    switch (categoria_hotel) {
        case Categoria_hotel::Cinque_stelle:
            prezzo_totale += (prezzo_totale * 0.30); // +30%
            break;
        case Categoria_hotel::Quattro_stelle:
            prezzo_totale += (prezzo_totale * 0.15); // +15%
            break;
        case Categoria_hotel::Tre_stelle: // nessuna maggiorazione
            break;
        default:
            break;
    }
    return prezzo_totale;
}

// Override di stampa dettagli
string Pacchetto_citta::stampa_dettagli() const{
    stringstream ss; // uso stringstream ss per incollare insieme stringhe e numeri in stringhe, altrimenti si concatenano con to_string
    
    ss << "--- Pacchetto Città d'Arte ---" << endl;
    ss << "Codice: " << get_codice_pacchetto() << endl;
    ss << "Destinazione: " << get_destinazione() << endl;
    ss << "Durata: " << get_durata_giorni() << " giorni" << endl;
    ss << "Prezzo Base: " << get_prezzo_base() << " EUR" << endl;
    
    // Dettagli specifici della classe derivata
    ss << "Numero Musei: " << numero_musei << endl;
    // Operatore ternario per stampare Sì/No invece di 1/0
    ss << "Guida Turistica: " << (guida_turistica ? "Sì" : "No") << endl; 
    // Usiamo il metodo statico per stampare la stringa dell'enum
    ss << "Categoria Hotel: " << categoria_to_string(categoria_hotel) << endl;
    
    ss << "Prezzo Finale: " << calcola_prezzo_finale() << " EUR" << endl;

    return ss.str();
}

// Override del metodo get_tipologia
string Pacchetto_citta::get_tipologia() const {
    return "Città d'Arte";
}

// getter
int Pacchetto_citta::get_numero_musei() const {
    return this->numero_musei;
}

bool Pacchetto_citta::has_guida_turistica() const {
    return this->guida_turistica;
}

Categoria_hotel Pacchetto_citta::get_categoria_hotel() const {
    return this->categoria_hotel;
}

// Metodi statici di conversione usando i template
string Pacchetto_citta::categoria_to_string(Categoria_hotel cat) {
    return Utils_enum::to_string(cat);
}

Categoria_hotel Pacchetto_citta::string_to_categoria(string cat) {
    return Utils_enum::from_string<Categoria_hotel>(cat);
}
