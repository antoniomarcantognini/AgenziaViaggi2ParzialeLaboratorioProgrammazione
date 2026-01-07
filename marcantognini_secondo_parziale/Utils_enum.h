#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <optional>     // Necessario per std::optional
#include "magic_enum.hpp"

class Utils_enum {
public:
    // METODO: ENUM TO STRING (etos)
    template <typename T>
    static std::string etos(T campo) {
        std::string s = std::string(magic_enum::enum_name(campo));
        std::replace(s.begin(), s.end(), '_' , ' ');
        return s;
    }

    // METODO: STRING TO ENUM (stoe) - Con numero riga per errori
    template <typename T>
    static std::optional<T> stoe(std::string stringa, int linea) {
        // Pulisce la stringa (spazi -> underscore)
        std::replace(stringa.begin(), stringa.end(), ' ', '_'); 
        
        // Tenta la conversione
        auto tipologia = magic_enum::enum_cast<T>(stringa, magic_enum::case_insensitive);
        
        // Se fallisce, stampa errore e ritorna nullopt
        if (!tipologia.has_value()) {
            std::cerr << "Errore riga " << linea << ": Valore '" << stringa << "' non valido." << std::endl;
            return std::nullopt;
        }
        return tipologia;
    }

    // METODO: STRING TO ENUM (stoe) - Senza numero riga (Overload)
    template <typename T>
    static std::optional<T> stoe(std::string stringa) {
        std::replace(stringa.begin(), stringa.end(), ' ', '_');
        
        auto tipologia = magic_enum::enum_cast<T>(stringa, magic_enum::case_insensitive);
        
        if (!tipologia.has_value()) {
            std::cerr << "Errore: Valore '" << stringa << "' non valido." << std::endl;
            return std::nullopt;
        }
        return tipologia;
    }
};