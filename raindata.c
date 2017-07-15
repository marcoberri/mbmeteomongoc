#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include "utils.h"

int main(int argc, char *argv[])
{

      char *conString;

      conString = "mongodb://localhost:27017/?appname=find-example";
      
     


      if (argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0)
      {
            printHelp();
            if(argc < 2){
                  printf("*** ERROR - Add one parameter\n");
            }
             return EXIT_SUCCESS;
      }

      printf("Connection server: %s\n",conString);
      mongoc_client_t *client;

      mongoc_init();

      client = mongoc_client_new(conString);

      if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "-ping") == 0)
      {

            if (!ping(client))
            {
                  printf("Ping failure on server: %s\n",conString);

                  return EXIT_FAILURE;
            }
            else
            {
                  printf("Ping complete on server:\n");

                  mongoc_client_destroy(client);
                  mongoc_cleanup();

                  return EXIT_SUCCESS;
            }
      }

      if (strcmp(argv[1], "-cr") == 0 || strcmp(argv[1], "-calRainData") == 0)
      {

            cleanCalcData(client);
            calculateRainData(client);
            mongoc_client_destroy(client);
            mongoc_cleanup();
            return EXIT_SUCCESS;
      }

      if (strcmp(argv[1], "-crb") == 0 || strcmp(argv[1], "-calRainDataBulk") == 0)
      {

            cleanCalcData(client);
            calculateRainDataBulk(client);
            mongoc_client_destroy(client);
            mongoc_cleanup();
            return EXIT_SUCCESS;
      }

      if (strcmp(argv[1], "-clean") == 0 || strcmp(argv[1], "-cleandata") == 0)
      {

            cleanCalcData(client);
            mongoc_client_destroy(client);
            mongoc_cleanup();
            return EXIT_SUCCESS;
      }

      mongoc_client_destroy(client);
      mongoc_cleanup();

      return EXIT_SUCCESS;
}