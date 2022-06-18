#!/usr/bin/env python3

import multiprocessing, requests, random

def bombardt(METHOD, URL, PAYLOAD = None):
    try:
        if METHOD == "HEAD":
            return { "URL": URL, "code": 0, "reply": requests.head(URL) }
        elif METHOD == "GET":
            return { "URL": URL, "code": 0, "reply": requests.get(URL) }
        elif METHOD == "POST":
            return { "URL": URL, "code": 0, "reply": requests.post(URL, json = PAYLOAD) }
        elif METHOD == "PUT":
            return { "URL": URL, "code": 0, "reply": requests.put(URL, json = PAYLOAD) }
        elif METHOD == "DELETE":
            return { "URL": URL, "code": 0, "reply": requests.delete(URL) }
        elif METHOD == "PATCH":
            return { "URL": URL, "code": 0, "reply": requests.patch(URL, json = PAYLOAD) }
    except Exception as ERR:
        print(ERR)
        return { "URL": URL, "code": 1, "reply": None }

def Kontoregistrierung(ID):
    data = bombardt("POST", "http://localhost:8080/api/account", {})
    data["ID"] = ID
    return data

def score(data):
    return len(data) - sum([ _["code"] for _ in data ])

if __name__=="__main__":
    IDs = range(1000)

    pool = multiprocessing.Pool(17)
    data = pool.map(Kontoregistrierung, IDs)
    pool.close()

    print(f"score = {score(data)}")
