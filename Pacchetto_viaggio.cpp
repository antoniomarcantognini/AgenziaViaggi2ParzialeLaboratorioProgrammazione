#include "Pacchetto_viaggio.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// Costruttore:
Pacchetto_viaggio::Pacchetto_viaggio(string codice, string dest, int giorni, double prezzo)
    : codice_pacchetto(codice), destinazione(dest), durata_giorni(giorni), prezzo_base(prezzo)
{
    // validazione input:
    if(giorni <= 0) throw invalid_argument("Durata del viaggio negativa.");
    if(prezzo < 0) throw invalid_argument("Prezzo base negativo.");
    if(codice.empty() || codice == nullptr) throw invalid_argument("Codice vuoto.");
    if(dest.empty() || dest == nullptr) throw invalid_argument("Destinazione vuota.");

    this->disponibile = true;
    pacchetti_creati++;
    cout << "Costruito pacchetto base: " << codice << endl;
}

// distruttore virtuale:
Pacchetto_viaggio::~Pacchetto_viaggio(){
    cout << "Distruzione pacchetto base: " << codice_pacchetto << endl;
}

// getter:
string Pacchetto_viaggio::get_codice() const{
    return this->codice_pacchetto;
}

string Pacchetto_viaggio::get_destinazione() const{
    return this->destinazione;
}

int Pacchetto_viaggio::get_durata_giorni() const{
    return this->durata_giorni;
}

bool Pacchetto_viaggio::is_disponibile() const{
    return this->disponibile;
}

void Pacchetto_viaggio::set_disponibile(bool disponibilita){
    this->disponibile = disponibilita;
}
