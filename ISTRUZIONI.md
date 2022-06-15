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

usando `sudo pacman -S --needed` si devono installare i seguenti software:

 - automake autoconf boost boost-libs double-conversion fmt gflags git google-glog gperf gtest libevent libsodium lz4 openssl python snappy sqlite zstd zlib ninja cmake libtool xz

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
linux-vdso.so.1 (0x00007ffde17bd000)
libsqlite3.so.0 => /usr/lib/libsqlite3.so.0 (0x00007f1796e7c000)
libssl.so.1.1 => /usr/lib/libssl.so.1.1 (0x00007f1796369000)
libjsoncpp.so.25 => /usr/lib/libjsoncpp.so.25 (0x00007f1796e42000)
libfmt.so.8 => /usr/lib/libfmt.so.8 (0x00007f1796348000)
libz.so.1 => /usr/lib/libz.so.1 (0x00007f179632e000)
libbz2.so.1.0 => /usr/lib/libbz2.so.1.0 (0x00007f179631b000)
liblzma.so.5 => /usr/lib/liblzma.so.5 (0x00007f17962ef000)
liblz4.so.1 => /usr/lib/liblz4.so.1 (0x00007f17962cc000)
libsnappy.so.1 => /usr/lib/libsnappy.so.1 (0x00007f17962c0000)
libsodium.so.23 => /usr/lib/libsodium.so.23 (0x00007f1796266000)
libunwind.so.8 => /usr/lib/libunwind.so.8 (0x00007f179624c000)
libzstd.so.1 => /usr/lib/libzstd.so.1 (0x00007f17961a3000)
libcrypto.so.1.1 => /usr/lib/libcrypto.so.1.1 (0x00007f1795e00000)
libglog.so.1 => /usr/lib/libglog.so.1 (0x00007f179616b000)
libgflags.so.2.2 => /usr/lib/libgflags.so.2.2 (0x00007f179613e000)
libdouble-conversion.so.3 => /usr/lib/libdouble-conversion.so.3 (0x00007f1796128000)
libboost_context.so.1.78.0 => /usr/lib/libboost_context.so.1.78.0 (0x00007f1796123000)
libevent-2.1.so.7 => /usr/lib/libevent-2.1.so.7 (0x00007f1795da7000)
libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007f1795a00000)
libm.so.6 => /usr/lib/libm.so.6 (0x00007f1795cc0000)
libgcc_s.so.1 => /usr/lib/libgcc_s.so.1 (0x00007f1796101000)
libc.so.6 => /usr/lib/libc.so.6 (0x00007f1795600000)
/lib64/ld-linux-x86-64.so.2 => /usr/lib64/ld-linux-x86-64.so.2 (0x00007f1796fd8000)
libpthread.so.0 => /usr/lib/libpthread.so.0 (0x00007f17960fc000)
```

# Guida all'uso

## cartella `runtime`

Questa cartella contiene la totalità dei file che il server usa in fase di esecuzione

Questa comprende una copia dell'eseguibile compilato `server`, un file di configurazione `config.json`, una cartella `static` con tutti i file html, js, css esposti staticamente ai client.

Contiene inoltre il file `database.db`, un file in formato sqlite3 che rappresenta i dati software in memoria secondaria.
E' inoltre presente un file `database.sql` che contiene le regole SQL che il server usa per creare il database in caso di rimozione del file.
