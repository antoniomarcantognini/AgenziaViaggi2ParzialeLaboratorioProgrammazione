#include "magic_enum.hpp"
#include "Gestore_azienda.h"
#include "Utils_enum.h"
#include "Esito_input_SN.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>
#include "menu.h"

using namespace std;
using namespace magic_enum;

// === IMPLEMENTAZIONE METODI HELPER PRIVATI (STATIC) ===
// Helper per split stringa
vector<string> Gestore_azienda::split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Funzione che restituisce SI se il valore inserito è 's'/'S', NO se il valore inserito è 'n'/'N', RIPROVA se si vuole riprovare l'inserimento, ANNULLA altrimenti.
Esito_input_SN Gestore_azienda::analizza_input_sn(const string& input) {
    if (input.empty()) return ANNULLA;
    char c = tolower(input[0]);
    if (c == 's') return SI;
    if (c == 'n') return NO;

    cerr << "Valore inserito non valido: puoi inserire solo 'S' o 'N'." << endl;
    // Reinserimento
    int scelta = menu::menu_reinserimento();
    if(scelta == 1) return RIPROVA;
    else return ANNULLA;
}

// Funzione che valida l'inserimento di 'S' e 'N'. Ritorna false se è stato annullato l'inserimento, 
// continua il ciclo se si vuole riprovare, altrimenti ritorna true e aggiorna flag_da_aggiornare.
// flag_da_aggiornare = true se è stato inserito 'S'/'s'
// flag_da_aggiornare = false se è stato inserito 'N'/'n'
bool Gestore_azienda::valida_inserimento_sn(string& stringa_s_n, bool& flag_da_aggiornare) {
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

// === IMPLEMENTAZIONE METODI MEMBRI DI Gestore_azienda ===

string Gestore_azienda::tolower_string(string stringa){
    for (char &c : stringa) {
        c = tolower((unsigned char)c);
    }
    return stringa;
}

// Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0001")
string Gestore_azienda::genera_codice_unico(char tipo) {
    int numero = 0;
    string prefisso;

    switch (tipo) {
        case 'C': // Cliente
            numero = Cliente::get_clienti_creati() + 1;
            prefisso = "CLT-";
            break;
        case 'P': // Pacchetto
            numero = Pacchetto_viaggio::get_pacchetti_creati() + 1;
            prefisso = "PKG-";
            break;
        case 'B': // Prenotazione (Booking)
            numero = Prenotazione::get_prenotazioni_create() + 1;
            prefisso = "BKG-";
            break;
        default:
            cerr << "Errore generazione codice: tipo sconosciuto." << endl;
            return "ERR";
    }

    // Usiamo stringstream per formattare il numero con 4 cifre (es. 0005)
    stringstream ss;
    ss << prefisso << std::setw(4) << std::setfill('0') << numero;
    
    return ss.str();
}

// Metodo di inizializzazione della mappa
bool Gestore_azienda::inizializza_mappa() {
    try {
        // Lambda per caricamento Avventura
        mappa_caricamento_specifico["Turismo Avventura"] = [this](string& codice, string& dest, int& giorni, bool& disp, double& prezzo, const vector<string>& campi, int& riga) -> bool {
            vector<string> lista;
            auto cat_opt = Utils_enum::stoe<Categoria_adrenalina>(campi[6], ++riga);
            if (!cat_opt) return false; // Controllo errori
            
            bool ass = (campi[7] == "Con Assicurazione");
            for (size_t i = 8; i < campi.size(); ++i) {
                if(!campi[i].empty()) lista.push_back(campi[i]);
            }
            
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
            auto pens_opt = Utils_enum::stoe<Categoria_pensione>(campi[8], ++riga);
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
            int esc = 0;
            
            try{
                int esc = stoi(campi[7]);  // Prova a convertire a intero
            }
            catch(const std::exception& e){
                cerr << "Errore riga " << riga << ": Il campo 'escursioni' (" << campi[7] << ") non e' valido (" << e.what() << ")."<< endl;
                return false;
            }
            
            auto diff_opt = Utils_enum::stoe<Categoria_difficolta>(campi[8], ++riga);
            if (!diff_opt) return false; // Controllo errori

            auto nuovo = Pacchetto_montagna::crea_pacchetto(codice, dest, giorni, prezzo, skip, esc, diff_opt.value());
            if (nuovo && aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo))) {
                cout << "Pacchetto Montagna " << codice << " caricato correttamente." << endl;
                return true;
            }
            return false;
        };

        // Lambda per caricamento Città
        mappa_caricamento_specifico["Citta' d'Arte"] = [this](string& codice, string& dest, int& giorni, bool& disp, double& prezzo, const vector<string>& campi, int& riga) -> bool {
            int musei = 0;
            try {
                musei = stoi(campi[6]); // Prova a convertire a intero
            } 
            catch (const std::exception& e) { 
                cerr << "Errore riga " << riga << ": Il campo 'musei' (" << campi[6] << ") non e' valido (" << e.what() << ")." << endl;
                return false; 
            }

            bool guida = (campi[7] == "Con Guida");
            
            auto hotel_opt = Utils_enum::stoe<Categoria_hotel>(campi[8], ++riga);
            if (!hotel_opt) return false; 

            auto nuovo = Pacchetto_citta::crea_pacchetto(codice, dest, giorni, prezzo, musei, guida, hotel_opt.value());
            if (nuovo && aggiungi_elemento(this->catalogo, dynamic_pointer_cast<Pacchetto_viaggio>(nuovo))) {
                cout << "Pacchetto Citta' " << codice << " caricato correttamente." << endl;
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

// === METODI DI INPUT DATI  ===

// Funzione di inserimento manuale dei dati da tastiera per pacchetti generico
bool Gestore_azienda::inserisci_dati_pacchetto_base(string& dest, int& giorni, double& prezzo) {
    cout << "=== INSERIMENTO DATI NUOVO PACCHETTO ===" << endl;
    cout << "Destinazione: ";
    getline(cin, dest);

    cout << "Durata in giorni: ";
    if (!(cin >> giorni)) return false;

    cout << "Prezzo base: ";
    if (!(cin >> prezzo)) return false;

    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti avventura
bool Gestore_azienda::inserisci_dati_pacchetto(vector<string>& lista_attivita, Categoria_adrenalina& categoria, bool& assicurazione) {
    cout << "Numero di attivita' incluse: ";
    int num_attivita;
    if (!(cin >> num_attivita)) return false;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // pulizia del buffer
    lista_attivita.clear();
    for (int i = 0; i < num_attivita; ++i) {
        string attivita;
        cout << "Attivita' #" << (i + 1) << ": ";
        getline(cin, attivita);
        lista_attivita.push_back(attivita);
    }

    cout << "Categoria Adrenalina (scrivere ESATTAMENTE Basso, Medio o Alto): ";
    string categoria_str;
    cin >> categoria_str;
    auto temp = Utils_enum::stoe<Categoria_adrenalina>(categoria_str);
    if (!temp.has_value()) return false; // Controllo errori
    else categoria = temp.value();

    cout << "Assicurazione inclusa? (S/N): ";
    string stringa_ass;
    if(!valida_inserimento_sn(stringa_ass, assicurazione)) return false; // Controllo se l'utente ha deciso di annullare l'operazione
    
    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti mare
bool Gestore_azienda::inserisci_dati_pacchetto(bool& ombrellone, bool& attrezzatura, Categoria_pensione& tipo) {
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
    auto temp = Utils_enum::stoe<Categoria_pensione>(string_pensione);
    if (!temp.has_value()) return false; // Controllo errori
    else tipo = temp.value();
    
    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti montagna
bool Gestore_azienda::inserisci_dati_pacchetto(bool& skipass_incluso, int& num_escursioni, Categoria_difficolta& difficolta) {
    cout << "Skipass incluso? (S/N): ";
    string stringa_skip;
    if(!valida_inserimento_sn(stringa_skip, skipass_incluso)) return false;

    cout << "Numero di escursioni: ";
    if (!(cin >> num_escursioni)) return false;

    cout << "Difficolta' (inserire ESATTAMENTE Facile, Media o Difficile): ";
    string stringa_diff;
    cin >> stringa_diff;
    auto temp = Utils_enum::stoe<Categoria_difficolta>(stringa_diff);
    if (!temp.has_value()) return false; // Controllo errore
    else difficolta = temp.value();

    return true;
}

// Funzione di inserimento manuale dei dati da tastiera per pacchetti città
bool Gestore_azienda::inserisci_dati_pacchetto(int& num_musei, bool& guida, Categoria_hotel& hotel) {
    cout << "Numero di musei: ";
    if (!(cin >> num_musei)) return false;

    cout << "Guida inclusa? (S/N): ";
    string stringa_guida;
    if (!valida_inserimento_sn(stringa_guida, guida)) return false;

    cout << "Hotel (inserire esattamente Tre stelle, Quattro stelle o Cinque stelle): ";
    string stringa_hotel;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, stringa_hotel);
    auto temp = Utils_enum::stoe<Categoria_hotel>(stringa_hotel);
    if (!temp.has_value()) return false;
    else hotel = temp.value();

    return true;
}

// Funzione inserimento dati cliente
bool Gestore_azienda::inserimento_dati_cliente(string& nome, string& cognome, string& email, string& telefono,
    int& eta, string& tipo_str, Tipologia_cliente& tipo) {
    
    cout << "=== INSERIMENTO DATI NUOVO CLIENTE ===" << endl;
    cout << "Nome: ";
    getline(cin, nome);

    cout << "Cognome: ";
    getline(cin, cognome); 

    cout << "Email (deve contenere @!): ";
    cin >> email;

    cout << "Telefono (10 cifre!): ";
    cin >> telefono;

    cout << "Eta' (deve essere maggiore di 0): ";
    if (!(cin >> eta)) return false;

    cout << "Tipologia Cliente (scrivere ESATTAMENTE Standard, Premium o VIP): ";
    cin >> tipo_str;
    auto temp = Utils_enum::stoe<Tipologia_cliente>(tipo_str);
    if (!temp.has_value()) return false;
    else tipo = temp.value();
    return true;
}

// Funzione inserimento dati prenotazione
bool Gestore_azienda::inserimento_dati_prenotazione(shared_ptr<Cliente>& cliente, shared_ptr<Pacchetto_viaggio>& pacchetto_viaggio,
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
bool Gestore_azienda::assegna_valore_cliente(vector<string>& campi, int& numero_riga, string& codice, string& nome, string& cognome, string& email, string& telefono, int& eta, Tipologia_cliente& tipologia) {
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
        cerr << "Errore conversione eta' (" << campi[4] << "): " << e.what() << endl;
        return false; 
    }

    // Il template stoe restituisce una variabile senza valore in caso di errore
    auto temp = Utils_enum::stoe<Tipologia_cliente>(campi[5], ++numero_riga);
    if (!temp.has_value()) return false;
    tipologia = temp.value();
    return true;
}

// Metodo che carica i clienti da un file
bool Gestore_azienda::carica_clienti(ifstream& file) {
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
bool Gestore_azienda::carica_pacchetti(ifstream& file) {
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
bool Gestore_azienda::carica_prenotazioni(ifstream& file) {
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
Gestore_azienda::Gestore_azienda(vector<shared_ptr<Pacchetto_viaggio>> cat, vector<shared_ptr<Cliente>> cli,
                    vector<shared_ptr<Prenotazione>> pre) :
    catalogo(cat), clienti(cli), prenotazioni(pre) {
        
        if(!inizializza_mappa()) {
            cerr << "ATTENZIONE: Errore durante l'inizializzazione del sistema di caricamento." << endl;
        }
        cout << "Gestione azienda inizializzata correttamente!" << endl;
}

Gestore_azienda::~Gestore_azienda() {
    cout << "Gestore azienda distrutto correttamente." << endl;
}

// === METODI PUBBLICI DI AGGIUNTA ===

bool Gestore_azienda::aggiungi_pacchetto() {
    do{
        string codice; string dest; int giorni; double prezzo;
        // Inserimento interattivo dei dati del pacchetto di base
        if (!inserisci_dati_pacchetto_base(dest, giorni, prezzo)) return false;
        
        codice = genera_codice_unico('P');

        int scelta = menu::stampa_menu_e_scelta({"MENU TIPOLOGIA PACCHETTO","Turismo Avventura","Turismo Balneare","Turismo Montano","Citta' d'Arte"});
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
shared_ptr<Pacchetto_viaggio> Gestore_azienda::cerca_pacchetto(string codice) const {
    return cerca_elemento(this->catalogo, codice);
}

bool Gestore_azienda::visualizza_catalogo() const {
    return stampa_elementi(this->catalogo);
}

// Metodo che ritorna true se la stampa è andata a buon fine, false altrimenti
bool Gestore_azienda::visualizza_pacchetti_per_tipologia(string tipo) const {
    bool flag = false;
    // Ciclo di stampa dei dettagli
    for (const auto& elemento : this->catalogo) {
        if (elemento->get_tipologia() == tipo) { // Controllo che la tipologia sia corretta
            elemento->stampa_dettagli(); 
            flag = true;
        }
    }
    if (!flag) cerr << "Errore! Non ci sono elementi della tipologia inserita!" << endl;
    return flag;
}

bool Gestore_azienda::visualizza_pacchetti_disponibili() const {
    bool found = false;
    for (const auto& pacchetto : catalogo) {
        if (pacchetto->is_disponibile()) { // Controllo che il pacchetto sia disponibile
            cout << pacchetto->stampa_dettagli() << endl;
            found = true;
        }
    }
    return found;
}

bool Gestore_azienda::aggiungi_cliente() {
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

shared_ptr<Cliente> Gestore_azienda::cerca_cliente(string codice) const {
    return cerca_elemento(this->clienti, codice);
}

bool Gestore_azienda::visualizza_clienti() const {
    return stampa_elementi(this->clienti);
}

bool Gestore_azienda::visualizza_clienti_per_tipologia(string tipo) const {
    bool flag = false;
    // Ciclo di stampa dettagli
    for (const auto& elemento : this->clienti) {
        if (Utils_enum::etos(elemento->get_tipologia()) == tipo) { // Controllo che la tipologia sia corretta
            elemento->stampa_dettagli();
            flag = true;
        }
    }
    if (!flag) cerr << "Errore! Non ci sono elementi della tipologia inserita!" << endl;
    return flag;
}

bool Gestore_azienda::aggiungi_prenotazione() {
    do {
        shared_ptr<Cliente> cliente; shared_ptr<Pacchetto_viaggio> pacchetto_viaggio; 
        int num_persone; string data;
        
        // Inserimento interattivo dei dati. Se c'è errore è possibile reinserire i dati.
        if (!inserimento_dati_prenotazione(cliente, pacchetto_viaggio, num_persone, data)) {
            if (menu::menu_reinserimento() == 2) return false; // Se scelta è 2 l'utente ha scelto di annullare l'operazione
            else continue; // Se la scelta è 1 l'utente ha scelto di riprovare (-> continue)
        }
        // Creazione della prenotazione
        auto nuova = Prenotazione::crea_prenotazione(genera_codice_unico('B'), cliente, pacchetto_viaggio, num_persone, data);
        
        if (nuova != nullptr) {
            //Calcolo PREZZO LISTINO dal Pacchetto
            double costo_base = pacchetto_viaggio->calcola_prezzo_finale();
            double costo_lordo = costo_base * num_persone;

            // Calcolo PREZZO SCONTATO dal Cliente
            double costo_netto = cliente->applica_sconto(costo_lordo);

            nuova->set_prezzo_totale(costo_netto);
            cout << "--- Riepilogo Costi ---" << endl;
            cout << "Totale Lordo: " << costo_lordo << " EUR" << endl;
            cout << "Totale Netto (Sconto " << Utils_enum::etos(cliente->get_tipologia()) << "): " << costo_netto << " EUR" << endl;
        }
        
        if (nuova && aggiungi_elemento(this->prenotazioni, nuova)) {
            cout << "Prenotazione per il cliente " << cliente->get_nome_completo() << " aggiunta correttamente al sistema." << endl;
            cout << "Prezzo totale calcolato e salvato: " << nuova->get_prezzo_totale() << " EUR" << endl;
            return true;
        } else {
            cout << "Errore nell'aggiunta della prenotazione." << endl;
            if (menu::menu_reinserimento() == 2) return false; // Se scelta è 2 l'utente ha scelto di annullare l'operazione
            // Se la scelta è 1 l'utente ha scelto di riprovare -> lascio finire il ciclo
        }
    } while (1);
}

// Metodo che conferma la prenotazione che possiede il codice inserito in input
// Questo metodo ritorna true se la modifica è stata apportata, altrimenti ritorna false (se il codice era già stato confermato ritorna false)
bool Gestore_azienda::conferma_prenotazione(string codice) {
    shared_ptr<Prenotazione> prenotazione = cerca_elemento(this->prenotazioni, codice);
    if (prenotazione != nullptr) { // Controllo errori di ricerca
        if (!prenotazione->conferma_prenotazione()) { // Controllo se il codice è già confermato
            cerr << "Errore! Codice gia' confermato in precedenza!" << codice << "." << endl;
            return false;
        } else return true;
    } else {
        cerr << "Elemento con codice " << codice << " non trovato." << endl;
        return false;
    }
}

bool Gestore_azienda::visualizza_prenotazioni() const {
    if (this->prenotazioni.empty()) return false;

    int counter = 0;
    for (const auto& prenotazione : this->prenotazioni) {
        if (prenotazione != nullptr) {
            counter++;

            cout << "\n========================================" << endl;
            cout << "       PRENOTAZIONE #" << counter << endl;
            cout << "========================================" << endl;

            prenotazione->stampa_dettagli();
        }
    }
    cout << "\n(Fine lista prenotazioni)" << endl;
    return true;
}

// Metodo che stampa le prenotazione del cliente associato al codiceCliente inserito in input
bool Gestore_azienda::visualizza_prenotazioni_cliente(string codice_cliente) const {
    int i = 0;
    bool found = false;
    // Ciclo su tutte le prenotazioni, con filtro su quelli associati al giusto codice
    for (const auto& prenotazione : prenotazioni) {
        if (prenotazione->get_cliente()->get_codice() == codice_cliente) { // Filtro
            i++;
            cout << "\n----------------------------------------" << endl;
            cout << "Prenotazione #" << i << " del cliente " << prenotazione->get_cliente()->get_nome_completo() << ":" << endl;
            prenotazione->stampa_dettagli();
            found = true;
        }
    }
    if (!found) cout << "Cliente con codice " << codice_cliente << " non trovato o senza prenotazioni." << endl;
    return found;
}

bool Gestore_azienda::visualizza_prenotazioni_confermate() const {
    int i = 0;
    bool flag = false;

    // Ciclo su tutte le prenotazioni, con filtro su quelle confermate
    for (const auto& prenotazione : prenotazioni) {
        if (prenotazione->is_confermata()) { 
            i++;
            cout << "\n****************************************" << endl;
            cout << "Prenotazione confermata #" << i << ":" << endl;
            prenotazione->stampa_dettagli();
            flag = true;
        }
    }
    if (!flag) cerr << "Non ci sono prenotazioni confermate!" << endl;
    return flag;
}

// === STATISTICHE ===

// Metodo per sincronizzare tutte le statistiche di database caricati
bool Gestore_azienda::sincronizza_statistiche() {
    if (this->prenotazioni.empty()) {
        cout << "Attenzione: Nessuna prenotazione in memoria da sincronizzare." << endl;
        return false;
    }

    // Resetta le statistiche vecchie (per evitare duplicati)
    if (!Prenotazione::reset_statistiche()) {
        cout << "Errore nel reset delle statistiche." << endl;
        return false;
    }

    // Itera sul vettore (che ora è pieno grazie al load da file) e ripopola le mappe statiche
    bool stato_aggiornamento = true;
    for (const auto& prenot : this->prenotazioni) {
        if (prenot == nullptr) continue; // Salta puntatori nulli per sicurezza

        auto pacchetto_ptr = prenot->get_pacchetto();
        if(pacchetto_ptr != nullptr){
            // Recupera i dati
            string dest = pacchetto_ptr->get_destinazione();
            string pacc = pacchetto_ptr->get_tipologia();
            double costo = prenot->get_prezzo_totale();
            string codice_cli = prenot->get_cliente()->get_codice();

            // Chiama il metodo statico di aggiornamento
            if (!Prenotazione::aggiorna_statistiche_da_load(dest, pacc, costo, codice_cli)) {
                stato_aggiornamento = false;
            }
        }
    }

    // Aggiorna i contatori totali delle altre classi
    if (!Prenotazione::set_prenotazioni_create(this->prenotazioni.size())) {
        stato_aggiornamento = false;
    }

    if (!Cliente::set_clienti_creati(this->clienti.size())) {
        stato_aggiornamento = false;
    }

    if (!Pacchetto_viaggio::set_pacchetti_creati(this->catalogo.size())) {
        stato_aggiornamento = false;
    }

    cout << "Statistiche sincronizzate con successo." << endl;
    return stato_aggiornamento;
}

// Metodo che stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Pacchetto più prenotato, Destinazione più richiesta
bool Gestore_azienda::statistiche_generali() const {

    // Controllo errore nei pacchetti
    auto flag_pacchetti = calcola_massimo_mappa(Prenotazione::get_pacchetti_counter());    
    if (!flag_pacchetti.has_value()) {cout << "Non ci sono pacchetti nel database" << endl; return false;}

    // Stampa di tutto
    cout << "=== Statistiche Generali Azienda Viaggi ===" << endl;
    cout << "Totale Pacchetti nel Catalogo: " << Pacchetto_viaggio::get_pacchetti_creati() << endl;
    cout << "Totale Clienti Registrati: " << Cliente::get_clienti_creati() << endl;
    cout << "Totale Prenotazioni Effettuate: " << Prenotazione::get_prenotazioni_create() << endl;
    cout << "Fatturato Totale: " << Prenotazione::get_fatturato_totale() << " EUR" << endl;
        
    auto flag_destinazioni = calcola_massimo_mappa(Prenotazione::get_destinazioni_counter());

    if (flag_pacchetti.has_value()) {
        cout << "Pacchetto piu' prenotato: " << flag_pacchetti.value() << endl;
    } else {
        cout << "Pacchetto piu' prenotato: N/A (Nessuna prenotazione)" << endl;
    }

    if (flag_destinazioni.has_value()) {
        cout << "Destinazione piu' richiesta: " << flag_destinazioni.value() << endl;
    } else {
        cout << "Destinazione piu' richiesta: N/A (Nessuna prenotazione)" << endl;
    }

    // Se mancano dati, ritorniamo comunque true perché abbiamo stampato i parziali, 
    // oppure false se vuoi considerare il db vuoto come errore critico.
    if (!flag_pacchetti.has_value() && !flag_destinazioni.has_value()) return false;

    return true;
}
    
// Metodo che stampa il numero di prenotazioni per ogni tipologia di pacchetto
bool Gestore_azienda::statistiche_per_tipologia() const {
    cout << "=== Numero di prenotazioni per ogni Tipologia di Pacchetto ===" << endl;
    
    auto mappa_counter = Prenotazione::get_pacchetti_counter();

    if (mappa_counter.empty()) {
        cout << "Nessuna prenotazione registrata." << endl;
        return false;
    }

    for (const auto& campo : mappa_counter) {
        cout << "Pacchetto/Tipo: " << campo.first << " - Prenotazioni: " << campo.second << endl;
    }
    return true;
}

// Ritorna il cliente che ha speso di più
shared_ptr<Cliente> Gestore_azienda::cliente_migliore() const {

    // Controllo errore nel calcolo sulla mappa
    auto flag = calcola_massimo_mappa(Prenotazione::get_spesa_clienti());
    if (!flag.has_value()) {cout << "Non ci sono clienti nel database"; return nullptr;}
    cout << "Cliente che ha speso di piu': " << flag.value() << endl;
    return cerca_cliente(flag.value());
}

// === GESTIONE FILE ===
// Salva/Carica lo stato intero dell'azienda (CSV simulato)
bool Gestore_azienda::salva_dati_su_file(const string& nomefile, string& tipo) {
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

bool Gestore_azienda::carica_dati_da_file(const string& nomefile, const string& tipo) {
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
