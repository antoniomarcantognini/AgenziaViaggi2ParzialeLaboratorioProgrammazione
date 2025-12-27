#include <iostream>
#include <string>

int stampa_menu_e_scelta(vector<std::string> nome_opzioni) {
    cout << " === " << nome_opzioni[0] << " === " << endl;
    size_t dimensione = nome_opzioni.size();
    for (size_t i=0; i<dimensione; ++i) {
        cout << "   " << i << ": " << nome_opzioni[i] << endl;
    }

    do{
        int scelta = 0;
        cout << "La tua scelta: " << endl;
        cin >> scelta;
        bool flag = true;
        for (size_t i = 0; i<dimensione; ++i) {
            flag = flag || scelta == i;
            if (!flag) break;
        }
        if (flag) return scelta;
        else cerr << "Errore! L'elemento inserito è errato: sono ammessi solo i valori 1 e 2! Riprova." << endl;
    } while (1);
}

int menu_reinserimento() {
    stampa_menu_e_scelta({"SCELTA REINSERIMENTO", "Riprova inserimento", "Annulla operazione e torna al menu principale"});
}