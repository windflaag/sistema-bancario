# Guida d'installazione

il processo di installazione è pensato per Arch Linux con architettura x86_64

è possibile eseguirlo su altre distribuzioni Linux, ma potrebbero essere necessarie variazioni, come per esempio l'uso package manager o i nomi dei pacchetti della distribuzione.

## Installazione delle dipendenze

### Requisiti

E' necessario avere installati Proxygen e tutte le sue dipendenze.

Ho predisposto uno script bash `install_dependencies.sh` che controlla quali librerie e dipendenze non sono state installate e eventualmente le scarica e compila con `git`.
Se si possiede un AUR helper, su AUR è presente un pacchetto che installa da sorgenti Proxygen datato 2021.
Tutta via si *consiglia fortemente* di usare ugualmente lo script citato per controllare di soddisfare tutti i requisiti.

Il primo metodo è comunque *preferibile* in quanto è quanto più 'aggiornato' e 'controllato'.

E' necessario avere installati alcuni tool per c++ e librerie annesse che sono presenti nei repository ufficiali di Arch Linux. La lista è presente nei passi di installazione (e vengono installati automaticamente dal mio script)
Infatti i repository di git da cui scarica le librerie sono fork pubblici recenti creati da me per controllare meglio il processo di compilazione.

### Steps

#### Installazione delle dipendenze

##### Automatica

usando `sudo pacman -Syu --needed` si devono installare i seguenti software:

 - automake autoconf boost boost-libs double-conversion fmt gflags git google-glog gperf gtest jsoncpp libevent libsodium lz4 openssl python snappy sqlite zstd zlib ninja cmake libtool xz openssl

poi usando l'AUR helper scelto installare proxygen:

esempio con `yay`
```
yay -Syu proxygen
```

#### "Manuale"

```
make install
```

make richiama lo script `install_dependencies.sh`

## Installazione della mia applicazione

### Steps

```
make compile
```

make richiama lo script `build_project.sh`

Dopo la compilazione si presenterà un eseguibile al percorso runtime/server. Questo è il web server REST della consegna.

Di seguito lascio il risultato di `ldd runtime/server` per confrontarlo con l'eseguibile ottenuto.
```

```

# Guida all'uso

## cartella `runtime`

Questa cartella contiene la totalità dei file che il server usa in fase di esecuzione

Questa comprende una copia dell'eseguibile compilato `server`, un file di configurazione `config.json`, una cartella `static` con tutti i file html, js, css esposti staticamente ai client.

Contiene inoltre il file `database.db`, un file in formato sqlite3 che rappresenta i dati software in memoria secondaria.
E' inoltre presente un file `database.sql` che contiene le regole SQL che il server usa per creare il database in caso di rimozione del file.
