#!/usr/bin/env python3
import os, json, string
import requests, random

NAME_ALPHABET = string.ascii_lowercase + string.ascii_uppercase
def random_name(LENGTH = 20):
    return "".join([ random.choice(NAME_ALPHABET) for _ in range(LENGTH) ])

class API:
    def __init__(self, endpoint):
        self.endpoint = endpoint
    def at(self, path="/", query="", fragment=""):
        URI = self.endpoint + path
        if query != "":
            URI += "?" + query
        if fragment != "":
            URI += "#" + fragment 
        return URI

class Tanzomat:
    def __init__(self, api):
        self.api = api
     
    def head(self, URL):
        return requests.head(URL)
   
    def get(self, URL):
        return requests.get(URL)

    def put(self, URL, payload):
        return requests.put(URL, json=payload)

    def patch(self, URL, payload):
        return requests.patch(URL, json=payload)

    def post(self, URL, payload):
        return requests.post(URL, json=payload)

    def delete(self, URL):
        return requests.delete(URL)

    def getAccountList(self):
        response = self.get(URL = self.api.at("/api/account"))
        if response.status_code == 200:
            return response.json()["accounts"]
        else:
            raise ValueError(f"getAccountList request returned {response.status_code}")
    
    def addAccount(self, name, surname):
        response = self.post(URL = self.api.at("/api/account"), payload = { "name": name, "surname": surname })
        if response.status_code == 201:
            return response.text
        else:
            raise ValueError(f"addAccount({name}, {surname}) request returned {response.status_code}")

    def deleteAccount(self, accountId):
        response = self.delete(URL = self.api.at("/api/account", f"id={accountId}"))
        if response.status_code == 200:
            return True
        else:
            raise ValueError(f"deleteAccount({accountId}) request returned {response.status_code}")
    
    def transfer(self, from_, amount_, to_):
        response = self.post(URL = self.api.at("/api/transfer"), payload = { "from": from_, "amount": amount_, "to": to_ })
        if response.status_code == 201:
            return response.text
        else:
            raise ValueError(f"transfer({from_}, {amount_}, {to_}) request returned {response.status_code}")

    def headAccountDetails(self, accountId):
        response = self.head(URL = self.api.at(f"/api/account/{accountId}"))
        if response.status_code == 200:
            return response.headers['x-sistema-bancario']
        else:
            raise ValueError(f"getAccountDetails({accountId}) request returned {response.status_code}") 

    def getAccountDetails(self, accountId):
        response = self.get(URL = self.api.at(f"/api/account/{accountId}"))
        if response.status_code == 200:
            return response.text
        else:
            raise ValueError(f"getAccountDetails({accountId}) request returned {response.status_code}")
 
    def payment(self, amount_, to_):
        response = self.post(URL = self.api.at(f"/api/account/{to_}"), payload = { "amount":amount_ })
        if response.status_code == 201:
            return response.text
        else:
            raise ValueError(f"payment({amount_}, {to_}) request returned {response.status_code}")
 
    def withdraw(self, amount_, from_):
        response = self.post(URL = self.api.at(f"/api/account/{from_}"), payload = { "amount": -amount_ })
        if response.status_code == 201:
            return response.text
        else:
            raise ValueError(f"withdraw({amount_}, {to_}) request returned {response.status_code}")

    def reName(self, accountId, name):
        response = self.patch(URL = self.api.at(f"/api/account/{accountId}"), payload = { "name": name })
        if response.status_code == 200:
            return response.text
        else:
            raise ValueError(f"reName({accountId}, {name}) request returned {response.status_code}")


    def reSurname(self, accountId, surname):
        response = self.patch(URL = self.api.at(f"/api/account/{accountId}"), payload = { "surname": surname })
        if response.status_code == 200:
            return response.text
        else:
            raise ValueError(f"reSurname({accountId}, {surname}) request returned {response.status_code}")


    def reNameAndSurname(self, accountId, name, surname):
        response = self.put(URL = self.api.at(f"/api/account/{accountId}"), payload = { "name": name, "surname": surname })
        if response.status_code == 200:
            return response.text
        else:
            raise ValueError(f"reNameAndSurname({accountId}, {name}, {surname}) request returned {response.status_code}")

 
def testApi(SIZE = 7):
    tanzomat = Tanzomat(api = API("http://localhost:8080"))
    accounts = tanzomat.getAccountList()
    print(f"accounts: {accounts}")

    for _ in range(0, SIZE):
        tanzomat.addAccount(f"{random_name()}", f"{random_name()}")
    accounts = tanzomat.getAccountList();
    print(f"accounts: {accounts}")

    print()

    tanzomat.payment(3 * len(accounts[1:]), accounts[0])
    for destination in accounts[1:]:
        tanzomat.transfer(accounts[0], 2, destination)

    print()
    
    for accountId in accounts:
        print(f"<{accountId}> = {tanzomat.getAccountDetails(accountId)}")
        tanzomat.reName(accountId, random_name())
        tanzomat.reSurname(accountId, random_name())
        print(f"<{accountId}> = {tanzomat.headAccountDetails(accountId)}")
        tanzomat.reNameAndSurname(accountId, random_name(), random_name())
        print(f"<{accountId}> = {tanzomat.headAccountDetails(accountId)}")

    print()

    for accountId in accounts:
        tanzomat.deleteAccount(accountId)
    accounts = tanzomat.getAccountList();
    print(f"accounts: {accounts}")

if __name__=="__main__":
    testApi(17)
