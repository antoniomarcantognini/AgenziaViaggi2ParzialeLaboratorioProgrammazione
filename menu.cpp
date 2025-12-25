#include <iostream>
#include <string>

int stampa_menu_e_scelta(vector<std::string> nome_opzioni) {
    cout << " === " << nome_opzioni[0] << " === " << endl;
    for (size_t i=0; i<nome_opzioni.size(); ++i) {
        cout << "   " << i << ": " << nome_opzioni[i] << endl;
    }

    do{
        int scelta = 0;
        cout << "La tua scelta: " << endl;
        cin >> scelta;
        if (scelta == 1 || scelta == 2) return scelta;
        else cerr << "Errore! L'elemento inserito è errato: sono ammessi solo i valori 1 e 2! Riprova." << endl;
    } while (1);
}