#include "magic_enum.hpp"
#include "Gestore_azienda.h"
#include "Esito_input_SN.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "menu.h"

using namespace std;
using namespace magic_enum;
using namespace Esito_input_SN;

namespace {

    // Funzione che restituisce SI se il valore inserito è 's'/'S', NO se il valore inserito è 'n'/'N', RIPROVA se si vuole riprovare l'inserimento, ANNULLA altrimenti.
    Esito_input_SN analizza_input_sn(const string& input) {
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
    auto valida_inserimento_sn(string& stringa_S_N, bool& flag_da_aggiornare) {
        do{
            cin >> stringa_S_N;
            Esito_input_SN esito = analizza_input_sn(stringa_S_N);
            if (esito == ANNULLA) return false;
            else if (esito == RIPROVA) continue; // Se l'utente ha scelto di riprovare, ricomincia il ciclo while (e quindi reinserisci il valore)
            else flag_da_aggiornare = (esito ? SI : NO);
        } while(1);
        return true;
    }
    
    // Funzione che carica il valore
    bool assegna_valore_cliente(vector<string>& campi, int& numero_riga, string& codice, string& nome, string& cognome, string& email, string& telefono, int& eta, Tipologia_cliente& tipologia) {
        codice = campi[0];
        vector<string> nome_cognome = split(campi[1], ' '); // Il campo "Nome Cognome" deve essere diviso in "Nome" e "Cognome"
        nome = nome_cognome[0];
        cognome = nome_cognome[1];
        email = campi[2];
        telefono = campi[3];
        eta = stoi(campi[4]);
        auto temp = stoe<Tipologia_cliente>(campi[5],++numero_riga);
        if (!temp.has_value()) return false; // Il template stoe restituisce una variabile senza valore in caso di errore
        else {tipologia = temp.value(); return true;} 
    }

    // Funzione di inserimento manuale dei dati da tastiera per pacchetti generico
    bool inserimento_dati_pacchetto_base(string& codice, string& dest, int& giorni, double& prezzo) {
        cout << "=== INSERIMENTO DATI NUOVO PACCHETTO ===" << endl;

        cout << "Codice Pacchetto: ";
        cin >> codice;

        cout << "Destinazione: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, dest);

        cout << "Durata in giorni: ";
        cin >> giorni;

        cout << "Prezzo base: ";
        cin >> prezzo;

        return true;

    }

    // Funzione di inserimento manuale dei dati da tastiera per pacchetti avventura
    bool inserisci_dati_pacchetto(vector<string>& lista_attivita, Categoria_adrenalina& categoria, bool& assicurazione) {
        
        cout << "Numero di attività incluse: ";
        int num_attivita; // Variabile temporanea
        cin >> num_attivita;
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
        else Categoria_adrenalina categoria = temp;

        cout << "Assicurazione inclusa? (S/N): ";
        string stringa_assicurazione_inclusa;
        if(!valida_inserimento_sn(stringa_assicurazione_inclusa,assicurazione)) return false; // Controllo se l'utente ha deciso di annullare l'operazione
        
        return true;

    }

    // Funzione di inserimento manuale dei dati da tastiera per pacchetti mare
    bool inserisci_dati_pacchetto(bool& ombrellone, bool& attrezzatura, Categoria_pensione& tipo) {

        cout << "Ombrellone incluso? (S/N): ";
        string stringa_ombrellone_incluso;
        if(!valida_inserimento_sn(stringa_ombrellone_incluso,ombrellone)) return false;

        cout << "Attrezzatura inclusa? (S/N): ";
        string stringa_attrezzatura_inclusa;
        if(!valida_inserimento_sn(stringa_attrezzatura_inclusa,attrezzatura)) return false;

        cout << "Categoria Pensione (Inserire ESATTAMENTE Solo colazione, Mezza pensione o Pensione completa): ";
        string string_pensione;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, string_pensione);
        auto temp = stoe<Categoria_pensione>(string_pensione);
        if (!temp.has_value()) return false; // Controllo errori
        else Categoria_pensione tipo = temp.value();
        
        return true;

    }

    // Funzione di inserimento manuale dei dati da tastiera per pacchetti montagna
    bool inserisci_dati_pacchetto(bool& skipass_incluso, int& num_escursioni, Categoria_difficolta& difficolta) {
        
        cout << "Skipass incluso? (S/N): ";
        string stringa_skipass_incluso;
        if(!valida_inserimento_sn(stringa_skipass_incluso,skipass_incluso)) return false;

        cout << "Numero di escursioni: ";
        cin >> num_escursioni;

        cout << "Difficoltà (inserire ESATTAMENTE Facile, Media o Difficile): ";
        string stringa_difficolta;
        cin >> stringa_difficolta;
        auto temp = stoe<Categoria_difficolta>(stringa_difficolta);
        if (!temp.has_value()) return false; // Controllo errore
        else Categoria_difficolta difficolta = temp.value();

        return true;
    }

    // Funzione di inserimento manuale dei dati da tastiera per pacchetti città
    bool inserisci_dati_pacchetto(int& num_musei, bool& guida, Categoria_hotel& hotel) {
        
        cout << "Numero di musei: ";
        cin >> num_musei;

        cout << "Guida inclusa? (S/N): ";
        string stringa_guida_inclusa;
        if (!valida_inserimento_sn(stringa_guida_inclusa,guida)) return false;

        cout << "Hotel (inserire esattamente Tre stelle, Quattro stelle o Cinque stelle): ";
        string stringa_hotel;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, stringa_hotel);
        auto temp = stoe<Categoria_hotel>(stringa_hotel);
        if (!temp.has_value()) return false;
        else Categoria_hotel hotel = temp.value();

        return true;

    }

    string tolower_string(string& stringa){
        for (char &c : stringa) {
            c = tolower((unsigned char)c);
        }
        return stringa;
    }
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
            cerr << "Errore nella creazione del codice unico: tipo non valido.";
    }
    return codice_univoco;
}

bool inizializza_mappa() {
        mappa_caricamento_specifico["Turismo Avventura"] = [this](string codice,string destinazione,int giorni,bool disponibile,double prezzo,const vector<string>& campi,int& numero_linea) -> bool {
            vector<string> lista_attivita;
            auto temp = stoe<Categoria_adrenalina>(campi[6],++numero_linea);
            if (!temp.has_value()) return false; // Controllo errori
            else Categoria_adrenalina categoria = temp.value();
            bool assicurazione = (campi[7] == "Con Assicurazione");
            for (size_t i = 8; i < campi.size(); ++i) {
                lista_attivita.push_back(campi[i]);
            }
            if(crea_elemento<Pacchetto_avventura>(codice,destinazione,giorni,disponibile,prezzo,lista_attivita,categoria,assicurazione)) {cout << "Pacchetto Avventura " << codice << " caricato correttamente." << endl; return true}
            else return false;
        };

        mappa_caricamento_specifico["Turismo Balneare"] = [](string codice,string destinazione,int giorni,bool disponibile,double prezzo,const vector<string>& campi,int& numero_linea) -> bool {
            bool ombrellone = (campi[6] == "Con Ombrellone");
            bool attrezzatura = (campi[7] == "Con Attrezzatura");
            auto temp = stoe<Categoria_pensione>(campi[8],++numero_linea);
            if (!temp.has_value()) return false; // Controllo errori
            else Categoria_pensione tipo = temp.value();
            if(crea_elemento(codice,destinazione,giorni,disponibile,prezzo,ombrellone,attrezzatura,tipo)){ cout << "Pacchetto Mare " << codice << " caricato correttamente." << endl; return true}
            return true;
        };

        mappa_caricamento_specifico["Turismo Montano"] = [](string codice,string destinazione,int giorni,bool disponibile,double prezzo,const vector<string>& campi, int& numero_linea) -> bool {
            bool skipass = (campi[6] == "Con Skipass");
            int numero_escursioni = stoi(campi[7]);
            auto temp = stoe<Categoria_difficolta>(campi[8],++numero_linea);
            if (!temp.has_value()) return false; // Controllo errori
            else Categoria_difficolta difficolta = temp.value();
            if(crea_elemento(codice,destinazione,giorni,disponibile,prezzo,skipass,numero_escursioni,difficolta)) {cout << "Pacchetto Montagna " << codice << " caricato correttamente." << endl; return true;}
            return false;
        }

        mappa_caricamento_specifico["Città d'Arte"] = [](string codice,string destinazione,int giorni,bool disponibile,double prezzo,const vector<string>& campi, int& numero_linea) -> bool {
            bool guida_inclusa = (campi[6] == "Con Guida");
            int numero_musei = stoi(campi[7]);
            auto temp = stoe<Categoria_hotel>(campi[8],++numero_linea);
            if (!temp.has_value()) return false; // Controllo errori
            else Categoria_hotel categoria_hotel = temp.value;
            if(crea_elemento(codice,destinazione,giorni,disponibile,prezzo,guida_inclusa,numero_musei,categoria_hotel)) {cout << "Pacchetto Città " << codice << " caricato correttamente." << endl; return true;}
            return false;
        }
}

// Metodo che carica i clienti da un file
bool Gestore_azienda::carica_clienti(std::ifstream& file) {
    string linea;
    int numero_riga = 0;
    while (getline(file, linea)) {

        // Se la linea è vuota passo alla successiva
        if(linea.empty()) {
            cout << "Riga #" << ++numero_riga << ": vuota";
            continue;
        }

        // Dividiamo la stringa nel file in "campi"
        vector<string> campi;
        for (const auto& campo : split(linea, ';')) {
            campi.push_back(campo);
        }

        // Assegnazione dei valori dal file (la riga viene aggiornata in assegna_valore_cliente())
        string codice; string nome; string cognome; string email; string telefono; int eta; Tipologia_cliente tipologia;
        if(assegna_valore_cliente(campi,numero_riga,codice,nome,cognome,email,telefono,eta,tipologia)) continue; // Se ritorna false, passo alla riga successiva, ma segnalo l'errore e la riga (fatto dalla funzione stoe in assegna_valore_cliente)
        
        // Se la creazione dell'elemento avviene correttamente, la funzione restituisce false
        if(crea_elemento<Cliente>(codice, nome, cognome, email, telefono, eta, tipologia)) cout << "Riga #" << numero_riga << ": Cliente " << nome << " " << cognome << " caricato correttamente." << endl;
    }
    return true;
}

// Metodo che carica i pacchetti da un file
bool Gestore_azienda::carica_pacchetti(std::ifstream& file) {

        string linea;
        int numero_riga = 0;
        while (getline(file, linea)) {
            vector<string> campi = split(linea,';');
            string codice = campi[0];
            string destinazione = campi[1];
            int giorni = stoi(campi[2]);
            bool disponibile = (campi[3] == "Disponibile");
            double prezzo = stod(campi[4]);
            string tipologia = campi[5];

            auto temp = mappa_caricamento_specifico.find(tipologia)
            if (temp != mappa_caricamento_specifico.end()) return temp->second(codice,destinazione,giorni,disponibile,prezzo,tipologia,campi,numero_riga);
            else cerr << "Tipologia di pacchetto non riconosciuta durante il caricamento: " << tipologia << endl;
        }
        return true;
}

// Metodo che carica le prenotazioni da un file
bool Gestore_azienda::carica_prenotazioni(std::ifstream& file) {
    string linea;
    while (getline(file, linea)) {
        vector<string> campi = split(linea, ';');
        string codice_prenotazione = campi[0];
        string codice_cliente = campi[1];
        string codice_pacchetto = campi[2];
        int numero_persone = stoi(campi[3]);
        string data_prenotazione = campi[4];
        bool confermata = (campi[5] == "Confermata");
        double prezzo_totale = stod(campi[6]);

        shared_ptr<Cliente> cliente = cercaCliente(codice_cliente);
        shared_ptr<Pacchetto_viaggio> pacchetto = cercaPacchetto(codice_pacchetto);
        if(crea_elemento(codice_prenotazione, cliente, pacchetto, numero_persone, data_prenotazione)) cout << "Prenotazione " << codice_prenotazione << " per il cliente " << codice_cliente << " e il pacchetto " << codice_pacchetto << " caricata correttamente" << endl;
        
        if (confermata) prenotazioni[prenotazioni.size() - 1]->confermaPrenotazione();

    }
    return true;
}

// === COSTRUTTORE E DISTRUTTORE ===
Gestore_azienda::Gestore_azienda(std::vector<std::shared_ptr<Pacchetto_viaggio>> cat, std::vector<std::shared_ptr<Cliente>> cli,
                     std::vector<std::shared_ptr<Prenotazione>> pre) :
    catalogo(cat), clienti(cli), prenotazioni(pre) {
        cout << "Gestione azienda inizializzata correttamente!" << endl;
}
Gestore_azienda::~Gestore_azienda() {
    
    for (const auto& prenotazione : prenotazioni) {
        prenotazione->~Prenotazione();
    }

    for (const auto& cliente : clienti) {
        cliente->~Cliente();
    }
    
    for (const auto& pacchetto : catalogo) {
        pacchetto->~Pacchetto_viaggio();
    }

    cout << "Gestore azienda distrutto correttamente." << endl;
}

// === GESTIONE CATALOGO PACCHETTI ===

// METODI DI AGGIUNTA INTERATTIVI DA INTRODURRE DOPO LA MODIFICA DEL COSTRUTTORE (privato con try/catch)

// Metodo per l'aggiunta di un pacchetto avventura
bool Gestore_azienda::aggiungiPacchettoAvventura() {

    do {
        string codice; string dest; int giorni; double prezzo; vector<string> lista_attivita; Categoria_adrenalina categoria; bool assicurazione;
        
        // Inserimento interattivo dei dati del pacchetto di base
        inserisci_dati_pacchetto_base(codice,dest,giorni,prezzo);

        // Inserimento interattivo dei dati del pacchetto specifico
        if (!inserisci_dati_pacchetto(lista_attivita, categoria, assicurazione)) { // Errore -> reinserimento?
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) return false;
            else continue;
        }

        // Creazione dell'elemento
        if (crea_elemento<Pacchetto_avventura>(codice, dest, giorni, prezzo, lista_attivita, categoria, assicurazione)) {
            cout << "Pacchetto avventura " << codice << " aggiunto correttamente al sistema." << endl;
            return true;
        } else { // Errore -> reinserimento?
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) return false;
        }
    } while (1);
}

// Metodo per l'aggiunta di un pacchetto mare
bool Gestore_azienda::aggiungiPacchettoMare() {

    string codice; string dest; int giorni; double prezzo; bool ombrellone; bool attrezzatura; Categoria_pensione tipo;

    do {
        // Inserimento interattivo dei dati del pacchetto di base
        inserisci_dati_pacchetto_base(codice,dest,giorni,prezzo);

        // Inserimento interattivo dei dati del pacchetto specifici
        if (!inserisci_dati_pacchetto(ombrellone, attrezzatura, tipo)) { // Errore -> reinserimento?
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) return false; 
            else continue;
        }

        // Creazione dell'elemento
        if (crea_elemento<Pacchetto_mare>(codice, dest, giorni, prezzo, ombrellone, attrezzatura, tipo)) {
            cout << "Pacchetto Mare " << codice << " inserito correttamente." << endl;
            return true; 
        } else { // Errore -> reinserimento?
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) return false;
        }
    } while(1);
}

// Metodo per l'aggiunta di un pacchetto montagna
bool Gestore_azienda::aggiungiPacchettoMontagna() {

    string codice; string dest; int giorni; double prezzo; bool skipass_incluso; int num_escursioni; Categoria_difficolta difficolta;

    do {
        // Inserimento interattivo dei dati del pacchetto di base
        inserisci_dati_pacchetto_base(codice,dest,giorni,prezzo);

        // Inserimento interattivo dei dati del pacchetto di base
        if (!inserisci_dati_pacchetto(skipass_incluso, num_escursioni, difficolta)) { // Errore -> reinserimento?
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) return false; 
            else continue;
        }

        // Creazione dell'elemento
        if (crea_elemento<Pacchetto_montagna>(codice, dest, giorni, prezzo, skipass_incluso, num_escursioni, difficolta)) {
            cout << "Pacchetto Montagna " << codice << " inserito correttamente." << endl;
            return true; 
        } else { // Errore -> reinserimento?
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) return false;
        }
    } while(1);
    
    return true;
}

// Metodo per l'aggiunta di un pacchetto città
bool Gestore_azienda::aggiungiPacchettoCitta() {

    string codice; string dest; int giorni; double prezzo; int num_musei; bool guida; Categoria_hotel hotel;

    do {
        inserisci_dati_pacchetto_base(codice,dest,giorni,prezzo);

        if (!inserisci_dati_pacchetto(num_musei, guida, hotel)) {
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) {
                return false;
            } else continue;
        }

        if (crea_elemento<Pacchetto_citta>(codice, dest, giorni, prezzo, num_musei, guida, hotel)) {
            cout << "Pacchetto Città " << codice << " inserito correttamente." << endl;
            return true; 
        } else {
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) {
                return false;
            }
        }
    } while(1);
}

shared_ptr<Pacchetto_viaggio> Gestore_azienda::cercaPacchetto(string codice) {
    return cerca_elemento(this->catalogo, codice);
}

bool Gestore_azienda::visualizzaCatalogo() const {
    return stampa_elementi(this->catalogo);
}

bool Gestore_azienda::visualizzaPacchettiPerTipologia(string tipo) const {
    bool flag = false;
    for (const auto& elemento : this->catalogo) {
        if (elemento->get_tipologia() == tipo) {
            flag = elemento->stampa_dettagli();
        }
    }
    if (!flag) {cerr << "Errore! La tipologia inserita è errata!" << endl; return false;}
    return true;
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
bool Gestore_azienda::aggiungiCliente() {

    // Lambda function per l'inserimento interattivo dei dati del cliente
    auto inserimento_dati_cliente = [](string& nome, string& cognome, string& email, string& telefono,
        int& eta; string& tipo_str; Tipologia_cliente& tipo) {
        cout << "=== INSERIMENTO DATI NUOVO CLIENTE ===" << endl;
        
        cout << "Nome: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, nome);

        cout << "Cognome: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, cognome);

        cout << "Email (deve contenere @!): ";
        cin >> email;

        cout << "Telefono (10 cifre!): ";
        cin >> telefono;

        cout << "Età (deve essere maggiore di 0): ";
        cin >> eta;

        cout << "Tipologia Cliente (scrivere ESATTAMENTE Standard, Premium o VIP): ";
        cin >> tipo_str;
        auto temp = stoe<Tipologia_cliente>(tipo_str);
        if (!temp.has_value()) return false;
        else Tipologia_cliente tipo = temp.value();
        return true;
    };

    // Corpo del metodo
    do {
        string nome; string cognome; string email; string telefono; int eta; string tipo_str; Tipologia_cliente tipo;
        inserimento_dati_cliente(nome, cognome, email, telefono, eta, tipo_str, tipo);

        if (crea_elemento<Cliente>(generaCodiceUnico('C'), nome, cognome, email, telefono, eta, tipo)) {
        cout << "Cliente " << nome << " " << cognome << " aggiunto correttamente al sistema." << endl;
        return true;
        } else {
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) {
                return false;
            }
        }
    } while (1);
}
shared_ptr<Cliente> Gestore_azienda::cercaCliente(string codice) {
    return cerca_elemento(this->clienti, codice);
}

bool Gestore_azienda::visualizzaClienti() const {
    return stampa_elementi(this->clienti);
}

bool Gestore_azienda::visualizzaClientiPerTipologia(string tipo) const {
    bool flag = false;
    for (const auto& elemento : this->clienti) {
        if (etos(elemento->get_tipologia()) == tipo) {
            flag = elemento->stampa_dettagli();
        }
    }
    if (!flag) {cerr << "Errore! La tipologia inserita è errata!" << endl; return false;}
    return true;
}


// === GESTIONE PRENOTAZIONI ===
bool Gestore_azienda::aggiungiPrenotazione() {

    // Lambda function per l'inserimento interattivo dei dati della prenotazione
    auto inserisci_dati_prenotazione = [](shared_ptr<Cliente>& cliente, shared_ptr<Pacchetto_viaggio>& pacchetto_viaggio,
        int& num_persone, string& data) {
        string codice_cliente; string codice_pacchetto;
        cout << "=== INSERIMENTO DATI NUOVA PRENOTAZIONE ===" << endl;

        cout << "Codice Cliente: ";
        cin >> codice_cliente;
        cliente = cercaCliente(codice_cliente);
        if (cliente == nullptr) {
            return false;
        }

        cout << "Codice Pacchetto Viaggio: ";
        cin >> codice_pacchetto;
        pacchetto_viaggio = cercaPacchetto(codice_pacchetto);
        if (pacchetto_viaggio == nullptr) {
            return false;
        }

        cout << "Numero di Persone: ";
        cin >> num_persone;

        cout << "Data Prenotazione (DD/MM/YYYY): ";
        cin >> data;

        return true;
    };

    // Corpo del metodo
    do {
        shared_ptr<Cliente> cliente; shared_ptr<Pacchetto_viaggio> pacchetto_viaggio; int num_persone; string data;
        if (!inserisci_dati_prenotazione(cliente, pacchetto_viaggio, num_persone, data)) {
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) {
                return false;
            }
            continue;
        }

        if (crea_elemento<Prenotazione>(generaCodiceUnico('B'), cliente, pacchetto_viaggio, num_persone, data)) {
            cout << "Prenotazione per il cliente " << cliente->get_nome_completo() << " aggiunta correttamente al sistema." << endl;
            return true;
        } else {
            int scelta = menu::menu_reinserimento();
            if (scelta == 2) {
                return false;
            }
        }
    } while (1);
}

// Metodo che conferma la prenotazione che possiede il codice inserito in input
// Questo metodo ritorna true se la modifica è stata apportata, altrimenti ritorna false (se il codice era già stato confermato ritorna false)
bool Gestore_azienda::confermaPrenotazione(string codice) {
    shared_ptr<Prenotazione> prenotazione = cerca_elemento(this->prenotazioni, codice);
    if (prenotazione != nullptr) {
        if (!prenotazione->conferma_prenotazione()) {
            cerr << "Errore! Codice già confermato in precedenza!" << codice << "." << endl;
            return false;
        } else return true;
    } else {
        cerr << "Elemento con codice " << codice << " non trovato." << endl;
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
            prenotazione->stampa_dettagli();
            return true;
        }
    }
    cout << "Cliente con codice " << codiceCliente << "non trovato";
    return false;
}
bool Gestore_azienda::visualizzaPrenotazioniConfermate() const {
    int i = 0;
    bool flag = false;
    for (const auto& prenotazione : prenotazioni) {
        if (prenotazione->is_confermata()) {
            cout << "Prenotazione confermata #" << ++i << ":" << endl;
            flag = prenotazione->stampa_dettagli();
        }
    }
    if (!flag) cerr << "Non ci sono prenotazioni confermate!" << endl;
    return flag;
}

// === STATISTICHE ===

// Metodo che stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Pacchetto più prenotato, Destinazione più richiesta
bool Gestore_azienda::statisticheGenerali() const {

    // Controllo errore nei pacchetti
    auto flag_pacchetti = calcola_massimo_mappa(Prenotazione::pacchetti_counter);    
    if (!flag_pacchetti.has_value()) {cout << "Non ci sono pacchetti nel database" << endl; return false;}

    auto flag_destinazioni = calcola_massimo_mappa(Prenotazione::destinazioni_counter);
    if (!flag_destinazioni.has_value()) {cout << "Non ci sono pacchetti nel database" << endl; return false;}

    cout << "=== Statistiche Generali Agenzia Viaggi ===" << endl;
    cout << "Totale Pacchetti nel Catalogo: " << Pacchetto_viaggio::pacchetti_creati << endl;
    cout << "Totale Clienti Registrati: " << Cliente::clienti_creati << endl;
    cout << "Totale Prenotazioni Effettuate: " << Prenotazione::prenotazioni_create << endl;
    cout << "Fatturato Totale: " << Prenotazione::fatturato_totale << " EUR" << endl;
    cout << "Pacchetto più prenotato: " << flag_destinazioni.value() << endl;
    cout << "Destinazione più richiesta: " << flag_pacchetti.value() << endl;
    return true;
}
    
bool Gestore_azienda::statistichePerTipologia() const {
    cout << "=== Numero di prenotazioni per ogni Tipologia di Pacchetto ===" << endl;
    for (const auto& campo : Prenotazione::pacchetti_counter) {
        cout << "Tipologia: " << campo.first << " - Prenotazioni: " << campo.second << endl;
    }
    return true;
}

shared_ptr<Cliente> Gestore_azienda::clienteMigliore() const {
    auto flag = calcola_massimo_mappa(Prenotazione::spesa_clienti);
    if (!flag.has_value()) {cout << "Non ci sono clienti nel database"; return nullptr;}
    cout << "Cliente che ha speso di più: " << flag.value() << endl;
    return cercaCliente(flag);
}
// Ritorna il cliente che ha speso di più

// === GESTIONE FILE ===
// Salva/Carica lo stato intero dell'agenzia (CSV simulato)
bool Gestore_azienda::salvaDatiSuFile(const string& nomefile, string& tipo) {
    ofstream file(nomefile);
    if (!file) {cerr << "Errore nell'apertura del file per la scrittura: " << nomefile << endl; return false;}

    // Salvataggio clienti
    if (tolower_string(tipo) == "cliente") {
        for (const auto& cliente : this->clienti) {
            cliente->salva_dati_su_file(file);    
        }
        
    // Salvataggio pacchetti
    } else if (tolower_string(tipo) == "pacchetto") {
        for (const auto& pacchetto : this->catalogo) {
            pacchetto->salva_dati_su_file();
        }
    
    // Salvataggio prenotazioni
    } else if (tolower_string(tipo) == "prenotazione") {
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

bool Gestore_azienda::caricaDatiDaFile(const string& nomefile, const string& tipo) {
    ifstream file(nomefile);
    if (!file) { cerr << "Errore nell'apertura del file per la lettura: " << nomefile << endl; return false; }

    // Caricamento clienti
    if (tolower_string(tipo) == "clienti") {
        carica_clienti(file);    
    
    // Caricamento pacchetti
    } else if (tolower_string(tipo) == "pacchetti") {
        carica_pacchetti(file);
        
    // Caricamento prenotazioni
    } else if (tolower_string(tipo) == "prenotazioni") {
        carica_prenotazioni(file);

    } else {
        cerr << "Tipo di dato non riconosciuto per il caricamento." << endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}