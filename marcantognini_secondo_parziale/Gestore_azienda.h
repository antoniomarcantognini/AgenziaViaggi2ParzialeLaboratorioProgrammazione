#pragma once

#ifndef GESTORE_AZIENDA_H
#define GESTORE_AZIENDA_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <unordered_map>
#include <optional>
#include <algorithm>

#include "Pacchetto_viaggio.h"
#include "Cliente.h"
#include "Prenotazione.h"
#include "magic_enum.hpp"
#include "Esito_input_SN.h"
#include "Pacchetto_avventura.h"
#include "Pacchetto_mare.h"      
#include "Pacchetto_montagna.h"  
#include "Pacchetto_citta.h"     
#include "Utils_enum.h"          

using namespace std;

class Gestore_azienda {

private:
    
    std::vector<std::shared_ptr<Pacchetto_viaggio>> catalogo;
    std::vector<std::shared_ptr<Cliente>> clienti;
    std::vector<std::shared_ptr<Prenotazione>> prenotazioni;

    // Mappa per il factory: aggiornata per ritornare bool e gestire i tipi corretti
    map<string, function<bool(string&, string&, int&, bool&, double&, const vector<string>&, int&)>> mappa_caricamento_specifico;

    // I contatori per generare codici univoci sono statici e definiti nelle rispettive classi (Getter usati nel .cpp)

    // === METODI HELPER PRIVATI (Input e Utility) ===
    
    // Helper statici (non modificano lo stato della classe)
    static vector<string> split(const string& s, char delimiter);
    static Esito_input_SN analizza_input_sn(const string& input);
    static bool valida_inserimento_sn(string& stringa_s_n, bool& flag_da_aggiornare);
    
    // Helper membri (hanno bisogno di accedere a this o altri metodi)
    string tolower_string(string stringa);
    bool assegna_valore_cliente(vector<string>& campi, int& numero_riga, string& codice, string& nome, string& cognome, string& email, string& telefono, int& eta, Tipologia_cliente& tipologia);

    // Funzioni di inserimento manuale dei dati da tastiera (Overload)
    bool inserisci_dati_pacchetto_base(string& dest, int& giorni, double& prezzo);
    bool inserisci_dati_pacchetto(vector<string>& lista_attivita, Categoria_adrenalina& categoria, bool& assicurazione);
    bool inserisci_dati_pacchetto(bool& ombrellone, bool& attrezzatura, Categoria_pensione& tipo);
    bool inserisci_dati_pacchetto(bool& skipass_incluso, int& num_escursioni, Categoria_difficolta& difficolta);
    bool inserisci_dati_pacchetto(int& num_musei, bool& guida, Categoria_hotel& hotel);
    
    bool inserimento_dati_cliente(string& nome, string& cognome, string& email, string& telefono, int& eta, string& tipo_str, Tipologia_cliente& tipo);
    bool inserimento_dati_prenotazione(shared_ptr<Cliente>& cliente, shared_ptr<Pacchetto_viaggio>& pacchetto_viaggio, int& num_persone, string& data);

    // === METODI PRIVATI DI UTILITY ===
    
    // Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0005")
    std::string genera_codice_unico(char tipo);

    // Metodo di inizializzazione della mappa (da usare nel costruttore)
    bool inizializza_mappa();

    // Metodi per il caricamento di specifiche classi
    bool carica_clienti(std::ifstream& file); 
    bool carica_pacchetti(std::ifstream& file);    
    bool carica_prenotazioni(std::ifstream& file);
    
public:
    // === TEMPLATES === //

    // Template per l'aggiunta di un elemento ad un vettore
    template <typename T>
    std::shared_ptr<T> aggiungi_elemento(vector<std::shared_ptr<T>>& lista, std::shared_ptr<T> elemento);

    // Template per la ricerca di un elemento dato il codice e la lista in cui cercare
    template <typename T>
    std::shared_ptr<T> cerca_elemento(const vector<std::shared_ptr<T>>& lista, const std::string& codice) const;
    
    // Template di stampa di tutti gli elementi di una certa lista
    template <typename T>
    bool stampa_elementi(const vector<std::shared_ptr<T>>& lista) const;

    // Template che calcola il massimo valore in una mappa con valori interi in .second
    template <typename T>
    std::optional<std::string> calcola_massimo_mappa(const std::unordered_map<std::string, T> counter) const;
    
    // === COSTRUTTORE E DISTRUTTORE ===
    Gestore_azienda(std::vector<std::shared_ptr<Pacchetto_viaggio>> catalogo, std::vector<std::shared_ptr<Cliente>> clienti,
                      std::vector<std::shared_ptr<Prenotazione>> prenotazioni);
    ~Gestore_azienda(); 

    // === GESTIONE CATALOGO PACCHETTI ===

    // Aggiunta pacchetto
    bool aggiungi_pacchetto();

    // Metodo che trova un pacchetto in funzione del codice univoco
    std::shared_ptr<Pacchetto_viaggio> cerca_pacchetto(std::string codice) const;

    // Metodi di stampa pacchetti
    bool visualizza_catalogo() const;
    bool visualizza_pacchetti_per_tipologia(std::string tipo) const; 
    bool visualizza_pacchetti_disponibili() const;

    // === GESTIONE CLIENTI ===
    bool aggiungi_cliente();
    std::shared_ptr<Cliente> cerca_cliente(std::string codice) const;
    bool visualizza_clienti() const;
    bool visualizza_clienti_per_tipologia(std::string tipo) const;
    
    // === GESTIONE PRENOTAZIONI ===
    bool aggiungi_prenotazione(); 
    bool conferma_prenotazione(std::string codice_prenotazione);
    bool visualizza_prenotazioni() const;
    bool visualizza_prenotazioni_cliente(std::string codice_cliente) const;
    bool visualizza_prenotazioni_confermate() const;

    // === STATISTICHE ===

    // Sincronizzazione statistiche dopo il caricamento di un database
    bool sincronizza_statistiche();
    
    // Stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Top Pacchetto, Top Destinazione
    bool statistiche_generali() const;
    
    // Stampa quante prenotazioni ci sono per Mare, Montagna, ecc.
    bool statistiche_per_tipologia() const;

    // Ritorna il cliente che ha speso di più
    std::shared_ptr<Cliente> cliente_migliore() const;

    // === GESTIONE FILE ===
    
    // Salva/Carica lo stato intero dell'azienda (CSV simulato)
    bool salva_dati_su_file(const std::string& nomefile, std::string& tipo);
    bool carica_dati_da_file(const std::string& nomefile, const string& tipo);
};

#include "Gestore_azienda.tpp"

#endif