#include "Pacchetto_viaggio.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// INIZIALIZZAZIONE MEMBRO STATICO
int Pacchetto_viaggio::pacchetti_creati = 0;

// IMPLEMENTAZIONE GETTER STATICO
int Pacchetto_viaggio::get_pacchetti_creati() {
    return pacchetti_creati;
}

Pacchetto_viaggio::Pacchetto_viaggio(string codice, string dest, int giorni, double prezzo)
    : codice_pacchetto(codice), destinazione(dest), durata_giorni(giorni), prezzo_base(prezzo)
{
    if(giorni <= 0) throw runtime_error("Durata del viaggio negativa.");
    if(prezzo < 0) throw runtime_error("Prezzo base negativo.");
    if(codice.empty()) throw runtime_error("Codice vuoto.");
    if(dest.empty()) throw runtime_error("Destinazione vuota.");

    this->disponibile = true;
    
    // Incremento del contatore
    pacchetti_creati++;
    
    cout << "Costruito pacchetto base: " << codice << endl;
}

Pacchetto_viaggio::~Pacchetto_viaggio(){
     cout << "Pacchetto " << this->codice_pacchetto << " distrutto." << endl;
}

// Getter
string Pacchetto_viaggio::get_codice() const{ return this->codice_pacchetto; }
string Pacchetto_viaggio::get_destinazione() const{ return this->destinazione; }
int Pacchetto_viaggio::get_durata_giorni() const{ return this->durata_giorni; }
double Pacchetto_viaggio::get_prezzo_base() const{ return this->prezzo_base; }
bool Pacchetto_viaggio::is_disponibile() const{ return this->disponibile; }
void Pacchetto_viaggio::set_disponibile(bool disponibilita){ this->disponibile = disponibilita; }

bool Pacchetto_viaggio::set_pacchetti_creati(int n) {
    if (n < 0) return false;
    pacchetti_creati = n;    // Aggiornamento variabile static
    return true;
}

bool Pacchetto_viaggio::salva_dati_su_file(ofstream& file) const {
    file << this->codice_pacchetto << ";"
         << this->destinazione << ";"
         << this->durata_giorni << ";"
         << (this->disponibile ? "Disponibile" : "Non Disponibile") << ";"
         << this->prezzo_base << ";"
         << this->get_tipologia() << ";";
    return true;
}