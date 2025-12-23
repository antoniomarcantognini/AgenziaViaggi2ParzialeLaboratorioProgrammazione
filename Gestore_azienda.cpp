#include "magic_enum.hpp"
#include "Gestore_azienda.h"
using namespace std;
using namespace magic_enum;
    
// METODI PRIVATI DI UTILITY
// Genera codici progressivi formattati (es. generaCodice('C') -> "CLT-0005")
string Gestore_azienda::generaCodiceUnico(char tipo);

// === COSTRUTTORE E DISTRUTTORE ===
Gestore_azienda::Gestore_agenzia();
Gestore_azienda::~Gestore_agenzia(); 
// === GESTIONE CATALOGO PACCHETTI ===
void Gestore_azienda::aggiungiPacchettoManuale();
shared_ptr<Pacchetto_viaggio> Gestione_azienda::cercaPacchetto(string codice);
void Gestore_azienda::visualizzaCatalogo() const;
void Gestore_azienda::visualizzaPacchettiPerTipologia(string tipo) const; 
void Gestore_azienda::visualizzaPacchettiDisponibili() const;

// === GESTIONE CLIENTI ===
void Gestore_azienda::aggiungiCliente(); // Wizard interattivo
shared_ptr<Cliente> Gestore_azienda::cercaCliente(string codice);
void Gestore_azienda::visualizzaClienti() const;
void Gestore_azienda::visualizzaClientiPerTipologia(string tipo) const;
// === GESTIONE PRENOTAZIONI ===
void Gestore_azienda::creaPrenotazione(); 
void Gestore_azienda::confermaPrenotazione(string codicePrenotazione);
void Gestore_azienda::visualizzaPrenotazioni() const;
void Gestore_azienda::visualizzaPrenotazioniCliente(string codiceCliente) const;
void Gestore_azienda::visualizzaPrenotazioniConfermate() const;

// === STATISTICHE ===
void Gestore_azienda::statisticheGenerali() const;
// Stampa: Totale pacchetti, clienti, prenotazioni, Fatturato, Top Pacchetto, Top Destinazione
    
void Gestore_azienda::statistichePerTipologia() const;
// Stampa quante prenotazioni ci sono per Mare, Montagna, ecc.

shared_ptr<Cliente> Gestore_azienda::clienteMigliore() const;
// Ritorna il cliente che ha speso di più

// === GESTIONE FILE ===
// Salva/Carica lo stato intero dell'agenzia (CSV simulato)
void Gestore_azienda::salvaDatiSuFile(const std::string& nomefile) const;
void Gestore_azienda::caricaDatiDaFile(const std::string& nomefile);
