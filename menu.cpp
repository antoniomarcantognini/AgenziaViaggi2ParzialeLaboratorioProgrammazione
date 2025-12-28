#include <iostream>
#include <string>
#include <vector>
#include "menu.h"

using namespace std;

int menu::stampa_menu_e_scelta(vector<string> nome_opzioni) {
    cout << " === " << nome_opzioni[0] << " === " << endl;
    size_t dimensione = nome_opzioni.size();
    for (size_t i=1; i<dimensione; ++i) { // Partiamo da 1 perché 0 è il titolo
        cout << "   " << i << ": " << nome_opzioni[i] << endl;
    }

    do{
        int scelta = 0;
        cout << "La tua scelta: ";
        if (!(cin >> scelta)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cerr << "Input non valido." << endl;
            continue;
        }
        
        // Controllo range (1 a dimensione-1)
        if (scelta >= 1 && scelta < (int)dimensione) return scelta;
        else cerr << "Errore! L'elemento inserito è errato. Riprova." << endl;
    } while (1);
}

int menu::menu_reinserimento() {
    return stampa_menu_e_scelta({"SCELTA REINSERIMENTO", "Riprova inserimento", "Annulla operazione e torna al menu principale"});
}
