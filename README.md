# Sistema Bancario

## Authors

- Refolli Francesco 865955

## Summary

Progetto di Sistemi Distribuiti, costruito seguendo la traccia proposta #2 (REST).

La soluzione implementata integra un web server scritto in C++ con Proxygen, un database locale con SQLite3 e un front-end realizzato in Javascript con JQuery.

Gli endpoint che accettano informazioni nel body usano strettamente il formato JSON.
Tutti gli endpoint che ritornano informazioni nel body usano il formato JSON.

Per ottenere maggiori informazioni sul comportamento degli endpoint: <a href="#acknowledgments">link</a>.

<div id="top"></div>
<!--
    template from https://github.com/othneildrew/Best-README-Template.git
  -->


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">Sistema Bancario</h3>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
        <a href="#built-with">Built With</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

### Built With

* [Proxygen](https://github.com/facebook/proxygen.git)
* [SQLite3](https://www.sqlite.org/index.html)
* [C++](https://en.wikipedia.org/wiki/C%2B%2B)
* [Cmake](https://cmake.org/)
* [Javascript](https://en.wikipedia.org/wiki/JavaScript)
* [JQuery](https://jquery.com)

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

Il software necessita di essere compilato.

### Installation

Le istruzioni di installazione si trovano nel file `ISTRUZIONI.md`.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

Le istruzioni d'uso si trovano nel file `ISTRUZIONI.md`.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the BEERWARE License. See `LICENSE` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

<!-- shield per metodi HTTP -->
[HEAD]: https://img.shields.io/badge/-HEAD-brightgreen
[GET]: https://img.shields.io/badge/-GET-green
[POST]: https://img.shields.io/badge/-POST-red
[PUT]: https://img.shields.io/badge/-PUT-purple
[PATCH]: https://img.shields.io/badge/-PATCH-blue
[DELETE]: https://img.shields.io/badge/-DELETE-pink

### Endpoint standard (dalla Traccia 2)

#### ![][GET] /api/account

`curl localhost:8080/api/account`

```json
{
    "accounts": [
        "1ce141897056b58d1832",
        "d91627ab5e086f3b4b0c"
    ]
}
```

#### ![][POST] /api/account

inserisci un nuovo account secondo i campi name e surname dentro il body in format JSON.

i dati di tipo "Nome" (name e surname) devono essere stringhe di al massimo 80 caratteri fra:
 - lettere dell'alfabeto [a-z] sia *maiuscole* che *minuscole*
 - il carattere spazio ' '
 - il carattere punto '.'

`curl -X POST -d "{\"name\": \"Amedeo\", \"surname\": \"Bianchi\"}" localhost:8080/api/account`

```json
{
    "accountId": "d91627ab5e086f3b4b0c"
}
```

#### ![][DELETE] /api/account?id={accountId}

l'accountId viene cancellato dalla tabella degli account. 
Visto che è importante mantenere lo storico delle transazioni, la relativa tabella rimarrà intonsa

`curl -v -X DELETE localhost:8080/api/account?id=d91627ab5e086f3b4b0c`

```http
HTTP/1.1 200 OK
Date: Fri, 17 Jun 2022 11:09:35 GMT
Content-Length: 0
```

#### ![][HEAD] /api/account/{accountId}

`curl localhost:8080/api/account/d91627ab5e086f3b4b0c`

```http
HTTP/1.1 200 OK
Date: Fri, 17 Jun 2022 11:09:35 GMT
X-Sistema-Bancario: Amedeo;Bianchi
Content-Length: 0
```

#### ![][GET] /api/account/{accountId}

come HEAD, ma ritorna in formato JSON i dati dell'account.

`curl localhost:8080/api/account/d91627ab5e086f3b4b0c`

```json
{
    "credit": 0,
    "name": "Amedeo",
    "surname": "Bianchi",
    "transactions": []
}
```

#### ![][PUT] /api/account/{accountId}

cambia io nome E il cognome (entrambi) di un Account specificando il nuovo nome e il nuovo cognome nel body.

`curl -v -X PUT -d "{\"name\": \"Tommaso\", \"surname\": \"Rossi\"}" localhost:8080/api/account/cb92b3e7a99e0d743a33`

```http
HTTP/1.1 200 OK
Date: Fri, 17 Jun 2022 11:11:35 GMT
Content-Length: 0
```

#### ![][PATCH] /api/account/{accountId}

cambia io nome O il cognome (non entrambi) di un Account specificando il nuovo nome/cognome nel body.

`curl -v -X PATCH -d "{\"name\": \"Andrea\"}" localhost:8080/api/account/cb92b3e7a99e0d743a33`

```http
HTTP/1.1 200 OK
Date: Fri, 17 Jun 2022 11:09:35 GMT
Content-Length: 0
```

#### ![][POST] /api/account/{accountId}

inserisce un prelievo o un deposito, a seconda che amount sia < o >= 0.

ritorna l'id della transazione e il nuovo credito dell'account.

`curl -X POST -d "{\"amount\": 9}" localhost:8080/api/account`

```json
{
    "credit": 9,
    "transaction": "3a1d8e47-2311-4844-998c-46a351a8542a"
}
```

#### ![][POST] /api/transfer

inserisce una transazione con amount >= 0.

ritorna l'id della transazione e il nuovo credito degli account coinvolti.

`curl -X POST -d "{\"from\": \"079770774a0285b6e104\", \"to\": \"cff5b87b40e5c2b0a947\", \"amount\": 9}" localhost:8080/api/account`

```json
{
    "079770774a0285b6e104": 1,
    "cff5b87b40e5c2b0a947": 98,
    "transaction":"f0783c9e-4f27-4389-aff7-7b3d1e8f47a3"
}
```

#### ![][POST] /api/divert

inserisce una transazione che inverte una transazione tra due account, il cui id è passato tramite il body, ma solo se il beneficiario originale ha abbastanza soldi per eseguire la nuova transazione.

ritorna l'id della transazione e il nuovo credito degli account coinvolti.

`curl -X POST -d "{\"id\": \"f0783c9e-4f27-4389-aff7-7b3d1e8f47a3\"}" localhost:8080/api/divert`

```json
{
    "079770774a0285b6e104": 0,
    "cff5b87b40e5c2b0a947": 99,
    "transaction": "229148a4-d2e9-447a-be9f-cf67b3592c96"
}
```

### Endpoint della mia estensione

#### ![][GET] /api/transaction/{transactionId}

restituisce i dettagli di una transazione passata come parametro nel path.

`curl localhost:8080/api/transaction/b3d9ec76-3b1d-4e87-8b45-2291c8643219`

```json
{
    "amount":44,
    "recipient":"d91627ab5e086f3b4b0c",
    "timestamp":"17-06-2022 12-58-44",
    "transactionId":"b3d9ec76-3b1d-4e87-8b45-2291c8643219"
}
```

 - se recipient è vuoto, la transazione era un prelievo.
 - se recipient è uguale all'accountId dell'account che la possiede, la transazione era un deposito.

### Gestione delle eccezioni

un esempio di risposta ad una richiesta fallita.
Il server imposta un codice di stato HTTP in base all'errore (400, 500, 501, 409 ...).
Quindi ritorna un JSON contenente il codice di status precedentemente settato, la sua rappresentazione in forma testuale e un messaggio che descrive la natura dell'errore.

`curl localhost:8080/kanban`

```json
{
	"message": "Unable to find requested resource",
	"error": "Not Found",
	"status": 404
}
```

### Avvertenze

Mandare richieste ![][GET] con Body non vuoto o ![][POST] con parametri url non provoca il crash del server.
Non viene restituito un errore solo perchè sono presenti informazioni in luoghi poco consoni.
Una ![][POST] con parametri url invece ma senza body restituirà un errore perchè è assente il body, il resto viene ignorato.

<p align="right">(<a href="#top">back to top</a>)</p>
