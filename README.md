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


### Help
./raindata -h

