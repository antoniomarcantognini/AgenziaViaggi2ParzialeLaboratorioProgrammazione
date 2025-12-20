#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "Tipologia_cliente.h"

class Cliente {
private:
    std::string codice_cliente; // Formato: "CLT-XXXX"
    std::string nome;
    std::string cognome;
    std::string email;
    std::string telefono;
    int eta;
    Tipologia_cliente tipologia;

    // Costruttore privato: può essere chiamato solo dal metodo statico crea_cliente
    Cliente(std::string codice, std::string nome, std::string cognome, 
            std::string email, std::string tel, int eta, Tipologia_cliente tipo);

public:
    static std::shared_ptr<Cliente> crea_cliente(std::string codice, std::string nome, std::string cognome, 
                                                 std::string email, std::string tel, int eta, Tipologia_cliente tipo);

    // Getter
    std::string get_codice_cliente() const;
    std::string get_nome_completo() const;  // Ritorna "Nome Cognome"
    int get_eta() const;
    Tipologia_cliente get_tipologia_cliente() const;

    // Logica di business
    double applica_sconto(double prezzo_base) const; // Standard: 0%, Premium: 10%, VIP 20%
    std::string stampa_info() const;

    // Helpers statici
    static std::string tipologia_to_string(Tipologia_cliente tipo);
    static Tipologia_cliente string_to_tipologia(std::string tipo);
};
