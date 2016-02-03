Cinema administration and booking app using diverse Linux IPC mechanisms and file locking
==============================

*Attention: this app was implemented for Linux Operating Systems.*

#### This is a terminal app written in C that simulates a cinema administration and booking system. 

#### Two types of architectures were implemented:

##### Insecure Mode
In this architectures client processes interact directly with the DB. The DB is a simple file directory and synchronicity is controlled using file locking.

In order to run the insecure mode, clone or download the repository and then run: 
```bash
$ chmod +x makeNoIPC
```
```bash
$ ./makeNoIpc
```
After that, the executable **run** was created.
```bash
$ chmod +x run
```
```bash
$ ./run
```


##### Secure Mode
In this architectures client processes interact with workers, not with the DB. Workers are procceses created by the server to serve clients. Workers interact with de DB. For communication between worker and client, several IPC mechanisms were implemented:
* Sockets
* Message Queues
* FIFO
* Semaphores and Shared Memory
* Signals

To run the secure mode, clone or download the repository and then run:
```bash
$ chmod +x [ makeFIFO | makeQUEUE | makeSIGNALS | makeSEM_SHM | makeSOCKET ]
```
```bash
$ ./[ makeFIFO | makeQUEUE | makeSIGNALS | makeSEM_SHM | makeSOCKET ]
```
Depending on which IPC mechanism was chosen.

*For example, to use FIFO use the **makeFIFO** script.*


After that, the executables **client** and **server** were created. Then:
```bash
$ chmod +x client
```
```bash
$ chmod +x server
```
In two different terminals, run
```bash
$ ./client
```
```bash
$ ./server
```
**Attention: server must be running for make actions with clients**
#### Screenshots

![Screenshots](https://raw.githubusercontent.com/iitzco/CinemaIPC/master/img/demo.png)

![Screenshots](https://raw.githubusercontent.com/iitzco/CinemaIPC/master/img/list.png)

![Screemshots](https://raw.githubusercontent.com/iitzco/CinemaIPC/master/img/server_monitor.png)


###### This was the final project of Operating Systems course, at [ITBA](http://itba.edu.ar/).

###### Made By:
* [Ivan Itzcovich](https://github.com/iitzco)
* [Kevin Kraus](https://github.com/kevinkraus92)
* Jorge Gomez
