# Install guide

il processo di installazione è pensato per Arch Linux

è possibile eseguirlo su altre distribuzioni Linux, ma potrebbero essere necessarie variazioni, come per esempio l'uso package manager o i nomi dei pacchetti della distribuzione.

## Installazione delle dipendenze

### Requisiti

E' necessario avere installati Sqlite3 e tutte le sue dipendenze (usualmente pre-distribuite nel pacchetto `base` durante l'installazione manuale di Arch Linux).
E' necessario avere installati Proxygen e tutte le sue dipendenze.
Se si possiede un AUR helper, su AUR è presente un pacchetto che installa da sorgenti Proxygen datato 2021.
Come alternativa ho predisposto uno script bash `install_dependencies.sh` che controlla quali librerie e dipendenze non sono state installate e eventualmente le scarica e compila con `git`.

Quest'ultima è preferibile in quanto è quanto meno 'controllata'.

### Steps

#### Installazione delle dipendenze

##### Automatica

esempio di installazione con l'AUR helper `yay`
```
yay -Syu proxygen
```

#### "Manuale"

```
./install_dependencies.sh
```

## Installazione della mia applicazione

