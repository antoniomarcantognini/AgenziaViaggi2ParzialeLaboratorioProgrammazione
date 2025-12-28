#include "Gestore_azienda.h"
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

using namespace std;

// Dichiarazione Globale per visibilità nelle funzioni menu
static vector<shared_ptr<Pacchetto_viaggio>> catalogo;
static vector<shared_ptr<Cliente>> clienti;
static vector<shared_ptr<Prenotazione>> prenotazioni;
static Gestore_agenzia gestore(catalogo, clienti, prenotazioni);

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
            case 6: if(uscita()); return 0;
        }
    } while(1);
}

bool uscita() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SALVATAGGIO IN USCITA","Salvare dati su un file","Non salvare i dati","Annulla uscita"});
    switch (scelta) {
        case 1: 
            do{
                salvataggio_dati(); 
                int scelta_2 menu::stampa_menu_e_scelta("MENU SALVATAGGIO IN USCITA","Salvare altri tipi di dato o su altri file","Non salvare altri dati","Annulla uscita");
                switch (scelta_2) {
                    case 1: continue;
                    case 2: return true;
                    case 3: return false;
                }while(1);
            }
        case 2: return true;
        case 3: return false;
    }
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
    return gestore.salvaDatiSuFile(nome_file, tipo);
}

bool caricamento_dati() {
    string nome_file; string tipo;
    cout << "Inserisci il nome del file: ";
    cin >> nome_file;
    cout << "Inserisci il tipo di variabile che deve essere caricata da questo file (Cliente, Pacchetto o Prenotazione): ";
    cin >> tipo;
    return gestore.caricaDatiDaFile(nome_file, tipo);
}

bool menu_statistiche_report() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU STATISTICHE E REPORT","Statistiche generali","Statistiche per tipologia di pacchetto","Trova cliente migliore","Report delle destinazioni più popolari"});
        switch (scelta) {
            case 1: gestore.statisticheGenerali(); break;
            case 2: gestore.statistichePerTipologia() break;
            case 3: gestore.clienteMigliore(); break;
            case 4: stampa_destinazioni_prenotate(); break;
            case 5: return true;
        }
    }while(1);
}

bool stampa_destinazioni_prenotate() {
    vector<pair<string, int>> destinazioni_ordinate(destinazioni.begin(), destinazioni.end());
    sort(destinazioni_ordinate.begin(), destinazioni_ordinate.end(), 
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second < b.second;
        }
    )
    for (const auto& elementi : destinazioni_ordinate) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

bool menu_prenotazioni() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE PRENOTAZIONI","Crea una nuova prenotazione","Conferma prenotazione","Visualizza tutte le prenotazioni", "Visualizza prenotazioni confermate", "Torna al menù principale"});
        if (gestore->prenotazioni.size() == 0 && scelta != 1 && scelta != 6) {
            cout << "Non sono presenti prenotazioni nel database! Puoi solo scegliere 1 o 6!" << endl;
            continue;
        }
        switch (scelta) {
            case 1: gestore.aggiungiPrenotazione();
            case 2: conferma_prenotazioni();
            case 3: gestore.visualizzaPrenotazioni();
            case 4: visualizza_prenotazioni_cliente();
            case 5: gestore.visualizzaPrenotazioniConfermate();
            case 6: return true;
        }
    }while(1);
}

bool conferma_prenotazioni() {
    do{
        string src;
        cout << "Inserisci il codice univoco del pacchetto (PKG-XXXX, dove X è una cifra): ";
        cin >> src;
        if(!gestore.confermaPrenotazione(src)) {
            int scelta = menu::stampa_menu_e_scelta("MENU CONFERMA PRENOTAZIONE","Inserisci un altro codice univoco", "Torna al menù gestione prenotazioni");
            if (scelta == 2) return false;
        } else return true;
    }while(1);
}

bool visualizza_prenotazioni_cliente() {
    do{
        string src;
        cout << "Inserisci il codice univoco del cliente (CLT-XXXX, dove X è una cifra): ";
        cin >> src;
        if(!gestore.visualizzaPrenotazioniCliente(src)) {
            int scelta = menu::stampa_menu_e_scelta("MENU CONFERMA PRENOTAZIONE","Inserisci un altro codice univoco", "Torna al menù gestione prenotazioni");
            if (scelta == 2) return false;
        } else return true;
    }
}

bool menu_clienti() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE CLIENTI","Registra un nuovo cliente","Visualizza tutti i clienti","Cerca cliente per codice","Visualizza clienti per tipologia","Torna al menù principale"});
        if (gestore->clienti.size() == 0 && scelta != 1 && scelta != 5) {
            cout << "Non sono presenti clienti nel database! Puoi solo scegliere 1 o 5!" << endl;
            continue;
        }
        switch (scelta) {
            case 1: gestore.aggiungiCliente();
            case 2: gestore.visualizzaClienti();
            case 3: ricerca_cliente();
            case 4: visualizza_tipologia_clienti();
            case 5: return true;
        }
    }while(1);
}

bool ricerca_cliente() {
    string src;
    cout << "Inserisci il codice univoco del pacchetto (CLT-XXXX, dove X è una cifra): ";
    cin >> src;
    shared_ptr<Pacchetto_viaggio> src_ptr = gestore.cercaCliente(src);
    if (src_ptr == nullptr) return false;
    else src_ptr->stampa_dettagli();
}

bool visualizza_tipologia_clienti() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SCELTA TIPOLOGIA CLIENTE","Standard","Premium","VIP"});
    switch (scelta) {
        case 1: gestore.visualizzaClientiPerTipologia("Standard");
        case 2: gestore.visualizzaClientiPerTipologia("Premium");
        case 3: gestore.visualizzaClientiPerTipologia("VIP");
    }
}

bool menu_pacchetti() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE PACCHETTI","Aggiungi un nuovo pacchetto","Visualizza tutti i pacchetti","Cerca pacchetto tramite codice","Visualizza pacchetti per tipologia","Visualizza solo pacchetti disponibili","Torna al menu principale"});
        if (gestore->catalogo.size() == 0 && scelta != 1 && scelta != 6) {
            cout << "Non sono presenti pacchetit nel catalogo! Puoi solo scegliere 1 o 6!" << endl;
            continue;
        }
        switch (scelta) {
            case 1: gestore.aggiungiPacchetto();
            case 2: gestore.visualizzaCatalogo();
            case 3: ricerca_pacchetto();
            case 4: visualizza_tipologia_pacchetti();
            case 5: gestore.visualizzaPacchettiDisponibili();
            case 6: return true;
        }
    }while(1);
}

bool ricerca_pacchetto() {
    string src;
    cout << "Inserisci il codice univoco del pacchetto (PKG-XXXX, dove X è una cifra): ";
    cin >> src;
    shared_ptr<Pacchetto_viaggio> src_ptr = gestore.cercaPacchetto(src);
    if (src_ptr == nullptr) return false;
    else src_ptr->stampa_dettagli();
}

bool visualizza_tipologia_pacchetti() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SCELTA TIPOLOGIA PACCHETTO","Turismo Avventura","Turismo Balneare","Turismo Montano","Città d'Arte"});
    switch (scelta) {
        case 1: gestore.visualizzaPacchettiPerTipologia("Turismo Avventura");
        case 2: gestore.visualizzaPacchettiPerTipologia("Turismo Balneare");
        case 3: gestore.visualizzaPacchettiPerTipologia("Turismo Montano");
        case 4: gestore.visualizzaPacchettiPerTipologia("Città d'Arte");
    }
}
