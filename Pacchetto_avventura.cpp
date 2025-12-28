#include "Pacchetto_avventura.h"
#include "Utils_enum.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace std;

// Metodo Privato di Validazione
bool Pacchetto_avventura::valida_dati() const {
    // Validazione ereditata: controlli base sul prezzo e giorni
    if (get_prezzo_base() < 0) {
        throw runtime_error("Il prezzo base non può essere negativo.");
    }
    if (get_durata_giorni() <= 0) {
        throw runtime_error("La durata del viaggio deve essere positiva.");
    }

    // Validazione specifica Avventura
    if (attivita.empty()) {
        // Questa è una scelta di business: decidiamo che un pacchetto avventura DEVE avere attività
        throw runtime_error("Un pacchetto avventura deve contenere almeno un'attività.");
    }
    if(get_categoria_adrenalina() == Categoria_adrenalina::Unknown){
        throw runtime_error("Livello di adrenalina non valido");
    }
    return true;
}

// Costruttore Privato
Pacchetto_avventura::Pacchetto_avventura(string codice, string dest, int giorni, double prezzo,
                                         const vector<string>& lista_attivita, 
                                         Categoria_adrenalina categoria, bool assicurazione)
    : Pacchetto_viaggio(codice, dest, giorni, prezzo),
      attivita(lista_attivita),
      assicurazione_extra(assicurazione),
      categoria_adrenalina(categoria)
{
    // Chiamiamo la validazione. Se fallisce, il costruttore si interrompe e lancia eccezione.
    valida_dati();

    cout << "Costruito Pacchetto Avventura: " << codice << endl;
}

// Factory Method Statico (Try-Catch)
shared_ptr<Pacchetto_avventura> Pacchetto_avventura::crea_pacchetto(string codice, string dest, int giorni, double prezzo,
                                                                    const vector<string>& lista_attivita, 
                                                                    Categoria_adrenalina categoria, bool assicurazione) {
    try {
        // Proviamo a creare l'oggetto, e usiamo 'new' perché il costruttore è privato
        return shared_ptr<Pacchetto_avventura>(new Pacchetto_avventura(codice, dest, giorni, prezzo, lista_attivita, categoria, assicurazione));
        
    } catch (const runtime_error& e) {
        // Gestione dell'errore centralizzata: stampa e ritorna nullptr
        cerr << "Errore creazione Pacchetto Avventura (" << codice << "): " << e.what() << endl;
        return nullptr;
    }
}

// Override di calcola prezzo finale
double Pacchetto_avventura::calcola_prezzo_finale() const {
    double prezzo_totale = get_prezzo_base();

    // attivita.size() * 80€ (costo per attività)
    double costo_attivita = attivita.size() * 80.0;
    prezzo_totale += costo_attivita;

    // Applicazione maggiorazione in base alla Categoria Adrenalina
    switch (categoria_adrenalina) {
        case Categoria_adrenalina::Alto:
            prezzo_totale += (prezzo_totale * 0.25); // +25%
            break;
        case Categoria_adrenalina::Medio:
            prezzo_totale += (prezzo_totale * 0.15); // +15%
            break;
        case Categoria_adrenalina::Basso:
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

// Metodi statici di conversione usando i template
string Pacchetto_avventura::categoria_to_string(Categoria_adrenalina cat) {
    return Utils_enum::to_string(cat);
}

Categoria_adrenalina Pacchetto_avventura::string_to_categoria(string cat) {
    return Utils_enum::from_string<Categoria_adrenalina>(cat);
}

// Override del metodo di salvataggio su file
bool salva_dati_su_file(ofstream& file) const {
    this->salva_dati_su_file();
    file << etos(this->categoria_adrenalina) << ";"
         << (this->assicurazione_extra ? "Con Assicurazione" : "Senza Assicurazione") << ";";
    for (const auto& attivita : this->attivita) {
        file << attivita << ";";
    }
    file << endl;
    cout << "Pacchetto Avventura " << this->codice << " salvato correttamente." << endl;
    return true;
}
