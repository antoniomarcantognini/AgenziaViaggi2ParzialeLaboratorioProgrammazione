#pragma once
#include "Pacchetto_viaggio.h"
#include "Categoria_pensione.h"
#include <string>

class Pacchetto_mare : public Pacchetto_viaggio{
    
private:
    int ombrellone_incluso;
    bool attrezzatura_sportiva;
    Categoria_pensione tipologia;

public:
    // costruttore
    Pacchetto_mare(std::string codice, std::string destinazione, int giorni, double prezzo,
                   int ombrellone, bool attrezzatura, Categoria_pensione tipo);

    // Override dei metodi virtuali puri
    double calcola_prezzo_finale() const override;
    std::string stampa_dettagli() const override;
    std::string get_tipologia() const override;

    // Getter specifici
    int get_ombrellone_incluso() const;
    bool has_attrezzatura_sportiva() const;
    Categoria_pensione get_categoria_pensione() const;

    // metodi statici di supporto
    static std::string pensione_to_string(Categoria_pensione tipo);
    static Categoria_pensione string_to_pensione(std::string tipo);
};
