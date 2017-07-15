#Installazione su Ubuntu/Debian
sudo apt-get install libmongoc-1.0-0

#Comando di compilazione
sudo gcc  -o raindata raindata.c utils.c $(pkg-config --cflags --libs libmongoc-1.0)

#Help
./raindata -h
