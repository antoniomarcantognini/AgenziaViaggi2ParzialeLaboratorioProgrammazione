#pragma once
#include "Pacchetto_viaggio.h"
#include "Categoria_adrenalina.h"
#include <string>
#include <vector>

class Pacchetto_avventura : public Pacchetto_viaggio {

private:
    std::vector<std::string> attivita;
    bool assicurazione_extra;
    Categoria_adrenalina categoria_adrenalina;

public:
    // Costruttore
    Pacchetto_avventura(std::string codice, std::string dest, int giorni, double prezzo,
                        const std::vector<std::string>& lista_attivita, 
                        Categoria_adrenalina categoria, bool assicurazione);

    // Override dei metodi virtuali puri
    double calcola_prezzo_finale() const override;
    std::string stampa_dettagli() const override;
    std::string get_tipologia() const override;

    // Getter specifici
    std::vector<std::string> get_attivita() const;
    bool has_assicurazione_extra() const;
    Categoria_adrenalina get_categoria_adrenalina() const;

    // Metodi statici di supporto
    static std::string categoria_to_string(Categoria_adrenalina cat);
    static Categoria_adrenalina string_to_categoria(std::string cat);
};
