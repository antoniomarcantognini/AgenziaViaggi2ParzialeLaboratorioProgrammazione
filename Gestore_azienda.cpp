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
    Esito_input_SN analizza_input_sn(const stringa& input) {
        char c = tolower(input[0]);
        if (c == 's') return SI;
        if (c == 'n') return NO;

        cerr << "Valore inserito non valido: puoi inserire solo 'S' o 'N'." << endl;

        if(menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento S/N", "Annulla operazione e torna al menu principale"}) == 1) return RIPROVA;
        else return ANNULLA;
    }
    
    // Funzione che richiede carica il valore
    bool assegna_valore_cliente(vector<string>& campi, int& numero_riga, string& codice,vector<string>& nome_cognome, string& email, string& telefono, int& eta, Tipologia_cliente& tipologia)
        string codice = campi[0];
        vector<string> nome_cognome = split(campi[1], ' '); // Il campo "Nome Cognome" deve essere diviso in "Nome" e "Cognome"
        string nome = nome_cognome[0];
        string cognome = nome_cognome[1];
        string email = campi[2];
        string telefono = campi[3];
        int eta = stoi(campi[4]);
        auto temp = stoe<Tipologia_cliente>(campi[5],++numero_riga);
        if (!tipologia.has_value()) return false; // Il template stoe restituisce una variabile senza valore in caso di errore
        else {tipologia = temp; return true;} 


    // Mappa statica tra una stringa e una funzione che a seconda della tipologia di oggetto, restituisce la giusta funzione di caricamento
    static const map<string, function<shared_ptr<Pacchetto_viaggio>(string& codice, string& destinazione, int& giorni, bool& disponibile, double& prezzo, const vector<string>& campi, int& numero_linea)>> mappa_caricamento_specifico = {

        {
            "Turismo Avventura", 
            [this](string codice,string destinazione,int giorni,bool disponibile,double prezzo,const vector<string>& campi,int& numero_linea) {
                vector<string> lista_attivita;
                auto categoria = stoe<Categoria_adrenalina>(campi[6],++numero_linea);
                if (!categoria.has_value()) return false;
                bool assicurazione = (campi[7] == "Con Assicurazione");
                for (size_t i = 8; i < campi.size(); ++i) {
                    lista_attivita.push_back(campi[i]);
                }
                if(crea_elemento<Pacchetto_avventura>(codice,destinazione,giorni,disponibile,prezzo,lista_attivita,categoria,assicurazione)) cout << "Pacchetto Avventura " << codice << " caricato correttamente." << endl;
            }
        }

        {
            "Turismo Balneare", 
            [this](string codice,string destinazione,int giorni,bool disponibile,double prezzo;const vector<string>& campi,int& numero_linea) {
                bool ombrellone = (campi[6] == "Con Ombrellone");
                bool attrezzatura = (campi[7] == "Con Attrezzatura");
                auto tipo = stoe<Categoria_pensione>(campi[8],++numero_linea);
                if (!tipo.has_value()) return false;
                if(crea_elemento(codice,destinazione,giorni,disponibile,prezzo,ombrellone,attrezzatura,tipo)) cout << "Pacchetto Mare " << codice << " caricato correttamente." << endl;
            }
        }

        {
            "Turismo Montano",
            [this](string codice,string destinazione,int giorni,bool disponibile,double prezzo;const vector<string>& campi, int& numero_linea) {
                bool skipass = (campi[6] == "Con Skipass");
                int numero_escursioni = stoi(campi[7]);
                auto difficolta = stoe<Categoria_difficolta>(campi[8],++numero_linea);
                if (!difficolta.has_value()) return false;
                if(crea_elemento(codice,destinazione,giorni,disponibile,prezzo,skipass,numero_escursioni,difficolta)) cout << "Pacchetto Montagna " << codice << " caricato correttamente." << endl;
            }
        }

        {
            "Città d'Arte",
            [this](string codice,string destinazione,int giorni,bool disponibile,double prezzo;const vector<string>& campi, int& numero_linea) {
                bool guida_inclusa = (campi[6] == "Con Guida");
                int numero_musei = stoi(campi[7]);
                auto categoria_hotel = stoe<Categoria_hotel>(campi[8],++numero_linea);
                if (!categoria_hotel.has_value()) return false;
                if(crea_elemento(codice,destinazione,giorni,disponibile,prezzo,guida_inclusa,numero_musei,categoria_hotel)) cout << "Pacchetto Città " << codice << " caricato correttamente." << endl;
            }
        }
    }

    // Funzione di inserimento manuale dei dati da tastiera per pacchetti avventura
    bool inserisci_dati_pacchetto(string& codice, string& dest, int& giorni, double& prezzo,
                                    vector<string>& lista_attivita, Categoria_adrenalina& categoria, bool& assicurazione) {
        cout << "=== INSERIMENTO DATI NUOVO PACCHETTO AVVENTURA ===" << endl;

        cout << "Codice Pacchetto: ";
        cin >> codice;

        cout << "Destinazione: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, dest);

        cout << "Durata in giorni: ";
        cin >> giorni;

        cout << "Prezzo base: ";
        cin >> prezzo;

        cout << "Numero di attività incluse: ";
        int num_attivita;
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
        if (!temp.has_value()) return false;
        else categoria = temp;

        cout << "Assicurazione inclusa? (S/N): ";
        string stringa_assicurazione_inclusa;
        if (!stringa_S_N(stringa_assicurazione_inclusa,assicurazione)) return false;

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
            cerr << "Errore nella creazione del codice unico: tipo non valido."
    }
    return codice_univoco;
}

// Funzione che valida l'inserimento di 'S' e 'N'. Ritorna false se si deve annullare l'operazione, true se è andata a buon fine
bool Gestore_azienda::valida_inserimento_S_N(string& stringa_S_N, bool& flag_da_aggiornare) {
    do{
        cin >> stringa_S_N;
        Esito_input_SN esito = analizza_input_sn(stringa_S_N);
        if (esito == ANNULLA) return false;
        else if (esito == RIPROVA) continue; // Se l'utente ha scelto di riprovare, ricomincia il ciclo while (e quindi reinserisci il valore)
        else flag_da_aggiornare = (esito ? SI : NO);
    } while(1);
    return true;
}

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
        if(crea_elemento<Cliente>(codice, nome, cognome, email, telefono, eta, tipologia)) cout << "Riga #" << numero_riga << ": Cliente " << nome_cognome << " caricato correttamente." << endl;
    }
    return true;
}

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
            if (temp != mappa_caricamento_specifico.end()) return second.mappa_caricamento_specifico(codice,destinazione,giorni,disponibile,prezzo,tipologia,campi,numero_riga);
            else cerr << "Tipologia di pacchetto non riconosciuta durante il caricamento: " << tipologia << endl;
        }
        return true;
}

bool Gestore_azienda::carica_prenotazioni(std::ifstream& file) {
    string linea;
    vector<string> campi;
    while (getline(file, linea)) {
        for (const auto& campo : split(linea, ';')) {
            campi.push_back(campo);
        }
        string codice_prenotazione = campi[0];
        string codice_cliente = campi[1];
        string codice_pacchetto = campi[2];
        int numero_persone = stoi(campi[3]);
        string data_prenotazione = campi[4];
        bool confermata = (campi[5] == "Confermata");
        double prezzo_totale = stod(campi[6]);

        if (confermata) {
            prenotazioni[prenotazioni.size() - 1]->confermaPrenotazione();
        }

        shared_ptr<Cliente> cliente = cercaCliente(codice_cliente);
        shared_ptr<Pacchetto_viaggio> pacchetto = cercaPacchetto(codice_pacchetto);
        if(crea_elemento(codice_prenotazione, cliente, pacchetto, numero_persone, data_prenotazione)) cout << "Prenotazione " << codice_prenotazione << " per il cliente " << codice_cliente << " e il pacchetto " << codice_pacchetto << " caricata correttamente" << endl;
        
    }
    return true;
}

// === COSTRUTTORE E DISTRUTTORE ===
Gestore_azienda::Gestore_azienda() {
    cout << "Gestore azienda creato." << endl;
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
        if (!inserisci_dati_pacchetto(codice, dest, giorni, prezzo, lista_attivita, categoria, assicurazione)) {
            int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
            if (scelta == 2) {
                return false;
            } else continue;
        }
        if (crea_elemento<Pacchetto_avventura>(codice, dest, giorni, prezzo, lista_attivita, categoria, assicurazione)) {
            cout << "Pacchetto avventura " << codice << " aggiunto correttamente al sistema." << endl;
            return true;
        } else {
            int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
            if (scelta == 2) {
                return false;
            }
        }
    } while (1);
}

// Metodo per l'aggiunta di un pacchetto mare
bool Gestore_azienda::aggiungiPacchettoMare() {

    auto inserisci_dati_pacchetto = [](string& codice, string& dest, int& giorni, double& prezzo,
        bool& ombrellone, bool& attrezzatura, Categoria_pensione& tipo) {
        cout << "=== INSERIMENTO DATI NUOVO PACCHETTO MARE ===" << endl;

        cout << "Codice Pacchetto: ";
        cin >> codice;

        cout << "Destinazione: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, dest);

        cout << "Durata in giorni: ";
        cin >> giorni;

        cout << "Prezzo base: ";
        cin >> prezzo;

        cout << "Ombrellone incluso? (S/N): ";
        string stringa_ombrellone_incluso;
        if(!valida_inserimento_S_N(stringa_ombrellone_incluso,ombrellone)) return false;

        cout << "Attrezzatura inclusa? (S/N): ";
        string stringa_attrezzatura_inclusa;
        if(!valida_inserimento_S_N(stringa_attrezzatura_inclusa,attrezzatura)) return false;

        cout << "Categoria Pensione (Inserire ESATTAMENTE Solo colazione, Mezza pensione o Pensione completa): "
        string string_pensione;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, string_pensione);
        auto temp = stoe<Categoria_pensione>(pensione);
        if (!temp.has_value()) return false;
        else tipo = temp;

    };

    string codice; string dest; int giorni; double prezzo; bool ombrellone; bool attrezzatura; Categoria_pensione tipo;

    do {
    if (!inserisci_dati_pacchetto(codice, dest, giorni, prezzo, ombrellone, attrezzatura, tipo)) {
        int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
        if (scelta == 2) {
            return false;
        } else continue;
    }

    if (crea_elemento<Pacchetto_mare>(codice, dest, giorni, prezzo, ombrellone, attrezzatura, tipo)) {
        cout << "Pacchetto Mare " << codice << " inserito correttamente." << endl;
        return true; 
    } else {
        int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
        if (scelta == 2) {
            return false;
        }
    }
    } while(1);
}

// Metodo per l'aggiunta di un pacchetto montagna
bool Gestore_azienda::aggiungiPacchettoMontagna() {

    auto inserisci_dati_pacchetto = [](string& codice, string& dest, int& giorni, double& prezzo,
        bool& skipass_incluso, int& num_escursioni, Categoria_difficolta& difficolta) {
        
        cout << "=== INSERIMENTO DATI NUOVO PACCHETTO MARE ===" << endl;

        cout << "Codice Pacchetto: ";
        cin >> codice;

        cout << "Destinazione: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, dest);

        cout << "Durata in giorni: ";
        cin >> giorni;

        cout << "Prezzo base: ";
        cin >> prezzo;
        
        cout << "Skipass incluso? (S/N): ";
        string stringa_skipass_incluso;
        if(!valida_inserimento_S_N(stringa_skipass_incluso,skipass_incluso)) return false;

        cout << "Numero di escursioni: ";
        cin >> num_escursioni;

        cout << "Difficoltà (inserire ESATTAMENTE Facile, Media o Difficile): ";
        string stringa_difficolta;
        cin >> stringa_difficolta;
        auto temp = stoe<Categoria_difficolta>(stringa_difficolta);
        if (!temp.has_value()) return false;
        else difficolta = temp;
    };

    string codice; string dest; int giorni; double prezzo; bool skipass_incluso; int num_escursioni; Categoria_difficolta difficolta;

    do {
    if (!inserisci_dati_pacchetto(codice, dest, giorni, prezzo, skipass_incluso, num_escursioni, difficolta)) {
        int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
        if (scelta == 2) {
            return false;
        } else continue;
    }

    if (crea_elemento<Pacchetto_montagna>(codice, dest, giorni, prezzo, skipass_incluso, num_escursioni, difficolta)) {
        cout << "Pacchetto Montagna " << codice << " inserito correttamente." << endl;
        return true; 
    } else {
        int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
        if (scelta == 2) {
            return false;
        }
    }
    } while(1);
    
    return true;
}

// Metodo per l'aggiunta di un pacchetto città
bool Gestore_azienda::aggiungiPacchettoCitta() {

    auto inserisci_dati_pacchetto = [](string& codice, string& dest, int& giorni, double& prezzo,
        int& num_musei, bool& guida, Categoria_hotel& hotel) {
        
        cout << "=== INSERIMENTO DATI NUOVO PACCHETTO MARE ===" << endl;

        cout << "Codice Pacchetto: ";
        cin >> codice;

        cout << "Destinazione: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, dest);

        cout << "Durata in giorni: ";
        cin >> giorni;

        cout << "Prezzo base: ";
        cin >> prezzo;

        cout << "Numero di musei: ";
        cin >> num_musei;

        cout << "Guida inclusa? (S/N): ";
        string stringa_guida_inclusa;
        if (!valida_inserimento_S_N(stringa_guida_inclusa,guida)) return false;

        cout << "Hotel (inserire esattamente Tre stelle, Quattro stelle o Cinque stelle): ";
        string stringa_hotel;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, stringa_hotel)
        auto temp = stoe<Categoria_hotel>(stringa_hotel);
        if (!temp.has_value()) return false;
        else hotel = temp;

    }

    string codice; string dest; int giorni; double prezzo; int num_musei; bool guida; Categoria_hotel hotel;

    do {
        if (!inserisci_dati_pacchetto(codice, dest, giorni, prezzo, num_musei, guida, hotel)) {
            int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
            if (scelta == 2) {
                return false;
            } else continue;
        }

        if (crea_elemento<Pacchetto_citta>(codice, dest, giorni, prezzo, num_musei, guida, hotel)) {
            cout << "Pacchetto Città " << codice << " inserito correttamente." << endl;
            return true; 
        } else {
            int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PACCHETTO", "Riprova inserimento dati pacchetto", "Annulla operazione e torna al menu principale"});
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
        int& eta, string& tipo_str, Tipologia_cliente& tipo) {
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
        else tipo = temp;
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
            int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO CLIENTE", "Riprova inserimento dati cliente", "Annulla operazione e torna al menu principale"});
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
    for (const auto& elemento : lista) {
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
            int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PRENOTAZIONE", "Riprova inserimento dati prenotazione", "Annulla operazione e torna al menu principale"});
            if (scelta == 2) {
                return false;
            }
            continue;
        }

        if (crea_elemento<Prenotazione>(generaCodiceUnico('B'), cliente, pacchetto_viaggio, num_persone, data)) {
            cout << "Prenotazione per il cliente " << cliente->get_nome_completo() << " aggiunta correttamente al sistema." << endl;
            return true;
        } else {
            int scelta = menu::stampa_menu_e_scelta({"SCELTA REINSERIMENTO PRENOTAZIONE", "Riprova inserimento dati prenotazione", "Annulla operazione e torna al menu principale"});
            if (scelta == 2) {
                return false;
            }
        }
    } while (1);
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
    cerr << "Cliente con codice " << codice << "non trovato";
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
bool Gestore_azienda::salvaDatiSuFile(const string& nomefile, const string& tipo) const {
    ofstream file(nomefile);
    if (!file) {cerr << "Errore nell'apertura del file per la scrittura: " << nomefile << endl; return false;}

    // Salvataggio clienti
    if (tolower(tipo) == "cliente") {
        for (const auto& cliente : this->clienti) {
            cliente->salva_dati_su_file(file);    
        }
        
    // Salvataggio pacchetti
    } else if (tolower(tipo) == "pacchetto") {
        for (const auto& pacchetto : this->catalogo) {
            pacchetto->salva_dati_su_file();
        }
    
    // Salvataggio prenotazioni
    } else if (tolower(tipo) == "prenotazione") {
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
    if (tolower(tipo) == "clienti") {
        carica_clienti(file);    
    
    // Caricamento pacchetti
    } else if (tolower(tipo) == "pacchetti") {
        carica_pacchetti(file);
        
    // Caricamento prenotazioni
    } else if (tolower(tipo) == "prenotazioni") {
        carica_prenotazioni(file);

    } else {
        cerr << "Tipo di dato non riconosciuto per il caricamento." << endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

// COSTRUTTORE
Gestore_azienda::Gestore_azienda(std::vector<std::shared_ptr<Pacchetto_viaggio>> cat, std::vector<std::shared_ptr<Cliente>> cli,
                     std::vector<std::shared_ptr<Prenotazione>> pre) :
    catalogo(cat), clienti(cli), prenotazioni(pre) {
        cout << "Gestione azienda inizializzata correttamente!" << endl;
}

// DISTRUTTORE
Gestore_azienda::~Gestore_azienda() {
    for (const auto& cliente : this->clienti) {
        cliente->~Cliente();
    }

    for (const auto& pacchetto : this->catalogo) {
        pacchetto->~Pacchetto_viaggio();
    }

    for (const auto& prenotazione : this->prenotazioni) {
        prenotazione->~Prenotazione();
    }
}