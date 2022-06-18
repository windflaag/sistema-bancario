#!/usr/bin/env python3

import requests, json, random
import multiprocessing, multiprocessing.pool

class DamonNichtProcess(multiprocessing.Process):
    @property
    def daemon(self):
        return False

    @daemon.setter
    def daemon(self, value):
        pass


class DamonNichtContext(type(multiprocessing.get_context())):
    Process = DamonNichtProcess

class Schwimmbad(multiprocessing.pool.Pool):
    def __init__(self, daemon = False, *args, **kwargs):
        if not daemon:
            kwargs['context'] = DamonNichtContext()
        super(Schwimmbad, self).__init__(*args, **kwargs)

# https://en.wikipedia.org/wiki/R4M
class Orkan:
    def generatePath(self, config):
        return "/" + "/".join([ self.generators[config[label]].functor() for label in config ])
    
    def generateQuery(self, config):
        return "&".join([ f"{label}={self.generators[config[label]].functor()}" for label in config ])
    
    def generateFragment(self, config):
        return self.generators[config].functor()
    
    def generatePayload(self, config):
        payload = {}
        
        for label in config:
            payload[label] = self.generators[config[label]].functor()
        
        return payload
    
    @staticmethod
    def assembleAddress(base, path, query, fragment):
        if query != "":
            query = "?" + query
        if fragment != "":
            fragment = "#" + fragment
        return base + path + query + fragment
    
    def __init__(self, config, generators):
        self.concurrency = config["concurrency"]
        self.identity = config["identity"]
        self.targets = config["targets"]
        self.length = config["length"]
        self.endpoint = config["entity"] + config["endpoint"]
        self.generators = generators
    
    def head(self, path = "", query = "", fragment = ""):
        address = Orkan.assembleAddress(self.endpoint, path, query, fragment)
        return requests.head(address)
    
    def get(self, path = "", query = "", fragment = ""):
        address = Orkan.assembleAddress(self.endpoint, path, query, fragment)
        return requests.get(address)
    
    def delete(self, path = "", query = "", fragment = ""):
        address = Orkan.assembleAddress(self.endpoint, path, query, fragment)
        return requests.delete(address)
    
    def post(self, payload = {}, path = "", query = "", fragment = ""):
        address = Orkan.assembleAddress(self.endpoint, path, query, fragment)
        return requests.post(address, json = payload)
    
    def patch(self, payload = {}, path = "", query = "", fragment = ""):
        address = Orkan.assembleAddress(self.endpoint, path, query, fragment)
        return requests.patch(address, json = payload)
    
    def put(self, payload = {}, path = "", query = "", fragment = ""):
        address = Orkan.assembleAddress(self.endpoint, path, query, fragment)
        return requests.put(address, json = payload)
    
    def pflasten(self, target):
        path, query, fragment, payload = "", "", "", ""
        result = {}
        
        if "path" in target:
            path = self.generatePath(target["path"])
        if "query" in target:
            query = self.generateQuery(target["query"])
        if "fragment" in target:
            fragment = self.generateFragment(target["fragment"])
        if "payload" in target:
            payload = self.generatePayload(target["payload"])
        
        if target["method"] == "HEAD":
            result = self.head(path, query, fragment)
        elif target["method"] == "GET":
            result = self.get(path, query, fragment)
        elif target["method"] == "DELETE":
            result = self.delete(path, query, fragment)
        elif target["method"] == "POST":
            result = self.post(payload, path, query, fragment)
        elif target["method"] == "PUT":
            result = self.put(payload, path, query, fragment)
        elif target["method"] == "PATCH":
            result = self.patch(payload, path, query, fragment)
        
        record = {
            "identity": self.identity,
            "method": target["method"],
            "path": path,
            "query": query,
            "fragment": fragment,
            "payload": payload,
            "status": result.status_code
        }
        
        try:
            record["reply"] = [ _ for _ in result.body.json() ]
        except:
            pass
        
        return record
    
    def angreifen(self):
        queue = [ random.choice(self.targets) for _ in range(self.length) ]
        with Schwimmbad(True, self.concurrency) as pool:
            return pool.map(self.pflasten, queue)

# https://en.wikipedia.org/wiki/Messerschmitt_Me_262
class Schwalbe:
    def __init__(self, config, generators):
        self.host = config["host"]
        self.port = config["port"]
        self.protocol = config["protocol"]
        self.concurrency = config["concurrency"]
        self.identity = config["identity"]
        
        self.entity = f"{self.protocol}://{self.host}:{self.port}"
        
        self.sperrfeuer = []
        ID = 0
        for orkan in config["orkans"]:
            orkan.update({ "entity": self.entity, "identity": f"{self.identity}_{ID}" })
            self.sperrfeuer.append(Orkan(orkan, generators))
            ID += 1
    
    def angreifen(self):
        print(f"{self.identity} does blitzkrieg")
        with Schwimmbad(False, self.concurrency) as pool:
            return pool.map(Dienstprogramm.bestellenAngreifen, self.sperrfeuer)

# https://en.wikipedia.org/wiki/Luftwaffe
class Luftwaffe:
    def __init__(self, config, generators):
        self.herde = []
        self.length = config["length"]
        
        for ID in range(self.length):
            schwalbe = config["schwalbe"]
            schwalbe.update({ "identity": f"LW{ID}" })
            self.herde.append(Schwalbe(schwalbe, generators))
        
        self.concurrency = config["concurrency"]
    
    def angreifen(self):
        print(f"Luftwaffe joins blitzkrieg")
        with Schwimmbad(False, self.concurrency) as pool:
            return pool.map(Dienstprogramm.bestellenAngreifen, self.herde)

# https://en.wikipedia.org/wiki/Wehrmacht
class Wehrmacht:
    
    def __init__(self, config, generators):
        self.luftwaffe = Luftwaffe(config["luftwaffe"], generators)
        self.concurrency = config["concurrency"]
    
    def angreifen(self):
        rustung = [ self.luftwaffe ]
        print(f"Wehrmacht starts blitzkrieg")
        with Schwimmbad(False, self.concurrency) as pool:
            return pool.map(Dienstprogramm.bestellenAngreifen, rustung)

class Generator:
    def __init__(self, config):
        self.config = config
    def functor(self):
        if self.config["type"] == "string":
            if self.config["value"] == "random":
                return "".join([random.choice(self.config["charset"]) for _ in range(self.config["length"])])

# https://de.wikipedia.org/wiki/Dienstprogramm
class Dienstprogramm:
    
    @staticmethod
    def register(record):
        print(record["identity"])
    
    @staticmethod
    def bestellenAngreifen(something):
        something.angreifen()
    
    @staticmethod
    def readFile(path):
        file = open(path, "r")
        txt = file.read()
        file.close()
        return txt
    
    @staticmethod
    def loadConfig(path):
        txt = Dienstprogramm.readFile(path)
        config = json.loads(txt)
        return config
    
    @staticmethod
    def makeGenerator(config):
        return Generator(config)
    
    @staticmethod
    def makeGenerators(config):
        generators = {}
        for generator in config["generators"]:
            generators[generator] = Dienstprogramm.makeGenerator(config["generators"][generator])
        
        return generators

if __name__=="__main__":
    config = Dienstprogramm.loadConfig("config.json")
    generators = Dienstprogramm.makeGenerators(config)
    
    wehrmacht = Wehrmacht(config["wehrmacht"], generators)
    print(wehrmacht.angreifen())