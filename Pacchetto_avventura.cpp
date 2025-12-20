#include "Pacchetto_avventura.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

// Costruttore
Pacchetto_avventura::Pacchetto_avventura(string codice, string dest, int giorni, double prezzo,
                                         const vector<string>& lista_attivita, 
                                         Categoria_adrenalina categoria, bool assicurazione)
    : Pacchetto_viaggio(codice, dest, giorni, prezzo),
      attivita(lista_attivita),
      assicurazione_extra(assicurazione),
      categoria_adrenalina(categoria)
{
    cout << "Costruito Pacchetto Avventura: " << codice << endl;
}

// Override di calcola prezzo finale
double Pacchetto_avventura::calcola_prezzo_finale() const {
    double prezzo_totale = get_prezzo_base();

    // attivita.size() * 80€ (costo per attività)
    double costo_attivita = attivita.size() * 80.0;
    prezzo_totale += costo_attivita;

    // Applicazione maggiorazione in base alla Categoria Adrenalina
    switch (categoria_adrenalina) {
        case Categoria_adrenalina::ALTO:
            prezzo_totale += (prezzo_totale * 0.25); // +25%
            break;
        case Categoria_adrenalina::MEDIO:
            prezzo_totale += (prezzo_totale * 0.15); // +15%
            break;
        case Categoria_adrenalina::BASSO:
            // Nessuna maggiorazione
            break;
        default:
            break;
    }

    // Se assicurazioneExtra: +100€ (sommato alla fine)
    if (assicurazione_extra) {
        prezzo_totale += 100.0;
    }

    return prezzo_totale;
}

// Override di stampa dettagli
string Pacchetto_avventura::stampa_dettagli() const {
    stringstream ss;

    ss << "--- Pacchetto Vacanze Avventura ---" << endl;
    ss << "Codice: " << get_codice_pacchetto() << endl;
    ss << "Destinazione: " << get_destinazione() << endl;
    ss << "Durata: " << get_durata_giorni() << " giorni" << endl;
    ss << "Prezzo Base: " << get_prezzo_base() << " EUR" << endl;

    // Dettagli specifici
    ss << "Livello Adrenalina: " << categoria_to_string(categoria_adrenalina) << endl;
    ss << "Assicurazione Extra: " << (assicurazione_extra ? "Sì" : "No") << endl;
    
    // Stampa attività
    ss << "Attività incluse (" << attivita.size() << "): ";
    if (attivita.empty()) {
        ss << "Nessuna";
    } else {
        for (size_t i = 0; i < attivita.size(); ++i) {
            ss << attivita[i];
            if (i < attivita.size() - 1) {
                ss << ", ";
            }
        }
    }
    ss << endl;

    ss << "Prezzo Finale: " << calcola_prezzo_finale() << " EUR" << endl;

    return ss.str();
}

// Override del metodo get_tipologia
string Pacchetto_avventura::get_tipologia() const {
    return "Turismo Avventura";
}

// Getter specifici
vector<string> Pacchetto_avventura::get_attivita() const {
    return this->attivita;
}

bool Pacchetto_avventura::has_assicurazione_extra() const {
    return this->assicurazione_extra;
}

Categoria_adrenalina Pacchetto_avventura::get_categoria_adrenalina() const {
    return this->categoria_adrenalina;
}

// Metodi statici per enum

string Pacchetto_avventura::categoria_to_string(Categoria_adrenalina cat) {
    switch (cat) {
        case Categoria_adrenalina::BASSO: return "Basso";
        case Categoria_adrenalina::MEDIO: return "Medio";
        case Categoria_adrenalina::ALTO:  return "Alto";
        default: return "Sconosciuto";
    }
}

Categoria_adrenalina Pacchetto_avventura::string_to_categoria(string cat) {
    if (cat == "Basso") return Categoria_adrenalina::BASSO;
    if (cat == "Medio") return Categoria_adrenalina::MEDIO;
    if (cat == "Alto")  return Categoria_adrenalina::ALTO;
    return Categoria_adrenalina::UNKNOWN;
}
