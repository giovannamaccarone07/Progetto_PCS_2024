# Project PCS - 2024 - PoliTO

### Aggiornamenti
- 19/05 sul branch gian: ho staccato il branch dal branch arianna il 18/05 e sistemato alcune righe, ho sostituito alcuni VectorXi con vector<unsigned int> perchè non mi sembrava servissero operazioni matematiche particolari.
CoordinateVertici non posso vedero come matrice NumeroVertici x 3 perchè a priori non so quanti sono i NumeroVertici... forse devo vederlo come vettore di matrici più piccole per ogni frattura

- 25/05 sul  branch input: ho cambiato  vector<Vector3d> CoordinateVertici = {};  con un vector<MatrixXd> CoordinateVertici = {}; ad ogni indice del vector che sarà lungo quanto il numero di fratture, associo una matrice 3xNumeroVertici ... forse sarebbe più comodo per memorizzarle poter usare una lista list<MatrixXd> in questo modo ad ogni iterata del ciclo di lettura devo solo appendere la MatrixXd. Se usiamo il vector non so a priori quanto sarà lungo perchè quando faccio  fract.CoordinateVertici.reserve(fract.NumeroFratture * 8) riservo spazio per un unsigned int ?? cioè c'è differenza nel resize se abbiamo un vector<Vector3d> o un vector<MatrixXd>...
Quindi in lettura devo per prima cosa creo una list<MatrixXd> poi a fine lettura dimensiono un vettore della giusta grandezza e travaso la lista.?? Il problema è IndiciVertici che non so quanto senso abbia ora perchè i vertici della frattura sono gia dentro la corrispettiva matrice, li trovo tutti li, non ho bisogno di distinguerli nella matriciona grande che avevamo prima.  

### Cose da Discutere
- meglio usare VectorX di Eigen oppure vector di STL nell'input dei file ? capire qual'è la sostanziale differenza, i vettori di Eigen permettono operazioni matematiche migliori ?

- meglio fare un resize anzichè reserve + push_back 
- IdFRatture tanto è un vettore ordinato da 0 a n_fratture
- SpazioMemoria

- introdurre una variabile nella classe struct : tolleranza, gestione tolleranze
- rivedere funzione piani paralleli
- capire perché la direzione del lato si calcola come punto iniziale - punto finale e non viceversa
- rivedere l'indice del ciclo for e scriverlo in modulo 6


### Codici di errore

