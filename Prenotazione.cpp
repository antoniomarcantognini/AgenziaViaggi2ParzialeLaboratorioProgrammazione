#include <iostream>
#include "Prenotazione.h"
#include "Cliente.h"
#include "Pacchetto_viaggio.h"

using namespace std;

// INIZIALIZZAZIONE STATICHE PRIVATE
int Prenotazione::prenotazioni_create = 0;
double Prenotazione::fatturato_totale = 0.0;
std::unordered_map<std::string, int> Prenotazione::destinazioni_counter;
std::unordered_map<std::string, int> Prenotazione::pacchetti_counter;
std::unordered_map<std::string, int> Prenotazione::spesa_clienti;

// IMPLEMENTAZIONE GETTER STATICHE
int Prenotazione::get_prenotazioni_create() { return prenotazioni_create; }
double Prenotazione::get_fatturato_totale() { return fatturato_totale; }
const std::unordered_map<std::string, int>& Prenotazione::get_destinazioni_counter() { return destinazioni_counter; }
const std::unordered_map<std::string, int>& Prenotazione::get_pacchetti_counter() { return pacchetti_counter; }
const std::unordered_map<std::string, int>& Prenotazione::get_spesa_clienti() { return spesa_clienti; }

// Metodi privati di validazione
bool Prenotazione::valida_input_prenotazione() const {
    auto check_pacchetto = [this]() {
        if (pacchetto_viaggio == nullptr) {
        throw runtime_error("Pacchetto non valido (nullptr).");
        } else if (!pacchetto_viaggio->is_disponibile()) {
            throw runtime_error("Pacchetto non disponibile.");
        }
        return true;
    };

    auto check_cliente = [this]() {
        if (cliente == nullptr) {
            throw runtime_error("Cliente non valido (nullptr).");
        }
        return true;
    };

    auto check_codice_prenotazione = [this]() {
        if (
            codice_prenotazione.substr(0,4) != "BKG-" ||
            codice_prenotazione.length() != 8 || 
            !isdigit(codice_prenotazione[4]) || 
            !isdigit(codice_prenotazione[5]) || 
            !isdigit(codice_prenotazione[6]) || 
            !isdigit(codice_prenotazione[7])) { 
            throw invalid_argument("Codice prenotazione non valido. Deve essere nel formato BKG-XXXX.");
        }
        return true;
    };

    auto check_numero_persone = [this]() {
        if (numero_persone <= 0) {
            throw invalid_argument("Numero di persone deve essere positivo.");
        }
        return true;
    };

    auto check_data_prenotazione = [this]() {
        if (data_prenotazione.length() != 10) {
            throw invalid_argument("Data prenotazione non valida (lunghezza errata): il formato è il seguente: gg/mm/aaaa.");
        }
        else if (
            !isdigit(data_prenotazione[0]) ||
            !isdigit(data_prenotazione[1]) ||
            data_prenotazione[2] != '/' ||
            !isdigit(data_prenotazione[3]) ||
            !isdigit(data_prenotazione[4]) ||
            data_prenotazione[5] != '/' ||
            !isdigit(data_prenotazione[6]) ||
            !isdigit(data_prenotazione[7]) ||
            !isdigit(data_prenotazione[8]) ||
            !isdigit(data_prenotazione[9])
        ) {
            throw invalid_argument("Data prenotazione non valida (formato errato): il formato è il seguente: gg/mm/aaaa.");
        }
        // Ulteriori validazioni sulla data possono essere aggiunte qui
        return true;
    };

    // Esecuzione delle validazioni
    check_pacchetto();
    check_cliente();
    check_codice_prenotazione();
    check_numero_persone();
    check_data_prenotazione();
    return true;
}

// Costruttore privato
Prenotazione::Prenotazione(string cod, shared_ptr<Cliente> cli, shared_ptr<Pacchetto_viaggio> pac, int num_persone, string data) : 
 codice_prenotazione(cod), 
 cliente(cli), 
 pacchetto_viaggio(pac),
 numero_persone(num_persone), 
 data_prenotazione(data) {
    valida_input_prenotazione(); // Validazione input
    this->prezzo_totale = 0.0;
    cout << "Prezzo totale impostato a 0.0" << endl;
    this->confermata = false;
    cout << "Impostazione di default: prenotazione non confermata." << endl;
    prenotazioni_create++; // Aggiorno il contatore statico
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
string Prenotazione::get_codice() const {
    return this->codice_prenotazione;
}

shared_ptr<Cliente> Prenotazione::get_cliente() const {
    return this->cliente;
}

shared_ptr<Pacchetto_viaggio> Prenotazione::get_pacchetto() const {
    return this->pacchetto_viaggio;
}

double Prenotazione::get_prezzo_totale() {
    if (this->prezzo_totale == 0.0) {
        calcola_prezzo_totale();
    }
    return this->prezzo_totale;
}

bool Prenotazione::is_confermata() const {
    return this->confermata;
}

int Prenotazione::get_numero_persone() const {
    return this->numero_persone;
}

string Prenotazione::get_data_prenotazione() const {
    return this->data_prenotazione;
}

// Metodo che calcola il prezzo totale con sconto del cliente applicato
bool Prenotazione::calcola_prezzo_totale() {
    bool flag_aggiornamento = false;
    if (this->prezzo_totale == 0.0) {
        flag_aggiornamento = true;
    }
    
    double prezzo_base = pacchetto_viaggio->calcola_prezzo_finale();
    this->prezzo_totale = cliente->applica_sconto(prezzo_base) * numero_persone;
    cout << "Prezzo totale calcolato: " << this->prezzo_totale << endl;
    
    // Modifica del fatturato clienti e totale se la prenotazione è già confermata
    if (this->confermata && flag_aggiornamento) {
        // Modifica fatturato totale
        fatturato_totale += this->prezzo_totale;
        cout << "Fatturato totale aggiornato (la prenotazione è già stata confermata): " << fatturato_totale << endl;

        // Modifica fatturato cliente
        spesa_clienti.insert({cliente->get_codice(), 0.0}); // Inizializzo il cliente se non esiste
        spesa_clienti[cliente->get_codice()] += this->prezzo_totale;
        cout << "Fatturato cliente  " << cliente->get_codice() << " aggiornato: " << spesa_clienti[cliente->get_codice()] << endl;
    }

    return true;
}

// Metodo che conferma la prenotazione
bool Prenotazione::conferma_prenotazione() {
    
    // Lambda per modificare i valori statici solo se la prenotazione viene confermata
    auto modifica_valori_static = []() {
        
        // Modifica del fatturato clienti e totale
        if (this->prezzo_totale != 0.0) { // Se il prezzo totale è stato calcolato, aggiorno il fatturato
            // Modifica fatturato totale
            fatturato_totale += this->prezzo_totale;
            cout << "Fatturato totale aggiornato (il prezzo totale è già stato calcolato): " << fatturato_totale << endl;

            // Modifica fatturato cliente
            spesa_clienti.insert({cliente->get_codice(), 0.0}); // Inizializzo il cliente se non esiste
            spesa_clienti[cliente->get_codice()] += this->prezzo_totale;
            cout << "Fatturato cliente  " << cliente->get_codice() << " aggiornato: " << spesa_clienti[cliente->get_codice()] << endl;
        }

        // Modifica conteggio delle destinazioni
        destinazioni_counter.insert({pacchetto_viaggio->get_destinazione(), 0}); // Inizializzo la destinazione se non esiste
        destinazioni_counter[pacchetto_viaggio->get_destinazione()]++;
        cout << "Conteggio destinazione " << pacchetto_viaggio->get_destinazione() << " aggiornato: " << destinazioni_counter[pacchetto_viaggio->get_destinazione()] << endl;

        // Modifica conteggio dei pacchetti
        pacchetti_counter.insert({pacchetto_viaggio->get_tipologia(), 0}); // Inizializzo il pacchetto se non esiste
        pacchetti_counter[pacchetto_viaggio->get_tipologia()]++;
        cout << "Conteggio pacchetto " << pacchetto_viaggio->get_tipologia() << " aggiornato: " << pacchetti_counter[pacchetto_viaggio->get_tipologia()] << endl;

    }

    if (this->is_confermata()) {
        cout << "Prenotazione già confermata." << endl;
        return false;
    } else {
        this->confermata = true;
        cout << "Prenotazione confermata." << endl;
        modifica_valori_static();
        return true;
    }
}

// Metodo che stampa i dettagli della prenotazione (anche dettagli cliente e pacchetto)
bool Prenotazione::stampa_info(int indice_produzione) const {
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

// Metodo per il savataggio di dati su un file
bool Prenotazione::salva_dati_su_file(std::ofstream& file) const {
    file << this->codice << ";"
         << this->cliente->get_codice() << ";"
         << this->pacchetto->get_codice() << ";"
         << this->numero_persone << ";"
         << this->data_prenotazione << ";"
         << (this->confermata ? "Confermata" : "Non Confermata") << ";"
         << this->prezzo_totale << endl;
    cout << "Prenotazione " << this->codice << " salvata correttamente." << endl;
}

// Distruttore
Prenotazione::~Prenotazione() {
    cout << "Prenotazione " << this->codice_prenotazione << " distrutta." << endl;
}
