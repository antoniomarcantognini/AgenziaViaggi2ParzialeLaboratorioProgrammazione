#include "Gestore_azienda.h"
#include "menu.h"
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

// Istanza globale del gestore
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

// Main
int main() {
    system("chcp 65001 > nul");
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU PRINCIPALE","Gestione catalogo pacchetti","Gestione clienti","Gestione prenotazioni","Statistiche e report","Gestione file","Esci"});

        switch (scelta) {
            case 1: menu_pacchetti(); break;
            case 2: menu_clienti(); break;
            case 3: menu_prenotazioni(); break;
            case 4: menu_statistiche_report(); break;
            case 5: menu_file(); break;
            case 6: 
                if(uscita()){
                    cout << "Chiusura applicazione. Arrivederci !" << endl;
                    return 0;
                }
                break;
        }
    } while(1);
}

// Implementazione funzioni
bool uscita() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SALVATAGGIO IN USCITA","Salvare dati su un file","Non salvare i dati","Annulla uscita"});
    switch (scelta) {
        case 1: 
            do{
                if(salvataggio_dati()) cout << "Salvataggio riuscito." << endl; 
                else cerr << "Salvataggio fallito o annullato." << endl;

                int scelta_2 = menu::stampa_menu_e_scelta({"MENU SALVATAGGIO IN USCITA","Salvare altri tipi di dato o su altri file","Non salvare altri dati","Annulla uscita"});
                
                switch (scelta_2) {
                    case 1: continue;
                    case 2: return true; // esce dal programma
                    case 3: return false; // annulla l'uscita
                }
            }while(1);
        case 2: return true;  // Esce senza salvare
        case 3: return false; // Annulla
    }
    return false;
}

bool menu_file() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU FILE","Salva dati su file","Carica dati da file","Torna al menu principale"});
        bool esito = false;
        switch (scelta) {
            case 1: 
                esito = salvataggio_dati(); 
                if(esito) cout << "Operazione completata con successo." << endl;
                break;
            case 2: 
                esito = caricamento_dati();
                if(esito) cout << "Operazione completata con successo." << endl;
                break;
            case 3: return true; // Torna al main
        }
    }while(1);
}

bool salvataggio_dati() {
    string nome_file; string tipo;
    cout << "Inserisci il nome del file: ";
    cin >> nome_file;
    cout << "Inserisci il tipo di variabile che deve essere salvata su questo file (Cliente, Pacchetto o Prenotazione): ";
    cin >> tipo;
    // Ritorna true se il gestore salva correttamente
    return gestore.salva_dati_su_file(nome_file, tipo);
}

bool caricamento_dati() {
    string nome_file; 
    string tipo;
    
    cout << "Inserisci il nome del file: ";
    cin >> nome_file;
    cout << "Inserisci il tipo di variabile che deve essere caricata (Cliente, Pacchetto o Prenotazione): ";
    cin >> tipo;

    // Proviamo a caricare i dati dal file
    bool esito_caricamento = gestore.carica_dati_da_file(nome_file, tipo);

    // Se il caricamento è andato a buon fine, sincronizziamo le statistiche statiche
    if (esito_caricamento) {
        
        if (!gestore.sincronizza_statistiche()) {
            cout << "Nota: Dati caricati, ma nessuna statistica da aggiornare (liste vuote?)." << endl;
        }
        return true; 
    }

    return false;
}

bool menu_statistiche_report() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU STATISTICHE E REPORT","Statistiche generali","Statistiche per tipologia di pacchetto","Trova cliente migliore","Report delle destinazioni piu' popolari","Torna al menu principale"});
        
        switch (scelta) {
            case 1: gestore.statistiche_generali(); break;
            case 2: gestore.statistiche_per_tipologia(); break;
            case 3: {
                auto migliore = gestore.cliente_migliore();
                if(migliore) {
                    cout << migliore->stampa_dettagli() << endl;
                } else {
                    cout << "Nessun cliente o dati insufficienti." << endl;
                }
                break;
            }
            case 4: 
                if(!stampa_destinazioni_prenotate()) cout << "Nessun dato sulle destinazioni disponibile." << endl;
                break;
            case 5: return true;  // Torna indietro
        }
    }while(1);
}

bool stampa_destinazioni_prenotate() {
    const auto& mappa = Prenotazione::get_destinazioni_counter();

    if(mappa.empty()) return false; // Segnala che non c'erano dati

    vector<pair<string, int>> destinazioni_ordinate(mappa.begin(), mappa.end());
    
    sort(destinazioni_ordinate.begin(), destinazioni_ordinate.end(), 
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        }
    );

    cout << "--- Destinazioni piu' popolari ---" << endl;
    for (const auto& elem : destinazioni_ordinate) {
        cout << elem.first << ": " << elem.second << " prenotazioni" << endl;
    }
    return true; // Stampa avvenuta con successo
}

bool menu_prenotazioni() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE PRENOTAZIONI","Crea una nuova prenotazione","Conferma prenotazione","Visualizza tutte le prenotazioni", "Visualizza prenotazioni confermate", "Torna al menu principale"});
        
        switch (scelta) {
            case 1: 
                if(gestore.aggiungi_prenotazione()) cout << "Prenotazione aggiunta." << endl;
                else cerr << "Operazione annullata o fallita." << endl;
                break;
            case 2: 
                if(conferma_prenotazioni()) cout << "Conferma registrata." << endl;
                break;
            case 3: 
                if(!gestore.visualizza_prenotazioni()) cout << "Lista vuota." << endl;
                break;
            case 4: 
                if(!visualizza_prenotazioni_cliente()) cout << "Nessuna prenotazione trovata per questo cliente." << endl;
                break;
            case 5: 
                return true;
            
            default:
                cout << "Scelta non valida." << endl;
        }
    }while(1);
}

bool conferma_prenotazioni() {
    do{
        string src;
        cout << "Inserisci il codice univoco della prenotazione (BKG-XXXX, dove X e' una cifra): ";
        cin >> src;
        
        if(!gestore.conferma_prenotazione(src)) {
            // Se fallisce, chiediamo se vuole riprovare
            int scelta = menu::stampa_menu_e_scelta({"MENU CONFERMA PRENOTAZIONE","Inserisci un altro codice univoco", "Torna al menu gestione prenotazioni"});
            if (scelta == 2) return false; // Operazione non completata
        } else {
            return true; // Successo
        }
    }while(1);
}

bool visualizza_prenotazioni_cliente() {
    do{
        string src;
        cout << "Inserisci il codice univoco del cliente (CLT-XXXX, dove X e' una cifra): ";
        cin >> src;

        if(!gestore.visualizza_prenotazioni_cliente(src)) {
            // Se fallisce chiediamo se vuole riprovare
            int scelta = menu::stampa_menu_e_scelta({"MENU CONFERMA PRENOTAZIONE","Inserisci un altro codice univoco", "Torna al menu gestione prenotazioni"});
            if (scelta == 2) return false; // Annullamento
        } else {
            return true;  // Successo
        }
    } while(1);
}

bool menu_clienti() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE CLIENTI","Registra un nuovo cliente","Visualizza tutti i clienti","Cerca cliente per codice","Visualizza clienti per tipologia","Torna al menu principale"});
        
        switch (scelta) {
            case 1: 
                if(gestore.aggiungi_cliente()) cout << "Cliente registrato." << endl;
                else cerr << "Inserimento annullato o fallito." << endl;
                break;
            case 2: 
                if(!gestore.visualizza_clienti()) cout << "Nessun cliente in archivio." << endl;
                break;
            case 3: 
                if(!ricerca_cliente()) cout << "Ricerca senza risultati." << endl;
                break;
            case 4: 
                visualizza_tipologia_clienti(); 
                break;
            case 5: return true;
        }
    }while(1);
}

bool ricerca_cliente() {
    string src;
    cout << "Inserisci il codice univoco del cliente (CLT-XXXX, dove X e' una cifra): ";
    cin >> src;

    auto cliente_ptr = gestore.cerca_cliente(src);
    if (cliente_ptr == nullptr) {
        return false; // Non trovato
    } else {
        cout << cliente_ptr->stampa_dettagli() << endl;
        return true; // Trovato e stampato
    }
}

bool visualizza_tipologia_clienti() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SCELTA TIPOLOGIA CLIENTE","Standard","Premium","VIP"});
    bool found = false;
    switch (scelta) {
        case 1: found = gestore.visualizza_clienti_per_tipologia("Standard"); break;
        case 2: found = gestore.visualizza_clienti_per_tipologia("Premium"); break;
        case 3: found = gestore.visualizza_clienti_per_tipologia("VIP"); break;
    }
    return found;  // Ritorna true se ha trovato qualcosa, false altrimenti
}

bool menu_pacchetti() {
    do{
        int scelta = menu::stampa_menu_e_scelta({"MENU GESTIONE PACCHETTI","Aggiungi un nuovo pacchetto","Visualizza tutti i pacchetti","Cerca pacchetto tramite codice","Visualizza pacchetti per tipologia","Visualizza solo pacchetti disponibili","Torna al menu principale"});
        
        switch (scelta) {
            case 1: 
                if(gestore.aggiungi_pacchetto()) cout << "Pacchetto aggiunto." << endl;
                break;
            case 2: 
                gestore.visualizza_catalogo(); 
                break;
            case 3: 
                if(!ricerca_pacchetto()) cout << "Pacchetto non trovato." << endl;
                break;
            case 4: 
                visualizza_tipologia_pacchetti(); 
                break;
            case 5: 
                if(!gestore.visualizza_pacchetti_disponibili()) cout << "Nessun pacchetto disponibile." << endl;
                break;
            case 6: return true;
        }
    }while(1);
}

bool ricerca_pacchetto() {
    string src;
    cout << "Inserisci il codice univoco del pacchetto (PKG-XXXX, dove X e' una cifra): ";
    cin >> src;
    
    auto pacchetto_ptr = gestore.cerca_pacchetto(src);
    if (pacchetto_ptr == nullptr) {
        return false;
    } else {
        cout << pacchetto_ptr->stampa_dettagli() << endl;
        return true;
    }
}

bool visualizza_tipologia_pacchetti() {
    int scelta = menu::stampa_menu_e_scelta({"MENU SCELTA TIPOLOGIA PACCHETTO","Turismo Avventura","Turismo Balneare","Turismo Montano","Citta' d'Arte"});
    bool found = false;
    switch (scelta) {
        case 1: found = gestore.visualizza_pacchetti_per_tipologia("Turismo Avventura"); break;
        case 2: found = gestore.visualizza_pacchetti_per_tipologia("Turismo Balneare"); break;
        case 3: found = gestore.visualizza_pacchetti_per_tipologia("Turismo Montano"); break;
        case 4: found = gestore.visualizza_pacchetti_per_tipologia("Citta' d'Arte"); break;
    }
    return found;
}