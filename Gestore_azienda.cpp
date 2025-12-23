#include "magic_enum.hpp"
#include "Gestore_azienda.h"
using namespace std;
using namespace magic_enum;

// TEMPLATE PER AGGIUNTA DI UN ELEMENTO AL VETTORE DI PUNTATORI

template <typename T>
shared_ptr<T> aggiungi_elemento(vector<shared_ptr<T>>& lista, shared_ptr<T> elemento) {
    if (elemento != nullptr) {
        lista.push_back(elemento);
        return elemento;
    } else {
        throw runtime_error("Impossibile aggiungere elemento nullo alla lista.");
        return nullptr;
    }
}

// TEMPLATE PER RICERCA DI UN ELEMENTO NEL VETTORE DI PUNTATORI
template <typename T>
shared_ptr<T> cerca_elemento(const vector<shared_ptr<T>>& lista, const string& codice) {
    for (const auto& elemento : lista) {
        if (elemento->get_codice() == codice) {
            return elemento;
        }
    }
    return nullptr; // Elemento non trovato
}

// TEMPLATE PER LA STAMPA DI ELEMENTI IN UN VETTORE DI PUNTATORI
template <typename T>
bool stampa_elementi(const vector<shared_ptr<T>>& lista) {
    for (const auto& elemento : lista) {
        cout << elemento->stampa_dettagli() << endl;
    }
    return true;
}

// TEMPLATE PER LA STAMPA DI ELEMENTI IN UN VETTORE DI PUNTATORI FILTRATI PER TIPOLOGIA
template <typename T>
bool stampa_elementi_per_tipologia(const vector<shared_ptr<T>>& lista, const string& tipologia) {
    for (const auto& elemento : lista) {
        if (enum_name(elemento->get_tipologia()) == tipologia) {
            cout << elemento->stampa_dettagli() << endl;
        }
    }
    return true;
}

// TEMPLATE PER IL CALCOLO DELLA CATEGORIA PIÙ RICHIESTA
template <typename T>
string calcola_massimo_mappa(const unordered_map<string, T> counter) {
    if (counter.empty()) {
            return "Nessuna prenotazione effettuata";
    }
    return max_element(
        
        // Posizioni di inizio e fine della mappa
        counter.begin(), 
        counter.end(),

        // Lambda per confrontare i valori delle mappe: considero "massimo" quello con il conteggio più alto
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }

    )->first;
}
// METODI PRIVATI DI UTILITY
// Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0005")
string Gestore_azienda::generaCodiceUnico(char tipo) {
    int numero = 0;
    string codice_univoco;
    switch (tipo) {
        case 'C': // Cliente
            numero = Cliente::clienti_creati + 1;
            codice_univoco = "CLT-" + to_string(numero);
            break;
        case 'P': // Pacchetto
            numero = Pacchetto_viaggio::pacchetti_creati + 1;
            codice_univoco = "PKG-" + to_string(numero);
            break;
        case 'B': // Prenotazione (Booking)
            numero = Prenotazione::prenotazioni_create + 1;
            codice_univoco = "BKG-" + to_string(numero);
            break;
        default:
            throw runtime_error("Tipo di codice non riconosciuto.");
    }
    return codice_univoco;
}

// === COSTRUTTORE E DISTRUTTORE ===
Gestore_azienda::Gestore_agenzia();
Gestore_azienda::~Gestore_agenzia(); 

// === GESTIONE CATALOGO PACCHETTI ===

// Metodo per l'aggiunta di un pacchetto avventura
bool Gestore_azienda::aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    const vector<string>& lista_attivita, 
                    Categoria_adrenalina categoria, bool assicurazione) {

                    // Creazione del puntatore al nuovo pacchetto avventura
                    shared_ptr<Pacchetto_avventura> nuovo_pacchetto = make_shared<Pacchetto_avventura>(codice, dest, giorni, prezzo, lista_attivita, categoria, assicurazione);
                    
                    // Aggiunta al catalogo
                    shared_ptr<Pacchetto_avventura> temp = aggiungi_elemento(this->catalogo, nuovo_pacchetto);
                    if (temp == nullptr) {
                        throw runtime_error("Errore nell'aggiunta del pacchetto avventura al catalogo.");
                    }
                    
                    return true;
}

// Metodo per l'aggiunta di un pacchetto mare
bool Gestore_azienda::aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    bool ombrellone, bool attrezzatura, Categoria_pensione tipo) {

                    // Creazione del puntatore al nuovo pacchetto mare
                    shared_ptr<Pacchetto_mare> nuovo_pacchetto = make_shared<Pacchetto_mare>(codice, dest, giorni, prezzo, ombrellone, attrezzatura, tipo);

                    // Aggiunta al catalogo
                    shared_ptr<Pacchetto_mare> temp = aggiungi_elemento(this->catalogo, nuovo_pacchetto);
                    if (temp == nullptr) {
                        throw runtime_error("Errore nell'aggiunta del pacchetto mare al catalogo.");
                    }

                    return true;
}

// Metodo per l'aggiunta di un pacchetto montagna
bool Gestore_azienda::aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    int altitudine, bool guida_inclusa, Categoria_difficolta difficolta) {

                    // Creazione del puntatore al nuovo pacchetto montagna
                    shared_ptr<Pacchetto_montagna> nuovo_pacchetto = make_shared<Pacchetto_montagna>(codice, dest, giorni, prezzo, altitudine, guida_inclusa, difficolta);

                    // Aggiunta al catalogo
                    shared_ptr<Pacchetto_montagna> temp = aggiungi_elemento(this->catalogo, nuovo_pacchetto);
                    if (temp == nullptr) {
                        throw runtime_error("Errore nell'aggiunta del pacchetto montagna al catalogo.");
                    }
                    
                    return true;
}

// Metodo per l'aggiunta di un pacchetto città
bool Gestore_azienda::aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    int num_musei, bool guida, Categoria_hotel hotel) {

                    // Creazione del puntatore al nuovo pacchetto città
                    shared_ptr<Pacchetto_citta> nuovo_pacchetto = make_shared<Pacchetto_citta>(codice, dest, giorni, prezzo, num_musei, guida, hotel);

                    // Aggiunta al catalogo
                    shared_ptr<Pacchetto_citta> temp = aggiungi_elemento(this->catalogo, nuovo_pacchetto);
                    if (temp == nullptr) {
                        throw runtime_error("Errore nell'aggiunta del pacchetto città al catalogo.");
                    }

                    return true;
}
shared_ptr<Pacchetto_viaggio> Gestore_azienda::cercaPacchetto(string codice) {
    return cerca_elemento(this->catalogo, codice);
}
bool Gestore_azienda::visualizzaCatalogo() const {
    return stampa_elementi(this->catalogo);
}

bool Gestore_azienda::visualizzaPacchettiPerTipologia(string tipo) const {
    return stampa_elementi_per_tipologia(this->catalogo, tipo);
}
bool Gestore_azienda::visualizzaPacchettiDisponibili() const {
    for (const auto& pacchetto : catalogo) {
        if (pacchetto->is_disponibile()) {
            cout << pacchetto->stampa_dettagli() << endl;
        }
    }
    return true;
}

// === GESTIONE CLIENTI ===
bool Gestore_azienda::aggiungiCliente(std::string codice, std::string nome, std::string cognome, 
                    std::string email, std::string tel, int eta, Tipologia_cliente tipo) {

                    // Creazione del puntatore e del nuovo cliente            
                    shared_ptr<Cliente> nuovo_cliente = crea_cliente(codice, nome, cognome, email, tel, eta, tipo);
                    
                    // Aggiunta alla lista clienti
                    shared_ptr<Cliente> temp = aggiungi_elemento(this->clienti, nuovo_cliente);
                    if (temp == nullptr) {
                        throw runtime_error("Errore nell'aggiunta del cliente al catalogo.");
                    }
                    return true;
}
shared_ptr<Cliente> Gestore_azienda::cercaCliente(string codice) {
    return cerca_elemento(this->clienti, codice);
}

bool Gestore_azienda::visualizzaClienti() const {
    return stampa_elementi(this->clienti);
}

bool Gestore_azienda::visualizzaClientiPerTipologia(string tipo) const {
    return stampa_elementi_per_tipologia(this->clienti, tipo);
}


// === GESTIONE PRENOTAZIONI ===
bool Gestore_azienda::creaPrenotazione(string codice, shared_ptr<Cliente> cliente, shared_ptr<Pacchetto_viaggio> pacchetto_viaggio,
                     int num_persone, string data) {
    
                    // Creazione del puntatore e della nuova prenotazione
                    shared_ptr<Prenotazione> nuova_prenotazione = Prenotazione::crea_prenotazione(codice, cliente, pacchetto_viaggio, num_persone, data);
                    
                    // Aggiunta alla lista prenotazioni
                    shared_ptr<Prenotazione> temp = aggiungi_elemento(this->prenotazioni, nuova_prenotazione);
                    if (temp == nullptr) {
                        throw runtime_error("Errore nell'aggiunta della prenotazione al catalogo.");
                    }
                    return true;
}
bool Gestore_azienda::confermaPrenotazione(string codice) {
    // Questo metodo ritorna true se la modifica è stata apportata, altrimenti ritorna false (se il codice era già confermato ritorna false)
    shared_ptr<Prenotazione> prenotazione = cerca_elemento(this->prenotazioni, codice);
    if (prenotazione != nullptr) {
        if (!prenotazione->conferma_prenotazione()) {
            cerr << "Errore! Codice già confermato in precedenza!" << codice << "." << endl;
            return false;
        } else {
            return true;
        }
    } else {
        cerr << "Prenotazione con codice " << codice << " non trovata." << endl;
        return false;
    }
}
bool Gestore_azienda::visualizzaPrenotazioni() const {
    return stampa_elementi(this->prenotazioni);
}
bool Gestore_azienda::visualizzaPrenotazioniCliente(string codiceCliente) const {
    int i = 0;
    for (const auto& prenotazione : prenotazioni) {
        if (prenotazione->get_cliente()->get_codice() == codiceCliente) {
            cout << "Prenotazione #" << ++i << " del cliente " << prenotazione->get_cliente()->get_nome_completo() << ":" << endl;
            cout << prenotazione->stampa_dettagli() << endl;
        }
    }
    return true;
}
bool Gestore_azienda::visualizzaPrenotazioniConfermate() const {
    int i = 0;
    for (const auto& prenotazione : prenotazioni) {
        if (prenotazione->is_confermata()) {
            cout << "Prenotazione confermata #" << ++i << ":" << endl;
            cout << prenotazione->stampa_dettagli() << endl;
        }
    }
    return true;
}

// === STATISTICHE ===
bool Gestore_azienda::statisticheGenerali() const {
    cout << "=== Statistiche Generali Agenzia Viaggi ===" << endl;
    cout << "Totale Pacchetti nel Catalogo: " << Pacchetto_viaggio::pacchetti_creati << endl;
    cout << "Totale Clienti Registrati: " << Cliente::clienti_creati << endl;
    cout << "Totale Prenotazioni Effettuate: " << Prenotazione::prenotazioni_create << endl;
    cout << "Fatturato Totale: " << Prenotazione::fatturato_totale << " EUR" << endl;
    cout << "Pacchetto più prenotato: " << calcola_massimo_mappa(pacchetti_counter) << endl;
    cout << "Destinazione più richiesta: " << calcola_massimo_mappa(destinazioni_counter) << endl;
    return true;
}
// Stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Top Pacchetto, Top Destinazione
    
bool Gestore_azienda::statistichePerTipologia() const {
    cout << "=== Numero di prenotazioni per ogni Tipologia di Pacchetto ===" << endl;
    for (const auto& campo : pacchetti_counter) {
        cout << "Tipologia: " << campo.first << " - Prenotazioni: " << campo.second << endl;
    }
    return true;
}
// Stampa quante prenotazioni ci sono per Mare, Montagna, ecc.

shared_ptr<Cliente> Gestore_azienda::clienteMigliore() const {
    cout << "Cliente che ha speso di più: " << calcola_massimo_mappa(spesa_clienti) << endl;
}
// Ritorna il cliente che ha speso di più

// === GESTIONE FILE ===
// Salva/Carica lo stato intero dell'agenzia (CSV simulato)
bool Gestore_azienda::salvaDatiSuFile(const string& nomefile) const;
bool Gestore_azienda::caricaDatiDaFile(const string& nomefile);
