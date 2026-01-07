#pragma once
#include <iostream>
#include <string>
#include <fstream>

class Pacchetto_viaggio {

protected:
    std::string codice_pacchetto;
    std::string destinazione;
    int durata_giorni;
    double prezzo_base;
    bool disponibile;
    
    // VARIABILE STATICA PRIVATA
    static int pacchetti_creati;

    // Costruttore protetto
    Pacchetto_viaggio(std::string codice, std::string dest, int giorni, double prezzo);
    
public:
    virtual ~Pacchetto_viaggio();
    
    // GETTER STATICO PUBBLICO
    static int get_pacchetti_creati();

    // Metodi virtuali puri
    virtual double calcola_prezzo_finale() const = 0;
    virtual std::string stampa_dettagli() const = 0;
    virtual std::string get_tipologia() const = 0;

    // Getter
    std::string get_codice() const;
    std::string get_destinazione() const;
    int get_durata_giorni() const;
    double get_prezzo_base() const;
    bool is_disponibile() const;
    void set_disponibile(bool disponibilita);
    static bool set_pacchetti_creati(int n);

    virtual bool salva_dati_su_file(std::ofstream& file) const;
};