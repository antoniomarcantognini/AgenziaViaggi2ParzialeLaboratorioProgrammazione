#include "Gestore_azienda.h"
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

using namespace std;

int main() {
    vector<shared_ptr<Pacchetto_viaggio>> catalogo;
    vector<shared_ptr<Cliente>> clienti;
    vector<shared_ptr<Prenotazione>> prenotazioni;
    Gestore_azienda gestore = Gestore_azienda(catalogo, clienti, prenotazioni);
    do{
        int scelta = menu::stampa_menu_e_scelta(vector<string> nome_opzioni = {"MENU PRINCIPALE","Gestione catalogo pacchetti","Gestione clienti","Gestione prenotazioni","Statistiche e report","Gestione file","Esci"});
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

bool menu_prenotazioni() {
    do{
        int scelta = menu::stampa_menu_e_scelta(vector<string> nome_opzioni = {"MENU GESTIONE PRENOTAZIONI","Crea una nuova prenotazione","Conferma prenotazione","Visualizza tutte le prenotazioni", "Visualizza prenotazioni confermate", "Torna al menù principale"});
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
        int scelta = menu::stampa_menu_e_scelta(vector<string> nome_opzioni = {"MENU GESTIONE CLIENTI","Registra un nuovo cliente","Visualizza tutti i clienti","Cerca cliente per codice","Visualizza clienti per tipologia","Torna al menù principale"});
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
    int scelta = menu::stampa_menu_e_scelta(vector<string> nome_opzioni = {"MENU SCELTA TIPOLOGIA CLIENTE","Standard","Premium","VIP"});
    switch (scelta) {
        case 1: gestore.visualizzaClientiPerTipologia("Standard");
        case 2: gestore.visualizzaClientiPerTipologia("Premium");
        case 3: gestore.visualizzaClientiPerTipologia("VIP");
    }
}

bool menu_pacchetti() {
    do{
        int scelta = menu::stampa_menu_e_scelta(vector<string> nome_opzioni = {"MENU GESTIONE PACCHETTI","Aggiungi un nuovo pacchetto","Visualizza tutti i pacchetti","Cerca pacchetto tramite codice","Visualizza pacchetti per tipologia","Visualizza solo pacchetti disponibili","Torna al menu principale"});
        switch (scelta) {
            case 1: aggiunta_pacchetto();
            case 2: gestore.visualizzaCatalogo();
            case 3: ricerca_pacchetto();
            case 4: visualizza_tipologia_pacchetti();
            case 5: gestore.visualizzaPacchettiDisponibili();
            case 6: return true;
        }
    }while(1);
}

bool aggiunta_pacchetto() {
    int scelta = menu::stampa_menu_e_scelta(vector<string> nome_opzioni = {"MENU SCELTA TIPOLOGIA PACCHETTO","Turismo Avventura","Turismo Balneare","Turismo Montano","Città d'arte"});
    switch (scelta) {
        case 1: gestore.aggiungiPacchettoAvventura(); return true;
        case 2: gestore.aggiungiPacchettoMare(); return true;
        case 3: gestore.aggiungiPacchettoMontagna(); return true;
        case 4: gestore.aggiungiPacchettoCitta(); return true;
    }
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
    int scelta = menu::stampa_menu_e_scelta(vector<string> nome_opzioni = {"MENU SCELTA TIPOLOGIA PACCHETTO","Turismo Avventura","Turismo Balneare","Turismo Montano","Città d'Arte"});
    switch (scelta) {
        case 1: gestore.visualizzaPacchettiPerTipologia("Turismo Avventura");
        case 2: gestore.visualizzaPacchettiPerTipologia("Turismo Balneare");
        case 3: gestore.visualizzaPacchettiPerTipologia("Turismo Montano");
        case 4: gestore.visualizzaPacchettiPerTipologia("Città d'Arte");
    }
}