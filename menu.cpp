#include "menu.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits> 

using namespace std;

int menu::stampa_menu_e_scelta(const vector<string>& nome_opzioni) {
    if(nome_opzioni.empty()) return 0;

    cout << "\n === " << nome_opzioni[0] << " === " << endl;
    
    size_t dimensione = nome_opzioni.size();
    for (size_t i=1; i<dimensione; ++i) { // Partiamo da 1 perché 0 è il titolo
        cout << "   " << i << ": " << nome_opzioni[i] << endl;
    }

    int scelta = 0;
    do{
        cout << "La tua scelta: ";
        if (!(cin >> scelta)) {
            cin.clear();  // Ripristina lo stato del cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Pulisce tutto il buffer
            cerr << "Input non valido. Inserire un numero." << endl;
            continue;
        }
        
        // Controllo range (1 a dimensione-1)
        if (scelta >= 1 && scelta < static_cast<int>(dimensione)){
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Pulisce il buffer
            return scelta;
        }else{
            cerr << "Errore! L'elemento inserito è errato. Riprova." << endl;
        }
    } while (1);
}

int menu::menu_reinserimento() {
    return stampa_menu_e_scelta({"SCELTA REINSERIMENTO", "Riprova inserimento", "Annulla operazione e torna al menu principale"});
}
