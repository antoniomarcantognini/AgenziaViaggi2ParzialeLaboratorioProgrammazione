#include <iostream>
#include "Prenotazione.h"
#include "Cliente.h"
#include "Pacchetto_viaggio.h"

using namespace std;

// Metodi privati di validazione
bool Prenotazione::valida_pacchetto() const {
    if (pacchetto_viaggio == nullptr) {
        throw runtime_error("Pacchetto non valido (nullptr).");
    } else if (!pacchetto_viaggio->is_disponibile()) {
        throw runtime_error("Pacchetto non disponibile.");
    }
    return true;
}

bool Prenotazione::valida_cliente() const {
    if (cliente == nullptr) {
        throw runtime_error("Cliente non valido (nullptr).");
    }
    return true;
}

bool Prenotazione::valida_codice_prenotazione() const {
    if (
        codice_prenotazione.substr(0,4) != "BKG-" ||
        codice_prenotazione.length() != 8 || 
        !isdigit(codice_prenotazione[4]) || 
        !isdigit(codice_prenotazione[5]) || 
        !isdigit(codice_prenotazione[6]) || 
        !isdigit(codice_prenotazione[7])) { 
        throw runtime_error("Codice prenotazione non valido. Deve essere nel formato BKG-XXXX.");
    }
    return true;
}

// Costruttore privato
Prenotazione::Prenotazione(string cod, shared_ptr<Cliente> cli, shared_ptr<Pacchetto_viaggio> pac, int num_persone, string data) : 
 codice_prenotazione(cod), 
 cliente(cli), 
 pacchetto_viaggio(pac),
 numero_persone(num_persone), 
 data_prenotazione(data) {
    valida_codice_prenotazione();
    valida_pacchetto(); 
    valida_cliente();
    this->prezzo_totale = 0.0;
    cout << "Prezzo totale impostato a 0.0" << endl;
    this->confermata = false;
    cout << "Impostazione di default: prenotazione non confermata." << endl;
    cout << "Prenotazione " << this->codice_prenotazione << " creata correttamente." << endl;
}

// Metodo statico per provare la costruzione tramite smart pointer (è stato utilizzato shared_ptr per permettere la condivisione tra più prenotazioni dello stesso cliente o pacchetto)
shared_ptr<Prenotazione> Prenotazione::crea_prenotazione(string codice, shared_ptr<Cliente> cliente, shared_ptr<Pacchetto_viaggio> pacchetto_viaggio, int num_persone, string data) {
    try {
        return shared_ptr<Prenotazione>(new Prenotazione(codice, cliente, pacchetto_viaggio, num_persone, data)); // Qui viene messo new perché make_unique non può accedere al costruttore privato, mentre new sì.
    } catch (const runtime_error& e) {
        cerr << "Errore nella creazione della prenotazione: " << e.what() << endl; // cerr non crea buffer, quindi è più efficiente per gli errori.
        return nullptr;
    }
}

// Getter
string Prenotazione::get_codice_prenotazione() const {
    return this->codice_prenotazione;
}

shared_ptr<Cliente> Prenotazione::get_cliente() const {
    return this->cliente;
}

shared_ptr<Pacchetto_viaggio> Prenotazione::get_pacchetto() const {
    return this->pacchetto_viaggio;
}

double Prenotazione::get_prezzo_totale() const {
    return this->prezzo_totale;
}

bool Prenotazione::is_confermata() const {
    return this->confermata;
}

// Metodo che calcola il prezzo totale con sconto del cliente applicato
bool Prenotazione::calcola_prezzo_totale() {
    double prezzo_base = pacchetto_viaggio->calcola_prezzo_finale();
    this->prezzo_totale = cliente->applica_sconto(prezzo_base) * numero_persone;
    cout << "Prezzo totale calcolato: " << this->prezzo_totale << endl;
    return true;
}

// Metodo che conferma la prenotazione
bool Prenotazione::conferma_prenotazione() {
    if (this->is_confermata()) {
        cout << "Prenotazione già confermata." << endl;
        return false;
    } else {
        this->confermata = true;
        cout << "Prenotazione confermata." << endl;
        return true;
    }
}

// Metodo che stampa i dettagli della prenotazione (anche dettagli cliente e pacchetto)
bool Prenotazione::stampa_dettagli(int indice_produzione) const {
    cout << "Dettagli Prenotazione:" << endl;
    cout << "Codice Prenotazione: " << this->codice_prenotazione << endl;
    cout << "Data Prenotazione: " << this->data_prenotazione << endl;
    cout << "Numero Persone: " << this->numero_persone << endl;
    cout << "Prezzo Totale: " << this->prezzo_totale << endl;
    cout << "Stato Conferma: " << (this->confermata ? "Confermata" : "Non Confermata") << endl;
    cout << "Dettagli Cliente:" << endl;
    cout << cliente->stampa_info() << endl;
    cout << "Dettagli Pacchetto Viaggio:" << endl;
    cout << pacchetto_viaggio->stampa_dettagli() << endl;
    return true;
}

// Distruttore
Prenotazione::~Prenotazione() {
    cout << "Prenotazione " << this->codice_prenotazione << " distrutta." << endl;
}