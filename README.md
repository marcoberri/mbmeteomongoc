# MeteoMongoC

Applicatione basata sui driver mongoc per eseguire dei calcoli sulle statistiche di http://meteo.marcoberri.it per quanto riguarda la parte delle precipitazione 


### Installazione

Eseguito viluppo su Ubuntu 17.04 pre procedere alla configurazione dei driver C

```
sudo apt-get install libmongoc-1.0-0
```

Per eseguire la compilazione delle classi
```
sudo gcc  -o raindata raindata.c utils.c $(pkg-config --cflags --libs libmongoc-1.0)
```

### Esempio di performance

Si lavora su una collection di circa 1M di elementi, molto interessate la differenza di performance tra la funzionalità di insert standard e quella di bulk.

![alt text](https://raw.githubusercontent.com/marcoberri/mbmeteomongoc/master/resources/source.png)

![alt text](https://raw.githubusercontent.com/marcoberri/mbmeteomongoc/master/resources/count.png)


![alt text](https://raw.githubusercontent.com/marcoberri/mbmeteomongoc/master/resources/execute.png)




### Help
./raindata -h

