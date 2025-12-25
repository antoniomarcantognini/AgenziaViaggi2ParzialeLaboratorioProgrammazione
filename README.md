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
- I metodi privati di validazione dentro la classe prenotazione possono essere definiti come lambda function: studia Cliente.h e modifica codice prenotazione di conseguenza (__DONE__)
- Controllare che non manchino try/catch o controlli di alcun tipo sui return di funzioni boolean
- Modificare codice gestore_azienda dopo l'aggiornamento dei costruttori dei pacchetti (__DONE__)
- Inserire tutti i riferimenti nelle funzioni "Aggiungi..." (__DONE__)
- __URGENTE__ Creare file menu.h con dentro la funzione stampa_menu_e_scelta(vector<std::string> nome_opzioni) (__DONE__)
- Inserire template per enum_name ed enum_cast (__DONE__)
- Inserire controlli input di passaggi nei metodi "Aggiungi..."
- Assicurarsi che vengano effettuati tutti i controlli su stoe tramite if(...has_value()) (__DONE__)
- Snellire il codice del gestore_azienda
- Scrivere i commenti nel gestore_azienda
# PER ANTO:
- Togliere tipologia_to_string, string_to_tipologia ed Utils_enum (basta usare direttamente magic_enum e un template sul gestore_azienda)
- Aggiungere il costruttore privato e try/catch nei pacchetti (poi avvisare rick e segnare nel readme di modificare gestore_azienda di conseguenza) (__DONE__)
## IMPORTANTE ## 
-Ho messo il template per le conversioni fra classi di enum e stringhe
