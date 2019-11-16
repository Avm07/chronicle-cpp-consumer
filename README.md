# chronicle-cpp-consumer

EOSChronicle cpp consumer which filter and store data in MongoDb.

# Dependencies

* Boost 1.70
* mongo-c-driver	
* mongo-cxx-driver

# Compiling

```
#Mac OSX
#brew install mongo-c
#brew install mongo-cxx-driver

git clone https://github.com/Avm07/chronicle-cpp-consumer.git
cd chronicle-cpp-consumer
git submodule update --init --recursive
mkdir build
cd build
cmake ..
# use "make -j N" for N CPU cores for faster compiling (may require more RAM)
make
```

# Configuring and running

Chronicle-cpp-consumer needs a configuratuion directory with config.json in it.
Config.json can be found in examples folder.

Config example:

```
{
    "server": {
        "port": 8686,
        "ack_number": 100,
        "bin_mode": false
    },
    "database": {
        "address": "127.0.0.1",
        "port": 27017,
        "db_name" : "database",
        "user_name": "user",
        "user_pwd": "password"
    }
}
```

Run example:

```
./chronicle-consumer ../examples config.json 
```