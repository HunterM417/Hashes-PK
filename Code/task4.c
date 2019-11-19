#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

void get_hash(char * hashname, char *msg, unsigned char *md_value)
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    int md_len, i;
    OpenSSL_add_all_digests();
    md = EVP_get_digestbyname(hashname);
    if(!md) {
        printf("Unknown message digest %s\n", hashname);
        exit(1);
    }
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, msg, strlen(msg));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);
}

void get_rand(char *msg)
{
    int i;
    for (i = 0; i < 24; i++)
    msg[i] = rand()%256-128;
}

int one_way_hash(char * hashname)
{
    char msg1[24], msg2[24];
    unsigned char digt1[24], digt2[24];
    int count=0, i;
    get_rand(msg1);
    get_hash(hashname, msg1, digt1);
    do
    {
        get_rand(msg2);
        get_hash(hashname, msg2, digt2);
        count++;
    } while (strcmp(digt1, digt2) != 0);
    return count;
}

int collision_hash(char * hashname)
{
    char msg1[24], msg2[24];
    unsigned char digt1[24], digt2[24];
    int count=0, i;
    do
    {
        get_rand(msg1);
        get_hash(hashname, msg1, digt1);
        get_rand(msg2);
        get_hash(hashname, msg2, digt2);
        count++;
    } while (strcmp(digt1, digt2) != 0);
    return count;
}

int main(int argc, char *argv[])
{
    char *hashname;
    int trials_collision = 0;
    int trials_one_way = 0;
    if(!argv[1])
    {
        printf("Usage: ./hash [hash_name] [# of Collision-Free trials] [# of One-Way Collision trials]\n");
        exit(0);
    }
    else
    {
        hashname = argv[1];
    }

    if(!argv[2])
    {
        printf("Usage: ./hash [hash_name] [# of Collision-Free trials] [# of One-Way Collision trials]\n");
        exit(0);
    }
    else
    {
        trials_collision = atoi(argv[2]);
    }

    if(!argv[3])
    {
        printf("Usage: ./hash [hash_name] [# of Collision-Free trials] [# of One-Way Collision trials]\n");
        exit(0);
    }
    else
    {
        trials_one_way = atoi(argv[3]);
    }


    srand((int)time(0));
    int i,count;

    printf("Collision-Free Test\n====================\n");
    for (i=0,count=0; i < trials_collision; i++)
    {
        int trial = collision_hash(hashname);
        printf("Trial %d: %d tries.\n", i+1, trial);
        count += trial;
    }
    printf("Average across %d trials: %d \n", trials_collision, count / trials_collision);

    printf("\n\nOne-Way Test\n====================\n");
    for (i=0,count=0; i < trials_one_way; i++)
    {
        int trial = one_way_hash(hashname);
        printf("Trial %d: %d tries.\n", i+1, trial);
        count += trial;
    }
    printf("Average across %d trials: %d \n", trials_collision, count / trials_one_way);

    return 0;
}
