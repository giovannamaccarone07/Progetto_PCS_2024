# Project PCS - 2024 - PoliTO

### Aggiornamenti
- 19/05 sul branch gian: ho staccato il branch dal branch arianna il 18/05 e sistemato alcune righe, ho sostituito alcuni VectorXi con vector<unsigned int> perchè non mi sembrava servissero operazioni matematiche particolari.
CoordinateVertici non posso vedero come matrice NumeroVertici x 3 perchè a priori non so quanti sono i NumeroVertici... forse devo vederlo come vettore di matrici più piccole per ogni frattura
-
-

### Cose da Discutere
- meglio usare VectorX di Eigen oppure vector di STL nell'input dei file ? capire qual'è la sostanziale differenza, i vettori di Eigen permettono operazioni matematiche migliori ?

- meglio fare un resize anzichè reserve + push_back 
- IdFRatture tanto è un vettore ordinato da 0 a n_fratture
- SpazioMemoria

- introdurre una variabile nella classe struct : tolleranza, gestione tolleranze
- come unire t e d dentro la funzione per fare i piani
-  

### Codici di errore

### Calendario 
- 9 giugno: codice completato
- 18 giugno: consegna
- prima settimana (fino al 26): intersezioni, test in parallelo, dati input
- seconda settimana (fino al 2): tracce + output

(seconda parte, relazione e presentazione)
- terza settimana (fino al 9): seconda parte
- quarta settimana (fino al 16): seconda parte


