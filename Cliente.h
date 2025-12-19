#pragma once

#include <iostream>

class Cliente {

private: 
    std::string codice_cliente;
    std::string nome;
    std::string cognome;
    std::string email;
    std::string telefono;
    int eta;
    std::string tipologia_cliente; // Standard, Premium o VIP

public:
    Cliente(std::string codice,std::string nome, std::string cognome, std::string email, std::string tel, int eta, std::string tipo);

    // Getter
    std::string get_codice_cliente() const;
    std::string get_nome_completo() const;
    int get_eta() const;
    std::string get_tipologia_cliente() const;
    
    double applica_sconto(double prezzo_base) const;
    // Standard: 0%, Premium: 10%, VIP 20%

    std::string stampa_info() const;

};