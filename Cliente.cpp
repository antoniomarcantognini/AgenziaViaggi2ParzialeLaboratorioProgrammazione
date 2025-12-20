#include "Cliente.h"
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <algorithm>

using namespace std;

// Implementazione del Factory Method con validazioni tramite Lambda
shared_ptr<Cliente> Cliente::crea_cliente(string codice, string nome, string cognome, 
                                          string email, string tel, int eta, Tipologia_cliente tipo) {
    
    // Definizione Lambda functions per validazione
    // 1. Validazione Stringhe non vuote
    auto check_not_empty = [](const string& s, const string& field_name) {
        if (s.empty()) throw invalid_argument("Il campo " + field_name + " non può essere vuoto.");
    };

    // 2. Validazione Formato Codice (CLT-XXXX)
    auto check_codice = [](const string& s) {
        if (s.length() != 8 || s.substr(0, 4) != "CLT-") 
            throw invalid_argument("Formato codice errato. Richiesto: CLT-XXXX.");
        // Controlla che gli ultimi 4 caratteri siano cifre
        if (!all_of(s.begin() + 4, s.end(), ::isdigit))
            throw invalid_argument("La parte numerica del codice deve contenere solo cifre.");
    };

    // 3. Validazione Email (deve contenere '@')
    auto check_email = [](const string& s) {
        if (s.find('@') == string::npos) 
            throw invalid_argument("Email non valida: manca il carattere '@'.");
    };

    // 4. Validazione Telefono (solo cifre, 10 caratteri)
    auto check_telefono = [](const string& s) {
        if (s.length() != 10) 
            throw invalid_argument("Il numero di telefono deve essere di 10 cifre.");
        if (!all_of(s.begin(), s.end(), ::isdigit))
            throw invalid_argument("Il numero di telefono deve contenere solo numeri.");
    };

    // 5. Validazione Età (valore positivo)
    auto check_eta = [](int n) {
        if (n <= 0) throw invalid_argument("L'età deve essere un numero positivo.");
        if (n > 120) throw invalid_argument("Età non valida (troppo alta).");
    };

    // 6. Validazione Tipologia
    auto check_tipo = [](Tipologia_cliente t) {
        if (t == Tipologia_cliente::UNKNOWN) 
            throw invalid_argument("Tipologia cliente non valida.");
    };

    // --- ESECUZIONE CONTROLLI (TRY-CATCH) ---
    try {
        check_not_empty(codice, "Codice");
        check_not_empty(nome, "Nome");
        check_not_empty(cognome, "Cognome");
        check_not_empty(email, "Email");
        check_not_empty(tel, "Telefono");
        
        check_codice(codice);
        check_email(email);
        check_telefono(tel);
        check_eta(eta);
        check_tipo(tipo);

        // Se tutti i controlli passano, crea l'oggetto usando il costruttore privato
        // Nota: usiamo new perché make_shared non può accedere al costruttore privato
        return shared_ptr<Cliente>(new Cliente(codice, nome, cognome, email, tel, eta, tipo));

    } catch (const invalid_argument& e) {
        // Gestione dell'errore: stampiamo il messaggio e ritorniamo nullptr
        cerr << "Errore creazione Cliente (" << nome << " " << cognome << "): " << e.what() << endl;
        return nullptr;
    }
}

// Costruttore Privato
Cliente::Cliente(string codice, string nome, string cognome, 
                 string email, string tel, int eta, Tipologia_cliente tipo)
    : codice_cliente(codice), nome(nome), cognome(cognome),
      email(email), telefono(tel), eta(eta), tipologia(tipo) 
{
    cout << "Cliente " << codice << " creato con successo." << endl;
}

// Getter

string Cliente::get_codice_cliente() const{
    return this->codice_cliente;
}

string Cliente::get_nome_completo() const{
    return this->nome + " " + this->cognome;
}

int Cliente::get_eta() const{
    return this->eta;
}

Tipologia_cliente Cliente::get_tipologia_cliente() const{
    return this->tipologia;
}

// Metodo applica sconto
double Cliente::applica_sconto(double prezzo_base) const {
    if (prezzo_base < 0) return 0.0; // Controllo difensivo extra
    switch (this->tipologia) {
        case Tipologia_cliente::VIP:      return prezzo_base * 0.80;
        case Tipologia_cliente::PREMIUM:  return prezzo_base * 0.90;
        default:                          return prezzo_base;
    }
}

// Metodo stampa info
string Cliente::stampa_info() const {
    stringstream ss;
    ss << "Cliente: " << get_nome_completo() << " (" << this->codice_cliente << ")" << endl;
    ss << "Email: " << this->email << " | Tel: " << this->telefono << endl;
    ss << "Età: " << this->eta << endl;
    ss << "Tipologia: " << tipologia_to_string(this->tipologia);
    return ss.str();
}

string Cliente::tipologia_to_string(Tipologia_cliente tipo) {
    switch (tipo) {
        case Tipologia_cliente::STANDARD: return "Standard";
        case Tipologia_cliente::PREMIUM:  return "Premium";
        case Tipologia_cliente::VIP:      return "VIP";
        default: return "Sconosciuto";
    }
}

Tipologia_cliente Cliente::string_to_tipologia(string tipo) {
    if (tipo == "Standard" || tipo == "standard") return Tipologia_cliente::STANDARD;
    if (tipo == "Premium" || tipo == "premium")   return Tipologia_cliente::PREMIUM;
    if (tipo == "VIP" || tipo == "vip" || tipo == "Vip") return Tipologia_cliente::VIP;
    return Tipologia_cliente::UNKNOWN;
}
