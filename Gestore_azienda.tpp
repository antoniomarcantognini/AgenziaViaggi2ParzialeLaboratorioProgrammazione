#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Pacchetto_viaggio.h"
#include "Cliente.h"
#include "Prenotazione.h"

// TEMPLATE PER AGGIUNTA DI UN ELEMENTO AL VETTORE DI PUNTATORI

template <typename T>
std::shared_ptr<T> aggiungi_elemento(vector<std::shared_ptr<T>>& lista, std::shared_ptr<T> elemento) {
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
std::shared_ptr<T> cerca_elemento(const vector<std::shared_ptr<T>>& lista, const std::string& codice) {
    for (const auto& elemento : lista) {
        if (elemento->get_codice() == codice) {
            return elemento;
        }
    }
    return nullptr; // Elemento non trovato
}

// TEMPLATE PER LA STAMPA DI ELEMENTI IN UN VETTORE DI PUNTATORI
template <typename T>
bool stampa_elementi(const vector<std::shared_ptr<T>>& lista) {
    for (const auto& elemento : lista) {
        cout << elemento->stampa_dettagli() << endl;
    }
    return true;
}

// TEMPLATE PER IL CALCOLO DELLA CATEGORIA PIÙ RICHIESTA
template <typename T>
std::string calcola_massimo_mappa(const std::unordered_map<std::string, T> counter) {
    if (counter.empty()) {
            return "Nessuna prenotazione effettuata";
    }
    return max_element(
        
        // Posizioni di inizio e fine della mappa
        counter.begin(), 
        counter.end(),

        // Lambda per confrontare i valori delle mappe: considero "massimo" quello con il conteggio più alto
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        }

    )->first;
}

template <typename T>
string etos(T campo) {
    string s = enum_name(campo);
    return replace( s.begin(), s.end(), '_' , ' ');
}

template <typename T>
auto stoe(string stringa, int linea) {
    auto tipologia = enum_cast<T>(replace( s.begin(), s.end(), ' ', '_'));
    if (!tipologia.has_value()) {
        cerr < "Errore! La tipologia non è stata inserita correttamente in linea " << linea << "!" << endl;
        return nullopt;
    }
}

template <typename T>
auto stoe(string stringa) {
    auto tipologia = enum_cast<T>(replace( s.begin(), s.end(), ' ', '_'));
    if (!tipologia.has_value()) {
        cerr < "Errore! La tipologia non è stata inserita correttamente!" << endl;
        return nullopt;
    }
}

template <typename T, typename... Args>
auto crea_elemento(Args...& args) { {
    // Creazione del puntatore al nuovo pacchetto mare
    shared_ptr<T> nuovo_elemento = make_shared<T>(Args... args);

    // Aggiunta al catalogo
    if (aggiungi_elemento(this->catalogo, nuovo_elemento) == nullptr) {
        cerr << "Errore nella creazione. Operazione annullata" << endl;
        return false; // aggiungi_elemento da errore solo se nuovo_pacchetto è nullo, quindi non è necessario controllare nuovo_pacchetto. 
    }
    return true;
};
}