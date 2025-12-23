#pragma once

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

    bool salvataggio_clienti(std::ofstream& file) const;
    bool carica_clienti(std::ifstream& file); 

    bool salvataggio_pacchetti(std::ofstream& file) const;
    bool carica_pacchetti(std::ifstream& file);
    
    bool salvataggio_prenotazioni(std::ofstream& file) const;
    bool carica_prenotazioni(std::ifstream& file);
    
public:
    // === COSTRUTTORE E DISTRUTTORE ===
    Gestore_agenzia();
    ~Gestore_agenzia(); 

    // === GESTIONE CATALOGO PACCHETTI ===

    // Aggiunta pacchetto avventura
    bool aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    const vector<string>& lista_attivita, 
                    Categoria_adrenalina categoria, bool assicurazione);
    
    // Aggiunta pacchetto mare
    bool aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    bool ombrellone, bool attrezzatura, Categoria_pensione tipo);
    
    // Aggiunta pacchetto montagna
    bool aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    bool skipass_incluso, int num_escursioni, Categoria_difficolta difficolta);
    
    // Aggiunta pacchetto città
    bool aggiungiPacchetto(string codice, string dest, int giorni, double prezzo,
                    int num_musei, bool guida, Categoria_hotel categoria);

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
