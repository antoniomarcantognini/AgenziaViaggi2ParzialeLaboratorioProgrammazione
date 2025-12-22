#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "magic_enum.hpp"

class Utils_enum {
public:
    // Converte Enum -> String
    // Prende il nome esatto e cambia '_' in ' '
    template <typename T>
    static std::string to_string(T valore) {
        // 1. Ottieni il nome grezzo dalla variabile
        std::string str = std::string(magic_enum::enum_name(valore));
        
        // 2. Sostituisci '_' con spazio
        std::replace(str.begin(), str.end(), '_', ' ');
        
        return str;
    }

    // Converte String -> Enum
    // Prende l'input, cambia ' ' in '_' e cerca l'enum esatto
    template <typename T>
    static T from_string(std::string str) {
        // 1. Sostituisci spazi con underscore per tornare al formato codice
        std::replace(str.begin(), str.end(), ' ', '_');

        // 2. Prova la conversione esatta
        auto valore_optional = magic_enum::enum_cast<T>(str);

        if (valore_optional.has_value()) {
            return valore_optional.value();
        } else {
            // Gestione errore
            throw std::invalid_argument("Valore non valido per l'enum: " + str);
        }
    }
};
