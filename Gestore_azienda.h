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

    // Contatori per generare codici univoci
    int prossimoCodicePacchetto;      // Es: 1 -> "PCK-0001"
    int prossimoCodiceCliente;        // Es: 1 -> "CLT-0001"
    int prossimoCodicePrenotazione;   // Es: 1 -> "BKG-0001"

    // METODI PRIVATI DI UTILITY
    // Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0005")
    std::string generaCodiceUnico(char tipo);

public:
    // === COSTRUTTORE E DISTRUTTORE ===
    Gestore_agenzia();
    ~Gestore_agenzia(); 

    // === GESTIONE CATALOGO PACCHETTI ===
    void aggiungiPacchettoManuale();
    std::shared_ptr<Pacchetto_viaggio> cercaPacchetto(std::string codice);
    void visualizzaCatalogo() const;
    void visualizzaPacchettiPerTipologia(std::string tipo) const; 
    void visualizzaPacchettiDisponibili() const;

    // === GESTIONE CLIENTI ===
    void aggiungiCliente(); // Wizard interattivo
    std::shared_ptr<Cliente> cercaCliente(std::string codice);
    void visualizzaClienti() const;
    void visualizzaClientiPerTipologia(std::string tipo) const;
    // === GESTIONE PRENOTAZIONI ===
    void creaPrenotazione(); 
    void confermaPrenotazione(std::string codicePrenotazione);
    void visualizzaPrenotazioni() const;
    void visualizzaPrenotazioniCliente(std::string codiceCliente) const;
    void visualizzaPrenotazioniConfermate() const;

    // === STATISTICHE ===
    void statisticheGenerali() const;
    // Stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Top Pacchetto, Top Destinazione
    
    void statistichePerTipologia() const;
    // Stampa quante prenotazioni ci sono per Mare, Montagna, ecc.

    std::shared_ptr<Cliente> clienteMigliore() const;
    // Ritorna il cliente che ha speso di più

    // === GESTIONE FILE ===
    // Salva/Carica lo stato intero dell'agenzia (CSV simulato)
    void salvaDatiSuFile(const std::string& nomefile) const;
    void caricaDatiDaFile(const std::string& nomefile);
};
