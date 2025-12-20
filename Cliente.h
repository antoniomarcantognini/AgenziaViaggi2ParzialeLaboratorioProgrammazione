#pragma once
#include "Tipologia_cliente.h"
#include <iostream>
#include <string>

class Cliente {

private: 
    std::string codice_cliente; // Formato: "CLT-XXXX"
    std::string nome;
    std::string cognome;
    std::string email;
    std::string telefono;
    int eta;
    Tipologia_cliente tipologia_cliente; // Standard, Premium o VIP

public:
    Cliente(std::string codice,std::string nome, std::string cognome, std::string email, std::string tel, int eta, Tipologia_cliente tipo);

    // Getter
    std::string get_codice_cliente() const;
    std::string get_nome_completo() const; // Ritorna "Nome Cognome"
    int get_eta() const;
    Tipologia_cliente get_tipologia_cliente() const;
    
    double applica_sconto(double prezzo_base) const;
    // Standard: 0%, Premium: 10%, VIP 20%

    std::string stampa_info() const;

    // Metodi statici di supporto per enum
    static std::string tipologia_to_string(Tipologia_cliente tipo);
    static Tipologia_cliente string_to_tipologia(std::string tipo);
};
