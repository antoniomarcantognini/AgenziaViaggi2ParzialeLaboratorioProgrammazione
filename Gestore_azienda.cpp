#include "magic_enum.hpp"
#include "Gestore_azienda.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include "menu.h"

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

bool Gestore_azienda::salvataggio_clienti(std::ofstream& file) const {
    for (const auto& cliente : this->clienti) {
        file << cliente->get_codice() << ";"
             << cliente->get_nome_completo() << ";"
             << cliente->get_email() << ";"
             << cliente->get_telefono() << ";"
             << cliente->get_eta() << ";"
             << enum_name(cliente->get_tipologia()) << endl;
        cout << "Cliente " << cliente->get_nome_completo() << " salvato correttamente." << endl;
    }
    return true;
}

bool Gestore_azienda::carica_clienti(std::ifstream& file) {
    string linea;
    vector<string> campi;
    while (getline(file, linea)) {
        for (const auto& campo : split(linea, ';')) {
            campi.push_back(campo);
        }
        string codice = campi[0];
        vector<string> nome_cognome = split(campi[1], ' ');
        string nome = nome_cognome[0];
        string cognome = nome_cognome[1];
        string email = campi[2];
        string telefono = campi[3];
        int eta = stoi(campi[4]);
        Tipologia_cliente tipologia = enum_cast<Tipologia_cliente>(campi[5]);
        aggiungiCliente(codice, nome, cognome, email, telefono, eta, tipologia);
        cout << "Cliente " << nome_cognome << " caricato correttamente." << endl;
    }
    return true;
}

bool Gestore_azienda::salvataggio_pacchetti(std::ofstream& file) const {

    auto salva_avventura = [](const shared_ptr<Pacchetto_avventura>& pacchetto_avventura) {
        file << enum_name(pacchetto_avventura->get_categoria_adrenalina()) << ";"
             << (pacchetto_avventura->has_assicurazione() ? "Con Assicurazione" : "Senza Assicurazione") << ";";
        for (const auto& attivita : pacchetto_avventura->get_lista_attivita()) {
            file << attivita << ";";
        }
        file << endl;
        cout << "Pacchetto Avventura " << pacchetto_avventura->get_codice() << " salvato correttamente." << endl;
    };

    auto salva_mare = [](const shared_ptr<Pacchetto_mare>& pacchetto_mare) {
        file << (pacchetto_mare->has_ombrellone_incluso() ? "Con Ombrellone" : "Senza Ombrellone") << ";"
             << (pacchetto_mare->has_attrezzatura_sportiva() ? "Con Attrezzatura" : "Senza Attrezzatura") << ";"
             << enum_name(pacchetto_mare->get_categoria_pensione()) << endl;
        cout << "Pacchetto Mare " << pacchetto_mare->get_codice() << " salvato correttamente." << endl;
    };

    auto salva_montagna = [](const shared_ptr<Pacchetto_montagna>& pacchetto_montagna) {
        file << (pacchetto_montagna->has_skipass_incluso() ? "Con Skipass" : "Senza Skipass") << ";"
             << pacchetto_montagna->get_numero_escursioni() << ";"
             << enum_name(pacchetto_montagna->get_difficolta()) << endl;
        cout << "Pacchetto Montagna " << pacchetto_montagna->get_codice() << " salvato correttamente." << endl;
    };

    auto salva_citta = [](const shared_ptr<Pacchetto_citta>& pacchetto_citta) {
        file << (pacchetto_citta->has_guida_inclusa() ? "Con Guida" : "Senza Guida") << ";"
             << pacchetto_citta->get_numero_musei() << ";";
             << enum_name(pacchetto_citta->get_categoria_hotel()) << endl;
        cout << "Pacchetto Città " << pacchetto_citta->get_codice() << " salvato correttamente." << endl;
    };

    for (const auto& pacchetto : this->catalogo) {
        file << pacchetto->get_codice() << ";"
             << pacchetto->get_destinazione() << ";";
             << pacchetto->get_durata_giorni() << ";"
             << (pacchetto->is_disponibile() ? "Disponibile" : "Non Disponibile") << ";";
             << pacchetto->calcola_prezzo_finale() << ";";
        string tipologia_pacchetto = enum_name(pacchetto->get_tipologia());
        file << tipologia_pacchetto << ";";

        // Salvataggio dei campi specifici in base alla tipologia
        if (tipologia_pacchetto == "Turismo Avventura") {
            salva_avventura(pacchetto);
        } else if (tipologia_pacchetto == "Turismo Balneare") {
            salva_mare(pacchetto);
        } else if (tipologia_pacchetto == "Turismo Montano") {
            salva_montagna(pacchetto);
        } else if (tipologia_pacchetto == "Turismo Culturale") {
            salva_citta(pacchetto);
        }
    }
    return true;
}

bool Gestore_azienda::carica_pacchetti(std::ifstream& file) {
    
    auto carica_avventura = [this](const vector<string>& campi) {
        vector<string> lista_attivita;
        Categoria_adrenalina categoria = enum_cast<Categoria_adrenalina>(campi[6]);
        bool assicurazione = (campi[7] == "Con Assicurazione");
        for (size_t i = 8; i < campi.size(); ++i) {
            lista_attivita.push_back(campi[i]);
        }
        aggiungiPacchetto(codice, destinazione, giorni, prezzo, lista_attivita, categoria, assicurazione);
        cout << "Pacchetto Avventura " << codice << " caricato correttamente." << endl;
    }

    auto carica_mare = [this](const vector<string>& campi) {
        bool ombrellone = (campi[6] == "Con Ombrellone");
        bool attrezzatura = (campi[7] == "Con Attrezzatura");
        Categoria_pensione tipo = enum_cast<Categoria_pensione>(campi[8]);
        aggiungiPacchetto(codice, destinazione, giorni, prezzo, ombrellone, attrezzatura, tipo);
        cout << "Pacchetto Mare " << codice << " caricato correttamente." << endl;
    }

    auto carica_montagna = [this](const vector<string>& campi) {
        bool skipass = (campi[6] == "Con Skipass");
        int numero_escursioni = stoi(campi[7]);
        Categoria_difficolta difficolta = enum_cast<Categoria_difficolta>(campi[8]);
        aggiungiPacchetto(codice, destinazione, giorni, prezzo, skipass, numero_escursioni, difficolta);
        cout << "Pacchetto Montagna " << codice << " caricato correttamente." << endl;
    }

    auto carica_citta = [this](const vector<string>& campi) {
        bool guida_inclusa = (campi[6] == "Con Guida");
        int numero_musei = stoi(campi[7]);
        Categoria_hotel categoria_hotel = enum_cast<Categoria_hotel>(campi[8]);
        aggiungiPacchetto(codice, destinazione, giorni, prezzo, numero_musei, guida_inclusa, categoria_hotel);
        cout << "Pacchetto Città " << codice << " caricato correttamente." << endl;
    }
    
    string linea;
    vector<string> campi;
    while (getline(file, linea)) {
        for (const auto& campo : split(linea, ';')) {
            campi.push_back(campo);
        }
        string codice = campi[0];
        string destinazione = campi[1];
        int giorni = stoi(campi[2]);
        bool disponibile = (campi[3] == "Disponibile");
        double prezzo = stod(campi[4]);
        string tipologia = campi[5];

        if (tipologia == "Turismo Avventura") {
            // Estrazione dei campi specifici Avventura
            carica_avventura(campi);

        } else if (tipologia == "Turismo Balneare") {
            // Estrazione dei campi specifici Mare
            carica_mare(campi);

        } else if (tipologia == "Turismo Montano") {
            // Estrazione dei campi specifici Montagna
            carica_montagna(campi);

        } else if (tipologia == "Turismo Culturale") {
            // Estrazione dei campi specifici Città
            carica_citta(campi);

        } else {
            cerr << "Tipologia di pacchetto non riconosciuta durante il caricamento: " << tipologia << endl;
        }
    }
    return true;
}

bool Gestore_azienda::salvataggio_prenotazioni(std::ofstream& file) const {
    for (const auto& prenotazione : this->prenotazioni) {
        file << prenotazione->get_codice() << ";"
             << prenotazione->get_cliente()->get_codice() << ";"
             << prenotazione->get_pacchetto()->get_codice() << ";"
             << prenotazione->get_numero_persone() << ";"
             << prenotazione->get_data_prenotazione() << ";"
             << (prenotazione->is_confermata() ? "Confermata" : "Non Confermata") << ";"
             << prenotazione->get_prezzo_totale() << endl;
            cout << "Prenotazione " << prenotazione->get_codice() << " salvata correttamente." << endl;
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

        shared_ptr<Cliente> cliente = cercaCliente(codice_cliente);
        shared_ptr<Pacchetto_viaggio> pacchetto = cercaPacchetto(codice_pacchetto);
        cout << "Prenotazione " << codice_prenotazione << " per il cliente " << codice_cliente << " e il pacchetto " << codice_pacchetto << " caricata correttamente" << endl;

        creaPrenotazione(codice_prenotazione, cliente, pacchetto, numero_persone, data_prenotazione);
        if (confermata) {
            prenotazioni[prenotazioni.size() - 1]->confermaPrenotazione();
            cout << "Prenotazione " << codice_prenotazione << " confermata durante il caricamento." << endl;
        }
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

                    auto nuovo_pacchetto = [this](string codice, string dest, int giorni, double prezzo,
                    bool ombrellone, bool attrezzatura, Categoria_pensione tipo) {
                    // Creazione del puntatore al nuovo pacchetto mare
                    shared_ptr<Pacchetto_mare> nuovo_pacchetto = make_shared<Pacchetto_mare>(codice, dest, giorni, prezzo, ombrellone, attrezzatura, tipo);

                    // Aggiunta al catalogo
                    shared_ptr<Pacchetto_mare> temp = aggiungi_elemento(this->catalogo, nuovo_pacchetto);
                    if (temp == nullptr) {
                        throw runtime_error("Errore nell'aggiunta del pacchetto mare al catalogo.");
                    }
                    return true;
                    };
                    return nuovo_pacchetto(codice, dest, giorni, prezzo, ombrellone, attrezzatura, tipo);
                }

// Metodo per l'aggiunta di un pacchetto montagna
bool Gestore_azienda::aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    bool skipass_incluso, int num_escursioni, Categoria_difficolta difficolta) {

                    // Creazione del puntatore al nuovo pacchetto montagna
                    shared_ptr<Pacchetto_montagna> nuovo_pacchetto = make_shared<Pacchetto_montagna>(codice, dest, giorni, prezzo, skipass_incluso, num_escursioni, difficolta);

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
bool Gestore_azienda::aggiungiCliente() {

    // Lambda function per la creazione del cliente e del suo puntatore
    auto crea_cliente = [this](string codice, string nome, string cognome, 
        string email, string tel, int eta, Tipologia_cliente tipo) {
        // Creazione del puntatore e del nuovo cliente            
        shared_ptr<Cliente> nuovo_cliente = crea_cliente(codice, nome, cognome, email, tel, eta, tipo);
        if (nuovo_cliente == nullptr) {
            return false; // Errore nella creazione del cliente
        }

        // Aggiunta alla lista clienti
        shared_ptr<Cliente> temp = aggiungi_elemento(this->clienti, nuovo_cliente);
        if (temp == nullptr) {
            return false;
        }
        return true;
    };

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
        Tipologia_cliente tipo = enum_cast<Tipologia_cliente>(tipo_str);
        return true;
    };

    // Corpo del metodo
    do {
        string nome; string cognome; string email; string telefono; int eta; string tipo_str; Tipologia_cliente tipo;
        inserimento_dati_cliente(nome, cognome, email, telefono, eta, tipo_str, tipo);

        if (crea_cliente(generaCodiceUnico('C'), nome, cognome, email, telefono, eta, tipo)) {
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
    return stampa_elementi_per_tipologia(this->clienti, tipo);
}


// === GESTIONE PRENOTAZIONI ===
bool Gestore_azienda::aggiungiPrenotazione() {
                    
    // Lambda function per la creazione della prenotazione e del suo puntatore
    auto crea_prenotazione = [this](string codice, shared_ptr<Cliente> cliente, shared_ptr<Pacchetto_viaggio> pacchetto_viaggio,
        int num_persone, string data) {
        // Creazione del puntatore e della nuova prenotazione
        shared_ptr<Prenotazione> nuova_prenotazione = Prenotazione::crea_prenotazione(codice, cliente, pacchetto_viaggio, num_persone, data);
        if (nuova_prenotazione == nullptr) {
            return false;
        }

        // Aggiunta alla lista prenotazioni
        shared_ptr<Prenotazione> temp = aggiungi_elemento(this->prenotazioni, nuova_prenotazione);
        if (temp == nullptr) {
            return false;
        }
        return true;
    };

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

        if (crea_prenotazione(generaCodiceUnico('B'), cliente, pacchetto_viaggio, num_persone, data)) {
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
bool Gestore_azienda::salvaDatiSuFile(const string& nomefile, const string& tipo) const {
    ofstream file(nomefile);
    if (!file) {cerr << "Errore nell'apertura del file per la scrittura: " << nomefile << endl; return false;}

    // Salvataggio clienti
    if (tolower(tipo) == "clienti") {
        salvataggio_clienti(file);
        
    // Salvataggio pacchetti
    } else if (tolower(tipo) == "pacchetti") {
        salvataggio_pacchetti(file);
    
    // Salvataggio prenotazioni
    } else if (tolower(tipo) == "prenotazioni") {
        salvataggio_prenotazioni(file);

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
        cerr << "Tipo di dato non riconosciuto per il caricamento: " << tipo << endl;
        file.close();
        return false;
    }
    return true;
}