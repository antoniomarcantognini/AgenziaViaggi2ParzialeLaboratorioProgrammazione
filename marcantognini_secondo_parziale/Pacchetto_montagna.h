#pragma once
#include "Pacchetto_viaggio.h"
#include "Categoria_difficolta.h"
#include <string>
#include <memory>
#include <fstream>

class Pacchetto_montagna : public Pacchetto_viaggio{

private:
    bool skipass_incluso;
    int numero_escursioni;
    Categoria_difficolta difficolta;
    bool valida_dati() const;

    // Costruttore privato
    Pacchetto_montagna(std::string codice, std::string dest, int giorni, double prezzo,
                       bool skipass, int num_escursioni, Categoria_difficolta diff);
public:
    // Factory Method statico
    static std::shared_ptr<Pacchetto_montagna> crea_pacchetto(std::string codice, std::string dest, int giorni, double prezzo,
                                                              bool skipass, int num_escursioni, Categoria_difficolta diff);

    // Override dei metodi virtuali puri
    double calcola_prezzo_finale() const override;
    std::string stampa_dettagli() const override;
    std::string get_tipologia() const override;

    // Getter specifici
    bool has_skipass_incluso() const;
    int get_numero_escursioni() const;
    Categoria_difficolta get_difficolta() const;

    // Override del metodo di salvataggio su file e caricamento da file 
    bool salva_dati_su_file(std::ofstream& file) const override;
};