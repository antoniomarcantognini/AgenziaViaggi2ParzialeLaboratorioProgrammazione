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
- Sei sicuro prince di togliere tipologia_to_string ? perche nelle enum rimane stampato il _ (in teoria)
# PER ANTO:
- Togliere tipologia_to_string, string_to_tipologia ed Utils_enum (basta usare direttamente magic_enum)
- Aggiungere il costruttore privato e try/catch nei pacchetti (poi avvisare rick e segnare nel readme di modificare gestore_azienda di conseguenza)
## IMPORTANTE ## 
-Ho messo il template per le conversioni fra classi di enum e stringhe
