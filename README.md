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
- I metodi privati di validazione dentro la classe prenotazione possono essere definiti come lambda function (FATTO): studia Cliente.h e modifica codice prenotazione di conseguenza
- Inserire metodi pubblici con inserimento manuale delle variabili
# PER ANTO:
- Togliere tipologia_to_string, string_to_tipologia ed Utils_enum (basta usare direttamente magic_enum)
- Aggiungere il costruttore privato e try/catch nei pacchetti (poi avvisare rick e segnare nel readme di modificare gestore_azienda di conseguenza)
## IMPORTANTE ## 
-Ho messo il template per le conversioni fra classi di enum e stringhe
