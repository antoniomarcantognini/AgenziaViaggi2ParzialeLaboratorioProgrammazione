#include "magic_enum.hpp"
#include "Gestore_azienda.h"
#include "Esito_input_SN.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include "menu.h"

using namespace std;
using namespace magic_enum;
using namespace Esito_input_SN;

// === IMPLEMENTAZIONE METODI HELPER PRIVATI (STATIC) ===

// Helper per split stringa
vector<string> Gestore_agenzia::split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Funzione che restituisce SI se il valore inserito è 's'/'S', NO se il valore inserito è 'n'/'N', RIPROVA se si vuole riprovare l'inserimento, ANNULLA altrimenti.
Esito_input_SN Gestore_agenzia::analizza_input_sn(const string& input) {
    if (input.empty()) return ANNULLA;
    char c = tolower(input[0]);
    if (c == 's') return SI;
    if (c == 'n') return NO;

    cerr << "Valore inserito non valido: puoi inserire solo 'S' o 'N'." << endl;

    // Reinserimento?
    int scelta = menu::menu_reinserimento();
    if(scelta == 1) return RIPROVA;
    else return ANNULLA;
}

// Funzione che valida l'inserimento di 'S' e 'N'. Ritorna false se è stato annullato l'inserimento, 
// continua il ciclo se si vuole riprovare, altrimenti ritorna true e aggiorna flag_da_aggiornare.
// flag_da_aggiornare = true se è stato inserito 'S'/'s'
// flag_da_aggiornare = false se è stato inserito 'N'/'n'
bool Gestore_agenzia::valida_inserimento_sn(string& stringa_s_n, bool& flag_da_aggiornare) {
    do{
        cin >> stringa_s_n;
        Esito_input_SN esito = analizza_input_sn(stringa_s_n);
        if (esito == ANNULLA) return false;
        else if (esito == RIPROVA) continue; // Se l'utente ha scelto di riprovare, ricomincia il ciclo while (e quindi reinserisci il valore)
        else {
            flag_da_aggiornare = (esito == SI);
            return true;
        }
    } while(1);
}

// === IMPLEMENTAZIONE METODI MEMBRI DI GESTORE_AGENZIA ===

string Gestore_agenzia::tolower_string(string stringa){
    for (char &c : stringa) {
        c = tolower((unsigned char)c);
    }
    return stringa;
}

// Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0005")
string Gestore_agenzia::genera_codice_unico(char tipo) {
    int numero = 0;
    string codice_univoco;
    switch (tipo) {
        case 'C': // Cliente
            numero = Cliente::get_clienti_creati() + 1;
            codice_univoco = "CLT-" + to_string(numero);
            break;
        case 'P': // Pacchetto
            numero = Pacchetto_viaggio::get_pacchetti_creati() + 1;
            codice_univoco = "PKG-" + to_string(numero);
            break;
        case 'B': // Prenotazione (Booking)
            numero = Prenotazione::get_prenotazioni_create() + 1;
            codice_univoco = "BKG-" + to_string(numero);
            break;
        default:
            cerr << "Errore generazione codice: tipo sconosciuto." << endl;
            return "ERR";
    }
    return codice_univoco;
}

// Metodo di inizializzazione della mappa
bool Gestore_agenzia::inizializza_mappa() {
    try {
        // Lambda per caricamento Avventura
        mappa_caricamento_specifico["Turismo Avventura"] = [this](string& codice, string& dest, int& giorni, bool& disp, double& prezzo, const vector<string>& campi, int& riga) -> bool {
            vector<string> lista;
            auto cat_opt = stoe<Categoria_adrenalina>(campi[6], ++riga);
            if (!cat_opt) return false; // Controllo errori
            
            bool ass = (campi[7] == "Con Assicurazione");
            for (size_t i = 8; i < campi.size(); ++i) lista.push_back(campi[i]);
            
            auto nuovo = Pacchetto_avventura::crea_pacchetto(codice, dest, giorni, prezzo, lista, cat_opt.value(), ass);
            if (nuovo && aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo))) {
                cout << "Pacchetto Avventura " << codice << " caricato correttamente." << endl;
                return true;
            }
            return false;
        };

        // Lambda per caricamento Mare
        mappa_caricamento_specifico["Turismo Balneare"] = [this](string& codice, string& dest, int& giorni, bool& disp, double& prezzo, const vector<string>& campi, int& riga) -> bool {
            bool omb = (campi[6] == "Con Ombrellone");
            bool attr = (campi[7] == "Con Attrezzatura");
            auto pens_opt = stoe<Categoria_pensione>(campi[8], ++riga);
            if (!pens_opt) return false; // Controllo errori

            auto nuovo = Pacchetto_mare::crea_pacchetto(codice, dest, giorni, prezzo, omb, attr, pens_opt.value());
            if (nuovo && aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo))) {
                cout << "Pacchetto Mare " << codice << " caricato correttamente." << endl;
                return true;
            }
            return false;
        };

        // Lambda per caricamento Montagna
        mappa_caricamento_specifico["Turismo Montano"] = [this](string& codice, string& dest, int& giorni, bool& disp, double& prezzo, const vector<string>& campi, int& riga) -> bool {
            bool skip = (campi[6] == "Con Skipass");
            int esc = stoi(campi[7]);
            auto diff_opt = stoe<Categoria_difficolta>(campi[8], ++riga);
            if (!diff_opt) return false; // Controllo errori

            auto nuovo = Pacchetto_montagna::crea_pacchetto(codice, dest, giorni, prezzo, skip, esc, diff_opt.value());
            if (nuovo && aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo))) {
                cout << "Pacchetto Montagna " << codice << " caricato correttamente." << endl;
                return true;
            }
            return false;
        };

        // Lambda per caricamento Città
        mappa_caricamento_specifico["Città d'Arte"] = [this](string& codice, string& dest, int& giorni, bool& disp, double& prezzo, const vector<string>& campi, int& riga) -> bool {
            bool guida = (campi[6] == "Con Guida");
            int musei = stoi(campi[7]);
            auto hotel_opt = stoe<Categoria_hotel>(campi[8], ++riga);
            if (!hotel_opt) return false; // Controllo errori

            auto nuovo = Pacchetto_citta::crea_pacchetto(codice, dest, giorni, prezzo, musei, guida, hotel_opt.value());
            if (nuovo && aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo))) {
                cout << "Pacchetto Città " << codice << " caricato correttamente." << endl;
                return true;
            }
            return false;
        };
        
        return true; // Mappa inizializzata con successo
    } catch (const std::exception& e) {
        cerr << "Errore inizializzazione mappa: " << e.what() << endl;
        return false;
    }
}

// === METODI DI INPUT DATI ===

// Funzione di inserimento manuale dei dati da tastiera per pacchetti generico
bool Gestore_agenzia::inserisci_dati_pacchetto_base(string& dest, int& giorni, double& prezzo) {
    cout << "=== INSERIMENTO DATI NUOVO PACCHETTO ===" << endl;
    cout << "Destinazione: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, dest);

    cout << "Durata in giorni: ";
    if (!(cin >> giorni)) return false;

    cout << "Prezzo base: ";
    if (!(cin >> prezzo)) return false;

    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti avventura
bool Gestore_agenzia::inserisci_dati_pacchetto(vector<string>& lista_attivita, Categoria_adrenalina& categoria, bool& assicurazione) {
    cout << "Numero di attività incluse: ";
    int num_attivita;
    if (!(cin >> num_attivita)) return false;
    
    lista_attivita.clear();
    for (int i = 0; i < num_attivita; ++i) {
        string attivita;
        cout << "Attività #" << (i + 1) << ": ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, attivita);
        lista_attivita.push_back(attivita);
    }

    cout << "Categoria Adrenalina (scrivere ESATTAMENTE Basso, Medio o Alto): ";
    string categoria_str;
    cin >> categoria_str;
    auto temp = stoe<Categoria_adrenalina>(categoria_str);
    if (!temp.has_value()) return false; // Controllo errori
    else categoria = temp.value();

    cout << "Assicurazione inclusa? (S/N): ";
    string stringa_ass;
    if(!valida_inserimento_sn(stringa_ass, assicurazione)) return false; // Controllo se l'utente ha deciso di annullare l'operazione
    
    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti mare
bool Gestore_agenzia::inserisci_dati_pacchetto(bool& ombrellone, bool& attrezzatura, Categoria_pensione& tipo) {
    cout << "Ombrellone incluso? (S/N): ";
    string stringa_omb;
    if(!valida_inserimento_sn(stringa_omb, ombrellone)) return false;

    cout << "Attrezzatura inclusa? (S/N): ";
    string stringa_attr;
    if(!valida_inserimento_sn(stringa_attr, attrezzatura)) return false;

    cout << "Categoria Pensione (Inserire ESATTAMENTE Solo colazione, Mezza pensione o Pensione completa): ";
    string string_pensione;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, string_pensione);
    auto temp = stoe<Categoria_pensione>(string_pensione);
    if (!temp.has_value()) return false; // Controllo errori
    else tipo = temp.value();
    
    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti montagna
bool Gestore_agenzia::inserisci_dati_pacchetto(bool& skipass_incluso, int& num_escursioni, Categoria_difficolta& difficolta) {
    cout << "Skipass incluso? (S/N): ";
    string stringa_skip;
    if(!valida_inserimento_sn(stringa_skip, skipass_incluso)) return false;

    cout << "Numero di escursioni: ";
    if (!(cin >> num_escursioni)) return false;

    cout << "Difficoltà (inserire ESATTAMENTE Facile, Media o Difficile): ";
    string stringa_diff;
    cin >> stringa_diff;
    auto temp = stoe<Categoria_difficolta>(stringa_diff);
    if (!temp.has_value()) return false; // Controllo errore
    else difficolta = temp.value();

    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti città
bool Gestore_agenzia::inserisci_dati_pacchetto(int& num_musei, bool& guida, Categoria_hotel& hotel) {
    cout << "Numero di musei: ";
    if (!(cin >> num_musei)) return false;

    cout << "Guida inclusa? (S/N): ";
    string stringa_guida;
    if (!valida_inserimento_sn(stringa_guida, guida)) return false;

    cout << "Hotel (inserire esattamente Tre stelle, Quattro stelle o Cinque stelle): ";
    string stringa_hotel;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, stringa_hotel);
    auto temp = stoe<Categoria_hotel>(stringa_hotel);
    if (!temp.has_value()) return false;
    else hotel = temp.value();

    return true;
}

// Funzione inserimento dati cliente
bool Gestore_agenzia::inserimento_dati_cliente(string& nome, string& cognome, string& email, string& telefono,
    int& eta, string& tipo_str, Tipologia_cliente& tipo) {
    
    cout << "=== INSERIMENTO DATI NUOVO CLIENTE ===" << endl;
    cout << "Nome: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nome);

    cout << "Cognome: ";
    getline(cin, cognome); 

    cout << "Email (deve contenere @!): ";
    cin >> email;

    cout << "Telefono (10 cifre!): ";
    cin >> telefono;

    cout << "Età (deve essere maggiore di 0): ";
    if (!(cin >> eta)) return false;

    cout << "Tipologia Cliente (scrivere ESATTAMENTE Standard, Premium o VIP): ";
    cin >> tipo_str;
    auto temp = stoe<Tipologia_cliente>(tipo_str);
    if (!temp.has_value()) return false;
    else tipo = temp.value();
    return true;
}

// Funzione inserimento dati prenotazione
bool Gestore_agenzia::inserimento_dati_prenotazione(shared_ptr<Cliente>& cliente, shared_ptr<Pacchetto_viaggio>& pacchetto_viaggio,
    int& num_persone, string& data) {
    
    string codice_cliente; string codice_pacchetto;
    cout << "=== INSERIMENTO DATI NUOVA PRENOTAZIONE ===" << endl;

    cout << "Codice Cliente: ";
    cin >> codice_cliente;
    cliente = cerca_cliente(codice_cliente); // Usa il metodo della classe
    if (cliente == nullptr) {
        cerr << "Cliente non trovato." << endl;
        return false;
    }

    cout << "Codice Pacchetto Viaggio: ";
    cin >> codice_pacchetto;
    pacchetto_viaggio = cerca_pacchetto(codice_pacchetto); // Usa il metodo della classe
    if (pacchetto_viaggio == nullptr) {
        cerr << "Pacchetto non trovato." << endl;
        return false;
    }

    cout << "Numero di Persone: ";
    if (!(cin >> num_persone)) return false;

    cout << "Data Prenotazione (DD/MM/YYYY): ";
    cin >> data;

    return true;
}

// === CARICAMENTO FILE ===

// Funzione che carica il valore
bool Gestore_agenzia::assegna_valore_cliente(vector<string>& campi, int& numero_riga, string& codice, string& nome, string& cognome, string& email, string& telefono, int& eta, Tipologia_cliente& tipologia) {
    if(campi.size() < 6) return false;
    codice = campi[0];
    
    // Il campo "Nome Cognome" deve essere diviso in "Nome" e "Cognome"
    vector<string> nome_cognome = split(campi[1], ' '); 
    if(nome_cognome.size() < 2) return false;
    nome = nome_cognome[0];
    cognome = nome_cognome[1];
    
    email = campi[2];
    telefono = campi[3];
    
    try {
        eta = stoi(campi[4]);
    } catch (const std::exception& e) {
        cerr << "Errore conversione età (" << campi[4] << "): " << e.what() << endl;
        return false; 
    }

    // Il template stoe restituisce una variabile senza valore in caso di errore
    auto temp = stoe<Tipologia_cliente>(campi[5], ++numero_riga);
    if (!temp.has_value()) return false;
    tipologia = temp.value();
    return true;
}

// Metodo che carica i clienti da un file
bool Gestore_agenzia::carica_clienti(ifstream& file) {
    string linea;
    int numero_riga = 0;
    while (getline(file, linea)) {
        if(linea.empty()) continue;
        
        // Se la linea è vuota passo alla successiva
        if(linea.empty()) {
            cout << "Riga #" << ++numero_riga << ": vuota";
            continue;
        }

        // Dividiamo la stringa nel file in "campi"
        vector<string> campi = split(linea, ';');
        if(campi.empty()) continue;

        string codice, nome, cognome, email, telefono;
        int eta;
        Tipologia_cliente tipologia;

        // Assegnazione dei valori dal file (la riga viene aggiornata in assegna_valore_cliente())
        if(!assegna_valore_cliente(campi, numero_riga, codice, nome, cognome, email, telefono, eta, tipologia)) {
            cerr << "Errore formato riga " << numero_riga << endl;
            continue; // Se ritorna false, passo alla riga successiva
        }

        auto nuovo = Cliente::crea_cliente(codice, nome, cognome, email, telefono, eta, tipologia);
        // Se la creazione dell'elemento avviene correttamente
        if (nuovo && aggiungi_elemento(this->clienti, nuovo)) {
            cout << "Riga #" << numero_riga << ": Cliente " << nome << " " << cognome << " caricato correttamente." << endl;
        }
    }
    return true;
}

// Metodo che carica i pacchetti da un file
bool Gestore_agenzia::carica_pacchetti(ifstream& file) {
    string linea;
    int numero_riga = 0; // Variabile per stampare in quale riga del file compaiono eventuali errori di formato
    
    // Ciclo di caricamento
    while (getline(file, linea)) {
        if(linea.empty()) continue;
        vector<string> campi = split(linea, ';');
        if(campi.size() < 6) continue;

        // Salvo i valori dei campi
        string codice = campi[0];
        string destinazione = campi[1];
        int giorni = 0;
        double prezzo = 0.0;
        try {
            giorni = stoi(campi[2]);
            prezzo = stod(campi[4]);
        } catch(const std::exception& e) {
            cerr << "Errore riga " << numero_riga << ": Giorni o Prezzo non validi (" << e.what() << ")" << endl;
            continue; // Salta questa riga e passa alla prossima
        }
        
        bool disponibile = (campi[3] == "Disponibile");
        string tipologia = campi[5];

        auto it = mappa_caricamento_specifico.find(tipologia); // Cerco la giusta funzione di caricamento
        if (it != mappa_caricamento_specifico.end()) {
            // Se non ci sono errori, applico la funzione di caricamento dati corretta
            it->second(codice, destinazione, giorni, disponibile, prezzo, campi, numero_riga);
        } else {
            cerr << "Tipologia di pacchetto non riconosciuta durante il caricamento: " << tipologia << endl;
        }
    }
    return true;
}

// Metodo che carica le prenotazioni da un file
bool Gestore_agenzia::carica_prenotazioni(ifstream& file) {
    string linea;
    while (getline(file, linea)) {
        if(linea.empty()) continue;
        vector<string> campi = split(linea, ';');
        if(campi.size() < 7) continue;

        // Salvo i valori dei campi
        string codice_prenotazione = campi[0];
        string codice_cliente = campi[1];
        string codice_pacchetto = campi[2];
        int numero_persone = 0;
        try { numero_persone = stoi(campi[3]); } catch(...) { continue; }
        string data_prenotazione = campi[4];
        bool confermata = (campi[5] == "Confermata");
        // double prezzo_totale = stod(campi[6]); // Non usato, si ricalcola

        auto cliente = cerca_cliente(codice_cliente);
        auto pacchetto = cerca_pacchetto(codice_pacchetto);

        if (cliente && pacchetto) {
            // Creo l'elemento e (se non ci sono errori) stampo la conferma della prenotazione
            auto nuova = Prenotazione::crea_prenotazione(codice_prenotazione, cliente, pacchetto, numero_persone, data_prenotazione);
            if (nuova) {
                aggiungi_elemento(this->prenotazioni, nuova);
                
                // Salvo anche la conferma (che non viene inserito nel costruttore)
                if (confermata) nuova->conferma_prenotazione();
                cout << "Prenotazione " << codice_prenotazione << " per il cliente " << codice_cliente << " e il pacchetto " << codice_pacchetto << " caricata correttamente" << endl;
            }
        }
    }
    return true;
}

// === COSTRUTTORE E DISTRUTTORE ===
Gestore_agenzia::Gestore_agenzia(vector<shared_ptr<Pacchetto_viaggio>> cat, vector<shared_ptr<Cliente>> cli,
                    vector<shared_ptr<Prenotazione>> pre) :
    catalogo(cat), clienti(cli), prenotazioni(pre) {
        
        if(!inizializza_mappa()) {
            cerr << "ATTENZIONE: Errore durante l'inizializzazione del sistema di caricamento." << endl;
        }
        cout << "Gestione azienda inizializzata correttamente!" << endl;
}

Gestore_agenzia::~Gestore_agenzia() {
    cout << "Gestore azienda distrutto correttamente." << endl;
}

// === METODI PUBBLICI DI AGGIUNTA ===

bool Gestore_agenzia::aggiungi_pacchetto() {
    do{
        string codice; string dest; int giorni; double prezzo;
        // Inserimento interattivo dei dati del pacchetto di base
        if (!inserisci_dati_pacchetto_base(dest, giorni, prezzo)) return false;
        
        codice = genera_codice_unico('P');

        int scelta = menu::stampa_menu_e_scelta({"MENU TIPOLOGIA PACCHETTO","Turismo Avventura","Turismo Balneare","Turismo Montano","Città d'Arte"});
        bool flag = false;

        // Inserimento interattivo dei dati del pacchetto specifico
        switch (scelta) {
            case 1: {
                vector<string> lista_attivita; Categoria_adrenalina categoria; bool assicurazione;
                if (!inserisci_dati_pacchetto(lista_attivita, categoria, assicurazione)) { // Errore -> reinserimento?
                    if (menu::menu_reinserimento() == 2) return false; else continue;
                }
                auto nuovo = Pacchetto_avventura::crea_pacchetto(codice, dest, giorni, prezzo, lista_attivita, categoria, assicurazione); // Creazione dell'elemento
                if (nuovo) flag = (aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo)) != nullptr);
                break;
            }
            case 2: {
                bool omb, attr; Categoria_pensione tipo;
                if (!inserisci_dati_pacchetto(omb, attr, tipo)) { // Errore -> reinserimento?
                    if (menu::menu_reinserimento() == 2) return false; else continue;
                }
                auto nuovo = Pacchetto_mare::crea_pacchetto(codice, dest, giorni, prezzo, omb, attr, tipo); // Creazione dell'elemento
                if (nuovo) flag = (aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo)) != nullptr);
                break;
            }
            case 3: {
                bool skip; int esc; Categoria_difficolta diff;
                if (!inserisci_dati_pacchetto(skip, esc, diff)) { // Errore -> reinserimento?
                    if (menu::menu_reinserimento() == 2) return false; else continue;
                }
                auto nuovo = Pacchetto_montagna::crea_pacchetto(codice, dest, giorni, prezzo, skip, esc, diff); // Creazione dell'elemento
                if (nuovo) flag = (aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo)) != nullptr);
                break;
            }
            default: {
                int mus; bool gui; Categoria_hotel hotel;
                if (!inserisci_dati_pacchetto(mus, gui, hotel)) { // Errore -> reinserimento?
                    if (menu::menu_reinserimento() == 2) return false; else continue;
                }
                auto nuovo = Pacchetto_citta::crea_pacchetto(codice, dest, giorni, prezzo, mus, gui, hotel); // Creazione dell'elemento
                if (nuovo) flag = (aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo)) != nullptr);
                break;
            }
        }

        // Controllo sulla creazione dell'elemento
        if(flag) {
            cout << "Pacchetto " << codice << " aggiunto correttamente al sistema." << endl;
            return true;
        }
    } while(1);
}

// Wrapper metodi ricerca e visualizzazione
shared_ptr<Pacchetto_viaggio> Gestore_agenzia::cerca_pacchetto(string codice) {
    return cerca_elemento(this->catalogo, codice);
}

bool Gestore_agenzia::visualizza_catalogo() const {
    return stampa_elementi(this->catalogo);
}

// Metodo che ritorna true se la stampa è andata a buon fine, false altrimenti
bool Gestore_agenzia::visualizza_pacchetti_per_tipologia(string tipo) const {
    bool flag = false;
    // Ciclo di stampa dei dettagli
    for (const auto& elemento : this->catalogo) {
        if (etos(elemento->get_tipologia()) == tipo) { // Controllo che la tipologia sia corretta
            elemento->stampa_dettagli(); 
            flag = true;
        }
    }
    if (!flag) cerr << "Errore! Non ci sono elementi della tipologia inserita!" << endl;
    return flag;
}

bool Gestore_agenzia::visualizza_pacchetti_disponibili() const {
    bool found = false;
    for (const auto& pacchetto : catalogo) {
        if (pacchetto->is_disponibile()) { // Controllo che il pacchetto sia disponibile
            cout << pacchetto->stampa_dettagli() << endl;
            found = true;
        }
    }
    return found;
}

bool Gestore_agenzia::aggiungi_cliente() {
    do {
        string nome, cognome, email, telefono, tipo_str;
        int eta;
        Tipologia_cliente tipo;
        
        // Controllo di errori nell'inserimento dei dati
        if (!inserimento_dati_cliente(nome, cognome, email, telefono, eta, tipo_str, tipo)) {
            if (menu::menu_reinserimento() == 1) continue; else return false;
        }

        // Controllo di errori nella creazione del cliente
        auto nuovo = Cliente::crea_cliente(genera_codice_unico('C'), nome, cognome, email, telefono, eta, tipo);
        if (nuovo && aggiungi_elemento(this->clienti, nuovo)) {
            cout << "Cliente " << nome << " " << cognome << " aggiunto correttamente al sistema." << endl;
            return true;
        } else {
            if (menu::menu_reinserimento() == 2) return false;
        }
    } while (1);
}

shared_ptr<Cliente> Gestore_agenzia::cerca_cliente(string codice) {
    return cerca_elemento(this->clienti, codice);
}

bool Gestore_agenzia::visualizza_clienti() const {
    return stampa_elementi(this->clienti);
}

bool Gestore_agenzia::visualizza_clienti_per_tipologia(string tipo) const {
    bool flag = false;
    // Ciclo di stampa dettagli
    for (const auto& elemento : this->clienti) {
        if (etos(elemento->get_tipologia()) == tipo) { // Controllo che la tipologia sia corretta
            elemento->stampa_dettagli();
            flag = true;
        }
    }
    if (!flag) cerr << "Errore! Non ci sono elementi della tipologia inserita!" << endl;
    return flag;
}

bool Gestore_agenzia::aggiungi_prenotazione() {
    do {
        shared_ptr<Cliente> cliente; shared_ptr<Pacchetto_viaggio> pacchetto_viaggio; 
        int num_persone; string data;
        
        // Inserimento interattivo dei dati. Se c'è errore è possibile reinserire i dati.
        if (!inserimento_dati_prenotazione(cliente, pacchetto_viaggio, num_persone, data)) {
            if (menu::menu_reinserimento() == 2) return false; // Se scelta è 2 l'utente ha scelto di annullare l'operazione
            else continue; // Se la scelta è 1 l'utente ha scelto di riprovare (-> continue)
        }

        // Creazione della prenotazione. Se ci sono errori è possibile reinserire i dati.
        auto nuova = Prenotazione::crea_prenotazione(genera_codice_unico('B'), cliente, pacchetto_viaggio, num_persone, data);
        if (nuova && aggiungi_elemento(this->prenotazioni, nuova)) {
            cout << "Prenotazione per il cliente " << cliente->get_nome_completo() << " aggiunta correttamente al sistema." << endl;
            return true;
        } else {
            if (menu::menu_reinserimento() == 2) return false; // Se scelta è 2 l'utente ha scelto di annullare l'operazione
            // Se la scelta è 1 l'utente ha scelto di riprovare -> lascio finire il ciclo
        }
    } while (1);
}

// Metodo che conferma la prenotazione che possiede il codice inserito in input
// Questo metodo ritorna true se la modifica è stata apportata, altrimenti ritorna false (se il codice era già stato confermato ritorna false)
bool Gestore_agenzia::conferma_prenotazione(string codice) {
    shared_ptr<Prenotazione> prenotazione = cerca_elemento(this->prenotazioni, codice);
    if (prenotazione != nullptr) { // Controllo errori di ricerca
        if (!prenotazione->conferma_prenotazione()) { // Controllo se il codice è già confermato
            cerr << "Errore! Codice già confermato in precedenza!" << codice << "." << endl;
            return false;
        } else return true;
    } else {
        cerr << "Elemento con codice " << codice << " non trovato." << endl;
        return false;
    }
}

bool Gestore_agenzia::visualizza_prenotazioni() const {
    return stampa_elementi(this->prenotazioni);
}

// Metodo che stampa le prenotazione del cliente associato al codiceCliente inserito in input
bool Gestore_agenzia::visualizza_prenotazioni_cliente(string codice_cliente) const {
    int i = 0;
    bool found = false;
    // Ciclo su tutte le prenotazioni, con filtro su quelli associati al giusto codice
    for (const auto& prenotazione : prenotazioni) {
        if (prenotazione->get_cliente()->get_codice() == codice_cliente) { // Filtro
            cout << "Prenotazione #" << ++i << " del cliente " << prenotazione->get_cliente()->get_nome_completo() << ":" << endl;
            prenotazione->stampa_dettagli();
            found = true;
        }
    }
    if (!found) cout << "Cliente con codice " << codice_cliente << " non trovato o senza prenotazioni." << endl;
    return found;
}

bool Gestore_agenzia::visualizza_prenotazioni_confermate() const {
    int i = 0;
    bool flag = false;

    // Ciclo su tutte le prenotazioni, con filtro su quelle confermate
    for (const auto& prenotazione : prenotazioni) {
        if (prenotazione->is_confermata()) { // Filtro
            cout << "Prenotazione confermata #" << ++i << ":" << endl;
            flag = prenotazione->stampa_dettagli();
        }
    }
    if (!flag) cerr << "Non ci sono prenotazioni confermate!" << endl;
    return flag;
}

// === STATISTICHE ===

// Metodo che stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Pacchetto più prenotato, Destinazione più richiesta
bool Gestore_agenzia::statistiche_generali() const {

    // Controllo errore nei pacchetti
    auto flag_pacchetti = calcola_massimo_mappa(Prenotazione::get_pacchetti_counter());    
    if (!flag_pacchetti.has_value()) {cout << "Non ci sono pacchetti nel database" << endl; return false;}

    auto flag_destinazioni = calcola_massimo_mappa(Prenotazione::get_destinazioni_counter());
    if (!flag_destinazioni.has_value()) {cout << "Non ci sono pacchetti nel database" << endl; return false;}

    // Stampa di tutto
    cout << "=== Statistiche Generali Agenzia Viaggi ===" << endl;
    cout << "Totale Pacchetti nel Catalogo: " << Pacchetto_viaggio::get_pacchetti_creati() << endl;
    cout << "Totale Clienti Registrati: " << Cliente::get_clienti_creati() << endl;
    cout << "Totale Prenotazioni Effettuate: " << Prenotazione::get_prenotazioni_create() << endl;
    cout << "Fatturato Totale: " << Prenotazione::get_fatturato_totale() << " EUR" << endl;
    cout << "Pacchetto più prenotato: " << flag_destinazioni.value() << endl;
    cout << "Destinazione più richiesta: " << flag_pacchetti.value() << endl;
    return true;
}
    
// Metodo che stampa il numero di prenotazioni per ogni tipologia di pacchetto
bool Gestore_agenzia::statistiche_per_tipologia() const {
    cout << "=== Numero di prenotazioni per ogni Tipologia di Pacchetto ===" << endl;
    
    // Ciclo su tutti gli elementi della mappa (chiave: tipologia, valore: numero di prenotazioni)
    for (const auto& campo : Prenotazione::get_pacchetti_counter()) {
        cout << "Tipologia: " << campo.first << " - Prenotazioni: " << campo.second << endl;
    }
    return true;
}

// Ritorna il cliente che ha speso di più
shared_ptr<Cliente> Gestore_agenzia::cliente_migliore() const {

    // Controllo errore nel calcolo sulla mappa
    auto flag = calcola_massimo_mappa(Prenotazione::get_spesa_clienti());
    if (!flag.has_value()) {cout << "Non ci sono clienti nel database"; return nullptr;}
    cout << "Cliente che ha speso di più: " << flag.value() << endl;
    return cerca_cliente(flag.value());
}

// === GESTIONE FILE ===
// Salva/Carica lo stato intero dell'agenzia (CSV simulato)
bool Gestore_agenzia::salva_dati_su_file(const string& nomefile, string& tipo) {
    ofstream file(nomefile);
    if (!file) {cerr << "Errore nell'apertura del file per la scrittura: " << nomefile << endl; return false;}

    string t = tolower_string(tipo);

    // Salvataggio clienti
    if (t == "cliente" || t == "clienti") {
        for (const auto& cliente : this->clienti) {
            cliente->salva_dati_su_file(file);    
        }
        
    // Salvataggio pacchetti
    } else if (t == "pacchetto" || t == "pacchetti") {
        for (const auto& pacchetto : this->catalogo) {
            pacchetto->salva_dati_su_file(file);
        }
    
    // Salvataggio prenotazioni
    } else if (t == "prenotazione" || t == "prenotazioni") {
        for (const auto& prenotazione : this->prenotazioni) {
            prenotazione->salva_dati_su_file(file);
        }

    } else {
        cerr << "Tipo di dato non riconosciuto per il salvataggio: " << tipo << endl;
        file.close();
        return false;
        }

    file.close();
    return true;
}

bool Gestore_agenzia::carica_dati_da_file(const string& nomefile, const string& tipo) {
    ifstream file(nomefile);
    if (!file) { cerr << "Errore nell'apertura del file per la lettura: " << nomefile << endl; return false; }

    string t = tipo;
    t = tolower_string(t);

    // Caricamento clienti
    if (t == "clienti" || t == "cliente") {
        carica_clienti(file);    
    
    // Caricamento pacchetti
    } else if (t == "pacchetti" || t == "pacchetto") {
        carica_pacchetti(file);
        
    // Caricamento prenotazioni
    } else if (t == "prenotazioni" || t == "prenotazione") {
        carica_prenotazioni(file);

    } else {
        cerr << "Tipo di dato non riconosciuto per il caricamento." << endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}
