#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node node;
typedef struct HashTable HashTable;

/*Δομή Κόμβου Εγγραφής Φοιτητή*/
struct node {
    int AM, key;
    char namesirname[100];
    int marker; /*Η μεταβλητή marker γίνεται 1 όταν η εγγραφή εισάγεται και -1 όταν δεν υφίσταται ή διαγράφεται*/
};
/*Ο Πίνακας HashTable αποτελείται από buckets, κάθε ένα από τα οποία είναι κόμβος (node) εγγραφής φοιτητών*/
struct HashTable {
    int tableSz;
    int totElements;
    node *hashBucket;
};
void insertIntoHashTable(int key, char *namesirname, int AM); /* Συνάρτηση ένθεσης εγγραφής φοιτητή στο HashTable*/
void deleteFromHashTable(int key); /* Συνάρτηση διαγραφής εγγραφής φοιτητή από το HashTable*/
void searchData(int key); /* Συνάρτηση εύρεσης εγγραφής φοιτητή στο HashTable*/
void display(); /* Εκτύπωση των εγγραφών που έχουν αποθηκευτεί στα buckets του πίνακα κατακερματισμού */
void HashTable_Init(HashTable *);

HashTable *ht; /* Μεταβλητή δείκτη στον πίνακα κατακερματισμού */

int main() {
/* Συνάρτηση εύρεσης εγγραφής φοιτητή στο HashTable*/
    int key, AM, ch, i, flag = 0;
    char namesirname[100];
    ht = (HashTable *)malloc(sizeof(HashTable));

   /* Αρχικοποίηση του Πίνακα Κατακερματισμού */
    HashTable_Init(ht);
    printf("Enter the no of elements:");
    scanf("%d", &ht->tableSz); /* Διαβάζουμε το μήκος του πίνακα κατακερματισμού */
    /* Δεσμεύουμε τον κατάλληλο χώρο μνήμης που απαιτείται για ένα bucket, δηλαδή για ένα κόμβο εγγραφής φοιτητή*/
    ht->hashBucket = (struct node *)malloc((ht->tableSz)*sizeof(struct node));
    /* Ακολοθεί το ΜΕΝΟΥ της εφαρμογής*/
    while (1) {
        printf("1. Insertion\n2. Deletion\n");
        printf("3. Searching\n4. Display \n");
        printf("5. Exit\n\nEnter your choice: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1:
            printf("Enter the key value: ");
            scanf("%d", &key);
            getchar();
            printf("Name and Sirname:");
            fgets(namesirname, 100, stdin);
            namesirname[strlen(namesirname) - 1] = '\0';
            printf("AM:");
            scanf("%d", &AM);
            insertIntoHashTable(key, namesirname, AM);
            break;
            case 2:
            printf("Enter the key value: ");
            scanf("%d", &key);
            deleteFromHashTable(key);
            break;
            case 3:
            printf("Enter the key value: ");
            scanf("%d", &key);
            searchData(key);
            break;
            case 4:
            display();
            break;
            case 5:
            exit(0);
            default:
            printf("You have entered wrong Option!!\n");
            break;
        }
    }
    return 0;
}
/* Ένθεση στο HashTable του φοιτητή με ονοματεπόωυμο και ΑΜ, βάσει του κλειδιού key */
void insertIntoHashTable(int key, char *namesirname, int AM)
{
    int i=1;
    if (ht->tableSz == ht->totElements) {
        printf("Can't perform Insertion..Hash Table is full!!\n");
        return;
    }

    /* 1st hashing - finding hash index */
    int hashInd1 = key % ht->tableSz, hashVal, hashInd;
    /* 2nd hashing */
    hashVal = (key / ht->tableSz) % ht->tableSz;

    hashInd=hashInd1;

    /* linear probing: Ψάξε στο HashTable τη σωστή θέση ένθεσης,
    αρχικά με τον πρώτο hash-index και εν συνεχεία κάνοντας linear probing με τη βοήθεια της δεύτερης hash συνάρτησης */
    while (ht->hashBucket[hashInd].marker == 1) {
        hashInd = (hashInd1 + i*hashVal)%ht->tableSz;
        ++i;
    }
    ht->hashBucket[hashInd].key = key;
    ht->hashBucket[hashInd].AM = AM;
    strcpy(ht->hashBucket[hashInd].namesirname, namesirname);
    ht->hashBucket[hashInd].marker = 1;
    ht->totElements++;
    return;
  }
/* Διαγραφή από το HashTable του φοιτητή, βάσει του κλειδιού key της σχετικής εγγραφής που είναι αποθηκευμένη στο bucket_node*/
void deleteFromHashTable(int key) {
    if (ht->totElements == 0) {
        printf("Hash Table is Empty!!n");
        return;
    }

    int hashInd1 = key % ht->tableSz, count = 0, flag = 0, hashVal, hashInd;
    int i=1;
    hashVal = (key / ht->tableSz) % ht->tableSz;
    hashInd=hashInd1;
    /* Ψάξε στο HashTable τη σωστή θέση διαγραφής, αρχικά με τον πρώτο hash-index και εν συνεχεία κάνοντας linear probing με τη βοήθεια
    της δεύτερης hash συνάρτησης*/
    while (ht->hashBucket[hashInd].marker != 0 && count <= ht->tableSz) {
        if (ht->hashBucket[hashInd].key == key) {
            ht->hashBucket[hashInd].key = 0;
            ht->hashBucket[hashInd].marker = -1;
            ht->hashBucket[hashInd].AM = 0;
            strcpy(ht->hashBucket[hashInd].namesirname, "");
            ht->totElements--;
            flag = 1;
        }
        hashInd = (hashInd1 + i*hashVal)%ht->tableSz;
        ++i;
        count++;
    }

    if (flag)
    printf("Given data deleted from Hash Table\n");
    else
    printf("Given data is not available in Hash Table\n");

    return;
}

/* Συνάρτηση εύρεσης εγγραφής φοιτητή στο HashTable, βάσει του κλειδιού της εγγραφής*/
void searchData(int key) {
    if (ht->totElements == 0) {
        printf("Hash Table is Empty!!");
        return;
    }

    int i=1;
    int hashInd1 = key % ht->tableSz, flag = 0, count = 0, hashVal = 0, hashInd;
    hashVal = (key / ht->tableSz) % ht->tableSz;
    hashInd=hashInd1;
/* Ψάξε στο HashTable τη σωστή θέση προσπέλασης, αρχικά με τον πρώτο hash-index και εν συνεχεία κάνοντας linear probing με τη βοήθεια
    της δεύτερης hash συνάρτησης*/
    while (ht->hashBucket[hashInd].marker != 0 && count <= ht->tableSz) {
        if (ht->hashBucket[hashInd].key == key) {
            printf("Student ID : %d\n", ht->hashBucket[hashInd].key);
            printf("Name : %s\n", ht->hashBucket[hashInd].namesirname);
            printf("AM  : %d\n", ht->hashBucket[hashInd].AM);
            flag = 1;
        }
        hashInd = (hashInd1 + i*hashVal) % ht->tableSz;
        ++i;
    }

    if (!flag)
    printf("Given data is not present in hash table\n");
    return;
}
/* Εκτύπωση των εγγραφών που έχουν αποθηκευτεί στα buckets του πίνακα κατακερματισμού */
void display() {
    int i;

    if (ht->totElements == 0) {
        printf("Hash Table is Empty!!n");
        return;
    }
    printf("Student ID Name AM Index \n");
    printf("-----------------------------------------\n");
    for (i = 0; i < ht->tableSz; i++) {
        if (ht->hashBucket[i].marker == 1) {
            printf("%-13d", ht->hashBucket[i].key);
            printf("%-15s", ht->hashBucket[i].namesirname);
            printf("%-7d", ht->hashBucket[i].AM);
            printf("%d\n", i);
        }
    }
    printf("\n");
    return;
}
/* Αρχικοποίηση του HashTable*/
void HashTable_Init(HashTable *table){
    table->hashBucket = (node *)malloc(sizeof(node));
    table->tableSz = 0;
    table->totElements = 0;
}
