#pragma once
#include "Pacchetto_viaggio.h"
#include "Categoria_hotel.h"
#include <string>
#include <memory>
#include <fstream>

class Pacchetto_citta : public Pacchetto_viaggio{

private:
    int numero_musei;
    bool guida_turistica;
    Categoria_hotel categoria_hotel;
    // Metodo helper per la validazione interna
    bool valida_dati() const;

    // Costruttore privato
    Pacchetto_citta(std::string codice, std::string destinazione, int giorni, double prezzo,
                    int num_musei, bool guida, Categoria_hotel hotel);

public:
    // Factory Method STATICO
    static std::shared_ptr<Pacchetto_citta> crea_pacchetto(std::string codice, std::string destinazione, int giorni, double prezzo,
                                                           int num_musei, bool guida, Categoria_hotel hotel);
    // Override dei metodi virtuali puri
    double calcola_prezzo_finale() const override;
    std::string stampa_dettagli() const override;
    std::string get_tipologia() const override; 

    // Getter specifici
    int get_numero_musei() const;
    bool has_guida_turistica() const;
    Categoria_hotel get_categoria_hotel() const;

    // metodi statici di supporto (per convertire enum in stringa)
    static std::string categoria_to_string(Categoria_hotel cat);
    static Categoria_hotel string_to_categoria(std::string cat);

    // Override del metodo di salvataggio su file
    bool salva_dati_su_file(std::ofstream& file) const override;
};
