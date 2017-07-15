#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

void printHelp()
{
    printf("Welcome to Rain Calculation Utility\n");
    printf("marcoberri@gmail.com - http://tecnicume.blogspot.it  - 2017\n");
    printf("-h or -help                This...\n");
    printf("-p or -ping                Ping server\n");
    printf("-cr or -calRainData        Calculate rain data\n");
    printf("-crb or -calRainDataBulk   Calculate rain data Bulk mode\n");
    printf("-clean or -cleandata       Clean data\n");
}

//funzione per il ping
bool ping(mongoc_client_t *client)
{
    bson_t *command, reply;
    bool retval;
    bson_error_t error;
    char *str;

    command = BCON_NEW("ping", BCON_INT32(1));
    retval = mongoc_client_command_simple(client, "admin", command, NULL, &reply, &error);

    if (!retval)
    {
        fprintf(stderr, "*** ERROR - %s\n", error.message);
        return retval;
    }

    str = bson_as_json(&reply, NULL);
    printf("%s\n", str);

    return retval;
}

//funzione per il salvataggio del dato
bool upsert(int actual, time_t ts, mongoc_collection_t *collection)
{
    bson_error_t error;
    bson_t *doc;
    doc = bson_new();
    BSON_APPEND_INT32(doc, "RC", actual);
    BSON_APPEND_TIME_T(doc, "ts", ts);

    if (!mongoc_collection_insert(collection, MONGOC_INSERT_NONE, doc, NULL, &error))
    {
        fprintf(stderr, "%s\n", error.message);
        return false;
    }
    bson_destroy(doc);
    return true;
}

bool cleanCalcData(mongoc_client_t *client)
{
    mongoc_collection_t *collection;
    bson_error_t error;

    collection = mongoc_client_get_collection(client, "mbmeteolacrosse", "raindata");

    if (!mongoc_collection_drop(collection, &error))
    {
        fprintf(stderr, "%s\n", error.message);
        return false;
    }

    mongoc_collection_destroy(collection);
    return true;
}

bool calculateRainData(mongoc_client_t *client)
{

    mongoc_collection_t *collection;
    mongoc_collection_t *collectionWrite;

    mongoc_cursor_t *cursor;

    const bson_t *doc;
    bson_t *query;
    bson_t *insert;
    bson_t *opts;

    bson_iter_t iter;

    collection = mongoc_client_get_collection(client, "mbmeteolacrosse", "rawdata");

    collectionWrite = mongoc_client_get_collection(client, "mbmeteolacrosse", "raindata");

    opts = BCON_NEW(
        "sort", "{", "ts", BCON_INT32(1), "}", "projection", "{",
        "RC", BCON_BOOL(true),
        "ts", BCON_BOOL(true),
        "}");

    query = bson_new();
    cursor = mongoc_collection_find_with_opts(collection, query, opts, NULL);
    int prev = -1;
    int actual = 0;
    time_t ts;
    while (mongoc_cursor_next(cursor, &doc))
    {
        if (prev != -1)
        {

            if (!upsert(actual, ts, collectionWrite))
            {

                return EXIT_FAILURE;
            }
        }

        if (bson_iter_init(&iter, doc))
        {
            while (bson_iter_next(&iter))
            {
                char *f = "RC";
                if (strcmp(bson_iter_key(&iter), f) == 0)
                {
                    int cur = bson_iter_int32(&iter);
                    if (prev == -1)
                        prev = cur;
                    else if (cur <= prev)
                        prev = cur;

                    actual = cur - prev;

                    prev = cur;
                }

                f = "ts";
                if (strcmp(bson_iter_key(&iter), f) == 0)
                {
                    ts = bson_iter_time_t(&iter);
                }
            }
        }
    }

    bson_destroy(query);
    bson_destroy(opts);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_collection_destroy(collectionWrite);
}

bool calculateRainDataBulk(mongoc_client_t *client)
{

    mongoc_collection_t *collection;
    mongoc_collection_t *collectionWrite;
    mongoc_bulk_operation_t *bulk;

    mongoc_cursor_t *cursor;

    const bson_t *doc;
    bson_t *query;
    bson_t *insert;
    bson_t *opts;
    bson_t *docBulk;
    bson_t reply;

    bson_error_t error;

    bson_iter_t iter;

    char *str;
    bool ret;

    collection = mongoc_client_get_collection(client, "mbmeteolacrosse", "rawdata");

    collectionWrite = mongoc_client_get_collection(client, "mbmeteolacrosse", "raindata");

    bulk = mongoc_collection_create_bulk_operation(collectionWrite, true, NULL);

    opts = BCON_NEW(
        "sort", "{", "ts", BCON_INT32(1), "}", "projection", "{",
        "RC", BCON_BOOL(true),
        "ts", BCON_BOOL(true),
        "}");

    query = bson_new();
    cursor = mongoc_collection_find_with_opts(collection, query, opts, NULL);
    int prev = -1;
    int actual = 0;
    time_t ts;
    while (mongoc_cursor_next(cursor, &doc))
    {
        if (prev != -1)
        {
            docBulk = bson_new();
            BSON_APPEND_INT32(docBulk, "RC", actual);
            BSON_APPEND_TIME_T(docBulk, "ts", ts);

            mongoc_bulk_operation_insert(bulk, docBulk);
            bson_destroy(docBulk);
        }

        if (bson_iter_init(&iter, doc))
        {
            while (bson_iter_next(&iter))
            {
                char *f = "RC";
                if (strcmp(bson_iter_key(&iter), f) == 0)
                {
                    int cur = bson_iter_int32(&iter);
                    if (prev == -1)
                        prev = cur;
                    else if (cur <= prev)
                        prev = cur;

                    actual = cur - prev;

                    prev = cur;
                }

                f = "ts";
                if (strcmp(bson_iter_key(&iter), f) == 0)
                {
                    ts = bson_iter_time_t(&iter);
                }
            }
        }
    }

    ret = mongoc_bulk_operation_execute(bulk, &reply, &error);
    str = bson_as_json(&reply, NULL);
    printf("%s\n", str);
    bson_free(str);

    if (!ret)
    {
        fprintf(stderr, "Error: %s\n", error.message);
    }

    bson_destroy(query);
    bson_destroy(opts);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_collection_destroy(collectionWrite);
}
