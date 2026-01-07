#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Pacchetto_viaggio.h"
#include <algorithm>
#include <optional>
#include <unordered_map>
#include "Cliente.h"
#include "Prenotazione.h"
#include "magic_enum.hpp"

using namespace std;

// TEMPLATE PER AGGIUNTA DI UN ELEMENTO AL VETTORE DI PUNTATORI

template <typename T>
std::shared_ptr<T> Gestore_azienda::aggiungi_elemento(vector<std::shared_ptr<T>>& lista, std::shared_ptr<T> elemento) {
    if (elemento != nullptr) {
        lista.push_back(elemento);
        return elemento;
    } else {
        cerr << "Errore! Impossibile aggiungere elemento nullo alla lista." << endl;
        return nullptr;
    }
}

// TEMPLATE PER RICERCA DI UN ELEMENTO NEL VETTORE DI PUNTATORI
template <typename T>
std::shared_ptr<T> Gestore_azienda::cerca_elemento(const vector<std::shared_ptr<T>>& lista, const std::string& codice) const{
    for (const auto& elemento : lista) {
        if (elemento->get_codice() == codice) {
            return elemento;
        }
    }
    return nullptr; // Elemento non trovato
}

// TEMPLATE PER LA STAMPA DI ELEMENTI IN UN VETTORE DI PUNTATORI
template <typename T>
bool Gestore_azienda::stampa_elementi(const vector<std::shared_ptr<T>>& lista) const {
    for (const auto& elemento : lista) {
        cout << elemento->stampa_dettagli() << endl;
    }
    return true;
}

// TEMPLATE PER IL CALCOLO DELLA CATEGORIA PIÙ RICHIESTA
template <typename T>
optional<string> Gestore_azienda::calcola_massimo_mappa(const std::unordered_map<std::string, T> counter) const {
    if (counter.empty()) {
        return nullopt;
    }
    
    auto max_it = max_element(
        // Posizioni di inizio e fine della mappa
        counter.begin(), 
        counter.end(),
        // Lambda per confrontare i valori delle mappe: considero "massimo" quello con il conteggio più alto
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }
    );

    return max_it->first;
}
