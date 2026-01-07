#pragma once
#include "Pacchetto_viaggio.h"
#include "Categoria_pensione.h"
#include <string>
#include <memory>
#include <fstream>

class Pacchetto_mare : public Pacchetto_viaggio{
    
private:
    bool ombrellone_incluso;
    bool attrezzatura_sportiva;
    Categoria_pensione tipologia;
    bool valida_dati() const;

    // Costruttore privato
    Pacchetto_mare(std::string codice, std::string dest, int giorni, double prezzo,
                   bool ombrellone, bool attrezzatura, Categoria_pensione tipo);
public:
   // Factory Method statico
    static std::shared_ptr<Pacchetto_mare> crea_pacchetto(std::string codice, std::string dest, int giorni, double prezzo,
                                                          bool ombrellone, bool attrezzatura, Categoria_pensione tipo);
    // Override dei metodi virtuali puri
    double calcola_prezzo_finale() const override;
    std::string stampa_dettagli() const override;
    std::string get_tipologia() const override;

    // Getter specifici
    bool has_ombrellone_incluso() const;
    bool has_attrezzatura_sportiva() const;
    Categoria_pensione get_categoria_pensione() const;

    // Override del metodo di salvataggio su file
    bool salva_dati_su_file(std::ofstream& file) const override;
};