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
- inserire reference sulle chiamate di funzione&? 
- introdurre una variabile nella classe struct : tolleranza, gestione tolleranze
- rivedere funzione piani paralleli
- capire perché la direzione del lato si calcola come punto iniziale - punto finale e non viceversa
- rivedere l'indice del ciclo for e scriverlo in modulo 6
- rivedere le strutture in fratture.hpp perché abbiamo creato delle matrici di double ma abbiamo intenzione di inserirci dei booleani. Potrebbe andare 0/1?
- IMPORTANTE CheckTraccia va considerato il caso retta parallela al segmento opp coincidente
- BISOGNA implementare le variabili booleane pass1 e pass2 in CheckTraccia 

### Codici di errore

## COSE DA CHIEDERE ALLA TEORA

- rivedere le strutture in fratture.hpp perché abbiamo creato delle matrici di double ma abbiamo intenzione di inserirci dei booleani. Potrebbe andare 0/1?
- rivedere l'indice del ciclo for e scriverlo in modulo 6
- rivedere valore k
- paraview
- test
- come si fanno le 24 condizioni FILOSOFIA DI GIOVANNA
- 1 file per ogni frattura?
-   Vector3d vettoreMax1(-1e100, -1e100, -1e100); Nella boundingBox come inizializzo dei vettori vuoti in modo che possa fare dei paragoni con le x,y,z che trovo? 

- output tips va bene mettere direttamente false 

- MEMORIZZARE IN MODO EFFICIENTE I DATI NELLA STRUCT TRAC
- le matrici possono contenere tipi misti ? 
- è più costoso aggiungere su un vector con .push_back opp vector[i] = oggetto


