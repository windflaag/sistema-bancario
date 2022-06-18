#!/usr/bin/env python3

import json, time, string, csv, multiprocessing, requests, random

def random_name():
    return "".join([ random.choice(string.ascii_lowercase + string.ascii_uppercase + ". ") for _ in range(random.randint(0, 80)) ])

def bombardt(METHOD, URL, PAYLOAD = None):
    if METHOD == "HEAD":
        return requests.head(URL)
    elif METHOD == "GET":
        return requests.get(URL)
    elif METHOD == "POST":
        return requests.post(URL, json = PAYLOAD)
    elif METHOD == "PUT":
        return requests.put(URL, json = PAYLOAD)
    elif METHOD == "DELETE":
        return requests.delete(URL)
    elif METHOD == "PATCH":
        return requests.patch(URL, json = PAYLOAD)

HEADER = ["id", "method", "url", "payload", "code", "status", "reply"]
def dekorat(METHOD, URL, PAYLOAD = None):
    try:
        data = bombardt(METHOD, URL, PAYLOAD)
        return {
                    "method": METHOD,
                    "url": URL,
                    "payload": PAYLOAD,
                    "code": 0,
                    "status": data.status_code,
                    "reply": data.json()
                }
    except:
        return {
                    "method": METHOD,
                    "url": URL,
                    "payload": PAYLOAD,
                    "code": 1
                }

def Kontoregistrierung(ID):
    data = dekorat("POST", "http://localhost:8080/api/account", {
        "name": random_name(),
        "surname": random_name()
        })
    data["id"] = ID
    print(f"| Achtung | blitzkrieg#{ID}                 |", end="\r")
    return data

def score(data):
    return (len(data) - sum([ _["code"] for _ in data ])) / len(data)

def deposit(data, header = HEADER, path = "result.csv"):
    with open(path, 'w') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames = header)
        writer.writeheader()
        writer.writerows(data)

    
def readFile(path):
    file = open(path, "r")
    txt = file.read()
    file.close()
    return txt

def loadConfig(path):
    txt = readFile(path)
    config = json.loads(txt)
    return config
 

if __name__=="__main__":
    config = loadConfig("tanzomat.json")
    IDs = range(config["length"])

    pool = multiprocessing.Pool(config["concurrency"])
    START = time.time()
    data = pool.map(Kontoregistrierung, IDs)
    END = time.time()
    pool.close()

    deposit(data)
    print()
    print(f"score = {score(data)}, elapsed = {END - START} seconds!!")
