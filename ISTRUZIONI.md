# Guida d'installazione

il processo di installazione è pensato per Arch Linux con architettura x86_64

è possibile eseguirlo su altre distribuzioni Linux, ma potrebbero essere necessarie variazioni, come per esempio l'uso package manager o i nomi dei pacchetti della distribuzione.

## Installazione delle dipendenze

### Requisiti

E' necessario avere installati cmake, libtool, autoconf, automake, make e g++.
E' necessario avere installati Sqlite3 e tutte le sue dipendenze (usualmente pre-distribuite nel pacchetto `base` durante l'installazione manuale di Arch Linux).
E' necessario avere installati Proxygen e tutte le sue dipendenze.

Ho predisposto uno script bash `install_dependencies.sh` che controlla quali librerie e dipendenze non sono state installate e eventualmente le scarica e compila con `git`.
Se si possiede un AUR helper, su AUR è presente un pacchetto che installa da sorgenti Proxygen datato 2021.
Tutta via si *consiglia fortemente* di usare ugualmente lo script citato per controllare di soddisfare tutti i requisiti.

Il primo metodo è comunque *preferibile* in quanto è quanto più 'aggiornato' e 'controllato'.
Infatti i repository di git da cui scarica le librerie sono fork pubblici recenti creati da me per controllare meglio il processo di compilazione.

### Steps

#### Installazione delle dipendenze

##### Automatica

usando `sudo pacman -S --needed` si devono installare i seguenti software:
automake autoconf boost boost-libs double-conversion fmt gflags git google-glog gperf gtest libevent libsodium lz4 openssl python snappy sqlite zstd zlib ninja cmake libtool xz

esempio di installazione con l'AUR helper `yay`
```
yay -Syu proxygen
```

#### "Manuale"

```
make install
```

## Installazione della mia applicazione

### Steps

```
make build
```

# Guida all'uso

## cartella `runtime`

Questa cartella contiene la totalità dei file che il server usa in fase di esecuzione

Questa comprende una copia dell'eseguibile compilato `server`, un file di configurazione `config.json`, una cartella `static` con tutti i file html, js, css esposti staticamente ai client.
Contiene inoltre il file `database.db`, un file in formato sqlite3 che rappresenta la copia di dati software in memoria secondaria. Poichè creato automaticamente da sqlite3, la sua cancellazione dal disco **coincide** con un reset logico della memoria server-side che si manifesta all'avvio dell'eseguibile `server`. Se il software è ancora in esecuzione quando quel file viene cancellato, questo cambiamento di stato verrà rilevato e come tentativo di riallineamento rispetto alla memoria secondaria, i dati in memoria primaria verranno "cancellati", e si procederà al ricreare le tabelle necessarie alla normale esecuzione, sospendendo temporaneamente la possibilità di eseguire azioni che prevedano la creazione o l'aggiornamento di dati fino al termine della operazione di riallineamento.
