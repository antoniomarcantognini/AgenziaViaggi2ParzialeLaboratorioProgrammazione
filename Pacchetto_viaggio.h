
#pragma once

#include <iostream>
#include <string>

class Pacchetto_viaggio {

protected:
    std::string codice_pacchetto;
    std::string destinazione;
    int durata_giorni;
    double prezzo_base;
    bool disponibile;
    // Costruttore
    Pacchetto_viaggio(std::string codice, std::string dest, int giorni, double prezzo);
    
public:
    // Distruttore virtuale
    virtual ~Pacchetto_viaggio();
    
    // Metodi virtuali puri (da implementare nelle classi derivate)
    virtual double calcola_prezzo_finale() const = 0;
    virtual std::string stampa_dettagli() const = 0;
    virtual std::string get_tipologia() const = 0;

    // Metodi getter/setter
    std::string get_codice_pacchetto() const;
    std::string get_destinazione() const;
    int get_durata_giorni() const;
    double get_prezzo_base() const;
    bool is_disponibile() const;
    void set_disponibile(bool disponibilita);
};
