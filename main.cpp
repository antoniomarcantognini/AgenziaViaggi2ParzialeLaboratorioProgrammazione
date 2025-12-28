#include "Gestore_azienda.h"
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

using namespace std;

// Inizializzazione membri statici
int Cliente::clienti_creati = 0;
int Pacchetto_viaggio::pacchetti_creati = 0;
int Prenotazione::prenotazioni_create = 0;
double Prenotazione::fatturato_totale = 0.0;
std::unordered_map<std::string, int> Prenotazione::destinazioni_counter; 
std::unordered_map<std::string, int> Prenotazione::pacchetti_counter;   
std::unordered_map<std::string, int> Prenotazione::spesa_clienti;

// Dichiarazione Globale per visibilità nelle funzioni menu
static vector<shared_ptr<Pacchetto_viaggio>> catalogo;
static vector<shared_ptr<Cliente>> clienti;
static vector<shared_ptr<Prenotazione>> prenotazioni;
static Gestore_azienda gestore(catalogo, clienti, prenotazioni);

// Prototipi
bool uscita();
bool menu_file();
bool salvataggio_dati();
bool caricamento_dati();
bool menu_statistiche_report();
bool stampa_destinazioni_prenotate();
bool menu_prenotazioni();
bool conferma_prenotazioni();
bool visualizza_prenotazioni_cliente();
bool menu_clienti();
bool ricerca_cliente();
bool visualizza_tipologia_clienti();
bool menu_pacchetti();
bool ricerca_pacchetto();
bool visualizza_tipologia_pacchetti();

int main() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU PRINCIPALE","Gestione catalogo pacchetti","Gestione clienti","Gestione prenotazioni","Statistiche e report","Gestione file","Esci"});
        switch (scelta) {
            case 1: menu_pacchetti(); break;
            case 2: menu_clienti(); break;
            case 3: menu_prenotazioni(); break;
            case 4: menu_statistiche_report(); break;
            case 5: menu_file(); break;
            case 6: if(uscita()) return 0; break;
        }
    } while(1);
}

bool uscita() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SALVATAGGIO IN USCITA","Salvare dati su un file","Non salvare i dati","Annulla uscita"});
    switch (scelta) {
        case 1: 
            do{
                if (!salvataggio_dati()) {
                    cerr << "Errore durante il salvataggio." << endl;
                }
                int scelta_2 = menu::stampa_menu_e_scelta({"MENU SALVATAGGIO IN USCITA","Salvare altri tipi di dato o su altri file","Non salvare altri dati","Annulla uscita"});
                switch (scelta_2) {
                    case 1: continue;
                    case 2: return true;
                    case 3: return false;
                }
            }while(1);
        case 2: return true;
        case 3: return false;
    }
    return false;
}

bool menu_file() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU FILE","Salva dati su file","Carica dati da file","Torna al menù principale"});
        switch (scelta) {
            case 1: salvataggio_dati(); break;
            case 2: caricamento_dati(); break;
            case 3: return true;
        }
    }while(1);
}

bool salvataggio_dati() {
    string nome_file; string tipo;
    cout << "Inserisci il nome del file: ";
    cin >> nome_file;
    cout << "Inserisci il tipo di variabile che deve essere salvata su questo file (Cliente, Pacchetto o Prenotazione): ";
    cin >> tipo;
    if (!gestore.salvaDatiSuFile(nome_file, tipo)) {
        cerr << "Salvataggio fallito." << endl;
        return false;
    }
    return true;
}

bool caricamento_dati() {
    string nome_file; string tipo;
    cout << "Inserisci il nome del file: ";
    cin >> nome_file;
    cout << "Inserisci il tipo di variabile che deve essere caricata da questo file (Cliente, Pacchetto o Prenotazione): ";
    cin >> tipo;
    if (!gestore.caricaDatiDaFile(nome_file, tipo)) {
        cerr << "Caricamento fallito." << endl;
        return false;
    }
    return true;
}

bool menu_statistiche_report() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU STATISTICHE E REPORT","Statistiche generali","Statistiche per tipologia di pacchetto","Trova cliente migliore","Report delle destinazioni più popolari"});
        switch (scelta) {
            case 1: gestore.statisticheGenerali(); break;
            case 2: gestore.statistichePerTipologia(); break;
            case 3: gestore.clienteMigliore(); break;
            case 4: stampa_destinazioni_prenotate(); break;
            case 5: return true;
        }
    }while(1);
}

bool stampa_destinazioni_prenotate() {
    if (Prenotazione::destinazioni_counter.empty()) {
        cout << "Nessuna destinazione registrata." << endl;
        return false;
    }
    vector<pair<string, int>> destinazioni_ordinate(Prenotazione::destinazioni_counter.begin(), Prenotazione::destinazioni_counter.end());
    
    sort(destinazioni_ordinate.begin(), destinazioni_ordinate.end(), 
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second; // Decrescente
        }
    ); // Mancava ;
    
    for (const auto& elementi : destinazioni_ordinate) {
        cout << elementi.first << ": " << elementi.second << endl;
    }
    return true;
}

bool menu_prenotazioni() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE PRENOTAZIONI","Crea una nuova prenotazione","Conferma prenotazione","Visualizza tutte le prenotazioni", "Visualizza prenotazioni confermate", "Torna al menù principale"});
        // Nota: i vettori ora sono globali o accessibili via gestore, ma prenotazioni è private in gestore. 
        // Si dovrebbe usare un metodo di gestore per controllare size o rimuovere il controllo qui.
        // Assumo rimozione controllo diretto per incapsulamento o gestione via metodi.
        
        switch (scelta) {
            case 1: gestore.aggiungiPrenotazione(); break;
            case 2: conferma_prenotazioni(); break;
            case 3: gestore.visualizzaPrenotazioni(); break;
            case 4: visualizza_prenotazioni_cliente(); break;
            case 5: gestore.visualizzaPrenotazioniConfermate(); break;
            case 6: return true;
        }
    }while(1);
}

bool conferma_prenotazioni() {
    do{
        string src;
        cout << "Inserisci il codice univoco della prenotazione (BKG-XXXX): ";
        cin >> src;
        if(!gestore.confermaPrenotazione(src)) {
            int scelta = menu::stampa_menu_e_scelta({"MENU CONFERMA PRENOTAZIONE","Inserisci un altro codice univoco", "Torna al menù gestione prenotazioni"});
            if (scelta == 2) return false;
        } else return true;
    }while(1);
}

bool visualizza_prenotazioni_cliente() {
    do{
        string src;
        cout << "Inserisci il codice univoco del cliente (CLT-XXXX): ";
        cin >> src;
        if(!gestore.visualizzaPrenotazioniCliente(src)) {
            int scelta = menu::stampa_menu_e_scelta({"MENU CONFERMA PRENOTAZIONE","Inserisci un altro codice univoco", "Torna al menù gestione prenotazioni"});
            if (scelta == 2) return false;
        } else return true;
    } while(1); // Mancava while
}

bool menu_clienti() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE CLIENTI","Registra un nuovo cliente","Visualizza tutti i clienti","Cerca cliente per codice","Visualizza clienti per tipologia","Torna al menù principale"});
        switch (scelta) {
            case 1: gestore.aggiungiCliente(); break;
            case 2: gestore.visualizzaClienti(); break;
            case 3: ricerca_cliente(); break;
            case 4: visualizza_tipologia_clienti(); break;
            case 5: return true;
        }
    }while(1);
}

bool ricerca_cliente() {
    string src;
    cout << "Inserisci il codice univoco del cliente (CLT-XXXX): ";
    cin >> src;
    shared_ptr<Cliente> src_ptr = gestore.cercaCliente(src);
    if (src_ptr == nullptr) return false;
    else {
        cout << src_ptr->stampa_dettagli() << endl;
        return true;
    }
}

bool visualizza_tipologia_clienti() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SCELTA TIPOLOGIA CLIENTE","Standard","Premium","VIP"});
    switch (scelta) {
        case 1: return gestore.visualizzaClientiPerTipologia("Standard");
        case 2: return gestore.visualizzaClientiPerTipologia("Premium");
        case 3: return gestore.visualizzaClientiPerTipologia("VIP");
    }
    return false;
}

bool menu_pacchetti() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE PACCHETTI","Aggiungi un nuovo pacchetto","Visualizza tutti i pacchetti","Cerca pacchetto tramite codice","Visualizza pacchetti per tipologia","Visualizza solo pacchetti disponibili","Torna al menu principale"});
        switch (scelta) {
            case 1: gestore.aggiungiPacchetto(); break;
            case 2: gestore.visualizzaCatalogo(); break;
            case 3: ricerca_pacchetto(); break;
            case 4: visualizza_tipologia_pacchetti(); break;
            case 5: gestore.visualizzaPacchettiDisponibili(); break;
            case 6: return true;
        }
    }while(1);
}

bool ricerca_pacchetto() {
    string src;
    cout << "Inserisci il codice univoco del pacchetto (PKG-XXXX): ";
    cin >> src;
    shared_ptr<Pacchetto_viaggio> src_ptr = gestore.cercaPacchetto(src);
    if (src_ptr == nullptr) return false;
    else {
        cout << src_ptr->stampa_dettagli() << endl;
        return true;
    }
}

bool visualizza_tipologia_pacchetti() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SCELTA TIPOLOGIA PACCHETTO","Turismo Avventura","Turismo Balneare","Turismo Montano","Città d'Arte"});
    switch (scelta) {
        case 1: return gestore.visualizzaPacchettiPerTipologia("Turismo Avventura");
        case 2: return gestore.visualizzaPacchettiPerTipologia("Turismo Balneare");
        case 3: return gestore.visualizzaPacchettiPerTipologia("Turismo Montano");
        case 4: return gestore.visualizzaPacchettiPerTipologia("Città d'Arte");
    }
    return false;
}
