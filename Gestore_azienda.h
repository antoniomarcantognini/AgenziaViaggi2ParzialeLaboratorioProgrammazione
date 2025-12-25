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

    // I contatori per generare codici univoci sono statici e definiti nelle rispettive classi

    // METODI PRIVATI DI UTILITY
    // Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0005")
    std::string generaCodiceUnico(char tipo);

    bool Gestore_azienda::valida_inserimento_S_N(std::string stringa_S_N&, bool flag_da_aggiornare&);

    bool salvataggio_clienti(std::ofstream& file) const;
    bool carica_clienti(std::ifstream& file); 

    bool salvataggio_pacchetti(std::ofstream& file) const;
    bool carica_pacchetti(std::ifstream& file);
    
    bool salvataggio_prenotazioni(std::ofstream& file) const;
    bool carica_prenotazioni(std::ifstream& file);
    
public:
    // === TEMPLATES ===
    template <typename T>
    std::shared_ptr<T> aggiungi_elemento(vector<std::shared_ptr<T>>& lista, std::shared_ptr<T> elemento);
    template <typename T>
    std::shared_ptr<T> cerca_elemento(const vector<std::shared_ptr<T>>& lista, const std::string& codice)
    template <typename T>
    bool stampa_elementi(const vector<std::shared_ptr<T>>& lista)
    template <typename T>
    std::string calcola_massimo_mappa(const std::unordered_map<std::string, T> counter)
    template <typename T>
    std::string etos(T categoria);
    template <typename T>
    auto stoe(string stringa, int numero_riga);
    template <typename T>
    auto stoe(string stringa);
    

    // === COSTRUTTORE E DISTRUTTORE ===
    Gestore_agenzia(std::vector<std::shared_ptr<Pacchetto_viaggio>> catalogo, std::vector<std::shared_ptr<Cliente>> clienti,
                     std::vector<std::shared_ptr<Prenotazione>> prenotazioni);
    ~Gestore_agenzia(); 

    // === GESTIONE CATALOGO PACCHETTI ===

    // Aggiunta pacchetto avventura
    bool aggiungiPacchettoAvventura();
    
    // Aggiunta pacchetto mare
    bool aggiungiPacchettoMare();
    
    // Aggiunta pacchetto montagna
    bool aggiungiPacchettoMontagna();
    
    // Aggiunta pacchetto città
    bool aggiungiPacchettoCitta();

    std::shared_ptr<Pacchetto_viaggio> cercaPacchetto(std::string codice);
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
    bool confermaPrenotazione(std::string codicePrenotazione);
    bool visualizzaPrenotazioni() const;
    bool visualizzaPrenotazioniCliente(std::string codiceCliente) const;
    bool visualizzaPrenotazioniConfermate() const;

    // === STATISTICHE ===
    bool statisticheGenerali() const;
    // Stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Top Pacchetto, Top Destinazione
    
    bool statistichePerTipologia() const;
    // Stampa quante prenotazioni ci sono per Mare, Montagna, ecc.

    std::shared_ptr<Cliente> clienteMigliore() const;
    // Ritorna il cliente che ha speso di più

    // === GESTIONE FILE ===
    // Salva/Carica lo stato intero dell'agenzia (CSV simulato)
    bool salvaDatiSuFile(const std::string& nomefile) const;
    bool caricaDatiDaFile(const std::string& nomefile);
};

#include "Gestore_azienda.tpp"

#endif 