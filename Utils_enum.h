#pragma once
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>

// Struttura base per la mappa
template <typename T>
struct EnumMappa {
    static const std::map<T, std::string>& get_map();
};

class Utils_enum {
public:
    // Converte Enum -> String
    template <typename T>
    static std::string to_string(T valore) {
        // Recupera la mappa specifica per questo tipo di Enum
        const auto& mappa = EnumMappa<T>::get_map();
        auto it = mappa.find(valore);
        if (it != mappa.end()) {
            return it->second;
        }
        return "Sconosciuto";
    }

    // Converte String -> Enum
    template <typename T>
    static T from_string(std::string str) {
        const auto& mappa = EnumMappa<T>::get_map();
        
        // Cerca la stringa nella mappa
        for (const auto& coppia : mappa) {
            if (coppia.second == str) {
                return coppia.first;
            }
        }
        // Se non trova nulla, ritorna UNKNOWN
    }
};
