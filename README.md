# AgenziaViaggi2ParzialeLaboratorioProgrammazione

# IDEE:
Classi template enum per gestire difficoltà skipass, hotel 3 / 4 /5 stelle , ecc
Funzione string utility con classe template
gestisci menu con le funzioni singole

- controllare override e const (!!!)
- gestione della memoria con uso corretto di new e delete (provare a utilizzare gli smart pointer)
- try catch sul costruttore
- fare il diagramma di classi UML, draw.io, Visual paradigm (ereditarietà e composizioni)

# PER RICK:
- (__DONE__) I metodi privati di validazione dentro la classe prenotazione possono essere definiti come lambda function: studia Cliente.h e modifica codice prenotazione di conseguenza 
- Controllare che non manchino try/catch o controlli di alcun tipo sui return di funzioni boolean
- (__DONE__) Modificare codice gestore_azienda dopo l'aggiornamento dei costruttori dei pacchetti 
- (__DONE__) Inserire tutti i riferimenti nelle funzioni "Aggiungi..." 
- (__DONE__) Creare file menu.h con dentro la funzione stampa_menu_e_scelta(vector<std::string> nome_opzioni) 
- (__DONE__) Inserire template per enum_name ed enum_cast 
- Inserire controlli input di passaggi nei metodi "Aggiungi..."
- (__DONE__) Assicurarsi che vengano effettuati tutti i controlli su stoe tramite if(...has_value()) 
- Snellire il codice del gestore_azienda
- Scrivere i commenti nel gestore_azienda
- (__DONE__) Finire il main
- (__DONE__) Aggiungere i prototipi
- (__DONE__) Correggere la visualizzazione per tipologia (eliminare template)
- (__DONE__) Creare un metodo "crea..." privato e utilizzarlo nel caricamento dei file
- (__DONE__) Fare il controllo del "crea_elemento" nelle varie funzioni di caricamento file
- Scrivere le funzioni di Gestore_azienda in sneak_case
- Togliere tutti i vector<string> nei vari stampa_menu_e_scelta()
- Cambiare la logica di salvataggio e caricamento (al posto di get_nome_cognome occorre inserire get_nome e get_cognome)
- Modificare tutti i salvataggi dei file scrivendo file.close() e togliere in salvaDatiSuFile
- Inserire nel main un controllo sul numero di clienti/pacchetti/prenotazioni inserite.
- __IMPORTANTE__ Ricordare di inserire un controllo del codice univoco nei vari caricamenti di file
- Inserire controllo "if(linea.empty()) continue;" nelle funzioni di caricamento di file.
# PER ANTO:
- Togliere tipologia_to_string, string_to_tipologia ed Utils_enum (basta usare direttamente magic_enum e un template sul gestore_azienda)
- Aggiungere il costruttore privato e try/catch nei pacchetti (poi avvisare rick e segnare nel readme di modificare gestore_azienda di conseguenza) (__DONE__)
# PER ENTRAMBI
- Fare controlli di formato nel caricamento dei file
## IMPORTANTE ## 
-Ho messo il template per le conversioni fra classi di enum e stringhe
