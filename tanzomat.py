#!/usr/bin/env python3
import os

def addAccount(name, surname):
    object = str({ "name":name, "surname":surname }).replace("'", "\\\"")
    os.system(f"curl -X POST -d \"{object}\" localhost:8080/api/account")
    print()

def transfer(fromId, amount, toId):
    object = str({ "from":fromId, "amount":amount, "to":toId }).replace("'", "\\\"")
    os.system(f"curl -X POST -d \"{object}\" localhost:8080/api/transfer")
    print()

if __name__=="__main__":
    addAccount("francesco", "refolli")
    addAccount("lara", "refolli")
    transfer("9375c7b64e8326836426", 55, "fa63eb31a36b238021cd")
