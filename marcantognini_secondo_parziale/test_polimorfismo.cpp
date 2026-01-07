#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "Pacchetto_viaggio.h"
#include "Pacchetto_citta.h"
#include "Pacchetto_mare.h"
#include "Pacchetto_montagna.h"
#include "Pacchetto_avventura.h"
#include "Utils_enum.h"

using namespace std;

int main() {
    system("chcp 65001 > nul");

    cout << "===================================================" << endl;
    cout << "       TEST DI VERIFICA POLIMORFISMO (6.2)         " << endl;
    cout << "===================================================" << endl;

    // 1. Creazione del contenitore polimorfico
    // Usiamo un vettore di puntatori alla CLASSE BASE
    vector<shared_ptr<Pacchetto_viaggio>> catalogo_test;

    cout << "\n[1] Creazione oggetti di tipi diversi..." << endl;

    // 2. Istanziazione di oggetti derivati diversi

    // Pacchetto CITTÀ (Base: 300 + Musei + Hotel 4*)
    auto p_citta = Pacchetto_citta::crea_pacchetto(
        "TEST-CITY", "Firenze", 3, 300.0, 
        4, true, Categoria_hotel::Quattro_stelle
    );

    // Pacchetto MARE (Base: 600 + Ombrellone + Pensione Completa)
    auto p_mare = Pacchetto_mare::crea_pacchetto(
        "TEST-SEA", "Sardegna", 7, 600.0, 
        true, false, Categoria_pensione::Pensione_completa
    );

    // Pacchetto MONTAGNA (Base: 400 + Skipass x Giorni + Difficoltà Alta)
    auto p_mont = Pacchetto_montagna::crea_pacchetto(
        "TEST-MNT", "Dolomiti", 5, 400.0, 
        true, 2, Categoria_difficolta::Difficile
    );

    // Pacchetto AVVENTURA (Base: 200 + Attività + Adrenalina Alta)
    vector<string> attivita = {"Rafting", "Bungee Jumping"};
    auto p_adv = Pacchetto_avventura::crea_pacchetto(
        "TEST-ADV", "Gran Sasso", 2, 200.0, 
        attivita, Categoria_adrenalina::Alto, true
    );

    // 3. Inserimento nel vettore della classe base
    if(p_citta) catalogo_test.push_back(p_citta);
    if(p_mare) catalogo_test.push_back(p_mare);
    if(p_mont) catalogo_test.push_back(p_mont);
    if(p_adv) catalogo_test.push_back(p_adv);

    cout << "[OK] Inseriti " << catalogo_test.size() << " oggetti nel vettore di Pacchetto_viaggio*." << endl;

    cout << "\n[2] Iterazione e Chiamata Metodi Virtuali..." << endl;
    cout << "---------------------------------------------------" << endl;

    // 4. Iterazione Polimorfica
    // Il ciclo non sa cosa sta puntando esattamente, sa solo che è un Pacchetto_viaggio.
    for (const auto& pacchetto : catalogo_test) {
        
        // A. Verifica del tipo a runtime
        cout << ">>> Tipo Rilevato (get_tipologia): " << pacchetto->get_tipologia() << endl;

        // B. Chiamata al metodo virtuale puro stampa_dettagli()
        // Ogni oggetto risponderà stampando i SUOI campi specifici (musei, o skipass, ecc.)
        cout << pacchetto->stampa_dettagli();

        // C. Chiamata al metodo virtuale puro calcola_prezzo_finale()
        // Ogni oggetto userà la SUA formula matematica
        double prezzo = pacchetto->calcola_prezzo_finale();
        cout << " -> Prezzo Calcolato (Binding Dinamico): " << prezzo << " EUR" << endl;
        
        cout << "---------------------------------------------------" << endl;
    }

    cout << "\n[CONCLUSIONE]" << endl;
    cout << "Il test ha dimostrato che invocando lo stesso metodo su puntatori" << endl;
    cout << "generici, viene eseguito il codice specifico della classe derivata." << endl;

    return 0;
}