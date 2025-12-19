#pragma once
#include "Pacchetto_viaggio.h"
#include "Tipologia_pensione.h"
#include <string>

class Pacchetto_mare : public Pacchetto_viaggio{
    
private:
    int escursioni_barca;
    bool vista_mare;
    Tipologia_pensione pensione;

public:
    // costruttore
    Pacchetto_mare(std::string codice, std::string destinazione, int giorni, double prezzo,
                   int num_escursioni, bool vista, Tipologia_pensione tipo_pensione);

    // Override dei metodi virtuali puri
    double calcola_prezzo_finale() const override;
    std::string stampa_dettagli() const override;
    std::string get_tipologia() const override;

    // Getter specifici
    int get_escursioni_barca() const;
    bool has_vista_mare() const;
    Tipologia_pensione get_tipologia_pensione() const;

    // metodi statici di supporto
    static std::string pensione_to_string(Tipologia_pensione tipo);
    static Tipologia_pensione string_to_pensione(std::string tipo);
};
