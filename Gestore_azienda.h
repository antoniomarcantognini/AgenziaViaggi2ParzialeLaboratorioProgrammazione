#pragma once

#ifndef GESTORE_AZIENDA_H
#define GESTORE_AZIENDA_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Pacchetto_viaggio.h"
#include "Cliente.h"
#include "Prenotazione.h"

class Gestore_agenzia {

private:
    
    std::vector<std::shared_ptr<Pacchetto_viaggio>> catalogo;
    std::vector<std::shared_ptr<Cliente>> clienti;
    std::vector<std::shared_ptr<Prenotazione>> prenotazioni;

    const map<string, function<shared_ptr<Pacchetto_viaggio>(string& codice, string& destinazione, int& giorni, bool& disponibile, double& prezzo, const vector<string>& campi, int& numero_linea)>> mappa_caricamento_specifico;

    // I contatori per generare codici univoci sono statici e definiti nelle rispettive classi

    // METODI PRIVATI DI UTILITY
    // Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0005")
    std::string generaCodiceUnico(char tipo);

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
    std::shared_ptr<T> cerca_elemento(const vector<std::shared_ptr<T>>& lista, const std::string& codice)
    
    // Template di stampa di tutti gli elementi di una certa lista
    template <typename T>
    bool stampa_elementi(const vector<std::shared_ptr<T>>& lista)

    // Template che calcola il massimo valore in una mappa con valori interi in .second
    template <typename T>
    std::string calcola_massimo_mappa(const std::unordered_map<std::string, T> counter)
    
    // Template che trasfroma variabili enum in stringhe
    template <typename T>
    std::string etos(T categoria);

    // Template che trasforma stringhe in variabili enum (stampa anche il numero di riga in cui è presente l'errore - per caricamento di file)
    template <typename T>
    auto stoe(string stringa, int numero_riga);
    
    // Template che trasforma stringhe in variabili enum
    template <typename T>
    auto stoe(string stringa);

    // Template che crea un elemento della tipologia T
    template <typename T, typename... Args>
    auto crea_elemento(Args...& args);
    
    // === COSTRUTTORE E DISTRUTTORE ===
    Gestore_agenzia(std::vector<std::shared_ptr<Pacchetto_viaggio>> catalogo, std::vector<std::shared_ptr<Cliente>> clienti,
                      std::vector<std::shared_ptr<Prenotazione>> prenotazioni);
    ~Gestore_agenzia(); 

    // === GESTIONE CATALOGO PACCHETTI ===

    // Aggiunta pacchetto
    bool aggiungiPacchetto();

    // Metodo che trova un pacchetto in funzione del codice univoco
    std::shared_ptr<Pacchetto_viaggio> cercaPacchetto(std::string codice);

    // Metodi di stampa pacchetti
    bool visualizzaCatalogo() const;
    bool visualizzaPacchettiPerTipologia(std::string tipo) const; 
    bool visualizzaPacchettiDisponibili() const;

    // === GESTIONE CLIENTI ===
    bool aggiungiCliente();
    std::shared_ptr<Cliente> cercaCliente(std::string codice);
    bool visualizzaClienti() const;
    bool visualizzaClientiPerTipologia(std::string tipo) const;
    
    // === GESTIONE PRENOTAZIONI ===
    bool aggiungiPrenotazione(); 
    bool confermaPrenotazione(std::string codice_prenotazione);
    bool visualizzaPrenotazioni() const;
    bool visualizzaPrenotazioniCliente(std::string codice_cliente) const;
    bool visualizzaPrenotazioniConfermate() const;

    // === STATISTICHE ===

    // Stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Top Pacchetto, Top Destinazione
    bool statisticheGenerali() const;
    
    // Stampa quante prenotazioni ci sono per Mare, Montagna, ecc.
    bool statistichePerTipologia() const;

    // Ritorna il cliente che ha speso di più
    std::shared_ptr<Cliente> clienteMigliore() const;

    // === GESTIONE FILE ===
    
    // Salva/Carica lo stato intero dell'agenzia (CSV simulato)
    bool salvaDatiSuFile(const std::string& nomefile, const string& tipo) const;
    bool caricaDatiDaFile(const std::string& nomefile, const string& tipo);
};

#include "Gestore_azienda.tpp"

#endif
