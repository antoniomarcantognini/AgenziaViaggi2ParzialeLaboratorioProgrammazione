#include "Cliente.h"
#include "Utils_enum.h"
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <algorithm>

using namespace std;

// Metodo di Validazione input cliente con Lambda Functions
bool Cliente::valida_dati() const {
    
    // 1. Lambda per stringhe vuote
    auto check_not_empty = [](const string& s, const string& field_name) {
        if (s.empty()) throw runtime_error("Il campo " + field_name + " non può essere vuoto.");
    };

    // 2. Lambda per Codice (CLT-XXXX)
    auto check_codice = [](const string& s) {
        if (s.length() != 8 || s.substr(0, 4) != "CLT-") 
            throw runtime_error("Formato codice errato (" + s + "). Richiesto: CLT-XXXX.");
        if (!all_of(s.begin() + 4, s.end(), ::isdigit)) 
            throw runtime_error("La parte numerica del codice deve contenere solo cifre.");
    };

    // 3. Lambda per Email (@)
    auto check_email = [](const string& s) {
        if (s.find('@') == string::npos) 
            throw runtime_error("Email non valida (" + s + "): manca il carattere '@'.");
    };

    // 4. Lambda per Telefono (10 cifre)
    auto check_telefono = [](const string& s) {
        if (s.length() != 10) 
            throw runtime_error("Il telefono deve essere di 10 cifre.");
        if (!all_of(s.begin(), s.end(), ::isdigit)) 
            throw runtime_error("Il telefono deve contenere solo numeri.");
    };

    // 5. Lambda per Età
    auto check_eta = [](int n) {
        if (n <= 0) throw runtime_error("L'età deve essere un numero positivo.");
        if (n > 130) throw runtime_error("Età non valida (troppo alta).");
    };

    // 6. Lambda per Tipologia
    auto check_tipo = [](Tipologia_cliente t) {
        if (t == Tipologia_cliente::UNKNOWN) 
            throw runtime_error("Tipologia cliente non valida.");
    };

    // Esecuzione dei controlli
    check_not_empty(this->codice_cliente, "Codice");
    check_not_empty(this->nome, "Nome");
    check_not_empty(this->cognome, "Cognome");
    check_not_empty(this->email, "Email");
    check_not_empty(this->telefono, "Telefono");
    
    check_codice(this->codice_cliente);
    check_email(this->email);
    check_telefono(this->telefono);
    check_eta(this->eta);
    check_tipo(this->tipologia);

    return true;
}

// Costruttore Privato
Cliente::Cliente(string codice, string nome, string cognome, 
                 string email, string tel, int eta, Tipologia_cliente tipo)
    : codice_cliente(codice), 
      nome(nome), 
      cognome(cognome),
      email(email), 
      telefono(tel), 
      eta(eta), 
      tipologia(tipo) 
{
    valida_dati();

    prossimo_codice_cliente++;
    
    cout << "Cliente " << this->codice_cliente << " creato correttamente." << endl;
}

// Factory Method Statico
shared_ptr<Cliente> Cliente::crea_cliente(string codice, string nome, string cognome, 
                                          string email, string tel, int eta, Tipologia_cliente tipo) {
    try {
        // Tenta di creare l'oggetto. Se le lambda nel costruttore lanciano eccezione, andiamo al catch.
        return shared_ptr<Cliente>(new Cliente(codice, nome, cognome, email, tel, eta, tipo));
    } catch (const runtime_error& e) {
        // Gestione uniforme dell'errore (cerr + return nullptr)
        cerr << "Errore creazione Cliente (" << nome << " " << cognome << "): " << e.what() << endl;
        return nullptr;
    }
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
        case Tipologia_cliente::Premium:  return prezzo_base * 0.90;
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

// Metodi di conversione usando il template
string Cliente::tipologia_to_string(Tipologia_cliente tipo) {
    return Utils_enum::to_string(tipo); 
}

Tipologia_cliente Cliente::string_to_tipologia(string tipo) {
    return Utils_enum::from_string<Tipologia_cliente>(tipo); 
}

// Distruttore
Cliente::~Cliente() {
    cout << "Cliente " << this->codice_cliente << " distrutto." << endl;
}
