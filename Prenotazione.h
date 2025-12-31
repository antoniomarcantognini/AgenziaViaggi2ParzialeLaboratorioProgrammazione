#pragma once

#ifndef PRENOTAZIONE_H
#define PRENOTAZIONE_H

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Cliente.h"
#include "Pacchetto_viaggio.h"

class Prenotazione{

private:
    std::string codice_prenotazione; // Formato: "BKG-XXXX" (generato automaticamente)
    std::shared_ptr<Cliente> cliente;
    std::shared_ptr<Pacchetto_viaggio> pacchetto_viaggio;
    int numero_persone;
    std::string data_prenotazione;
    double prezzo_totale;
    bool confermata;
    
    // VARIABILI STATICHE PRIVATE
    static int prenotazioni_create;
    static double fatturato_totale;
    static std::unordered_map<std::string, int> destinazioni_counter; // Mappa per contare le prenotazioni per destinazione
    static std::unordered_map<std::string, int> pacchetti_counter;   // Mappa per contare le prenotazioni per pacchetto
    static std::unordered_map<std::string, double> spesa_clienti;      // Mappa per contare la spesa totale per cliente

    // Metodo privato di validazione
    bool valida_input_prenotazione() const;

    // Costruttore privato
    Prenotazione(std::string codice, std::shared_ptr<Cliente> cliente, std::shared_ptr<Pacchetto_viaggio> pacchetto_viaggio, int num_persone, std::string data);

public:
    // GETTER STATICHE PUBBLICHE (Per accedere ai dati privati)
    static int get_prenotazioni_create();
    static double get_fatturato_totale();
    static const std::unordered_map<std::string, int>& get_destinazioni_counter();
    static const std::unordered_map<std::string, int>& get_pacchetti_counter();
    static const std::unordered_map<std::string, double>& get_spesa_clienti();
    // Metodo statico per creare una prenotazione (shared_ptr per permettere la condivisione tra più prenotazioni dello stesso cliente o pacchetto)
    static std::shared_ptr<Prenotazione> crea_prenotazione(std::string codice, std::shared_ptr<Cliente> cliente, std::shared_ptr<Pacchetto_viaggio> pacchetto_viaggio, int num_persone, std::string data);
    
    // Getter
    std::string get_codice() const;
    std::shared_ptr<Cliente> get_cliente() const;
    std::shared_ptr<Pacchetto_viaggio> get_pacchetto() const;
    int get_numero_persone() const;
    std::string get_data_prenotazione() const;
    double get_prezzo_totale();
    bool is_confermata() const; 

    // Calcola prezzo totale: 
    // (pacchetto->calcola_prezzo_finale() * num_persone)
    // con sconto del cliente applicato
    bool calcola_prezzo_totale();

    bool conferma_prenotazione();
    bool stampa_dettagli(int indice_produzione) const;

    // Metodo di salvataggio dati su un file
    bool salva_dati_su_file(std::ofstream& file) const;

    // Distruttore
    ~Prenotazione();
};

#endif
