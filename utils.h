
bool ping(mongoc_client_t *client);

bool upsert(int actual, time_t ts, mongoc_collection_t *collectionWrite);

bool calculateRainData(mongoc_client_t *client);

bool calculateRainDataBulk(mongoc_client_t *client);

bool cleanCalcData(mongoc_client_t *client);

void printHelp();

