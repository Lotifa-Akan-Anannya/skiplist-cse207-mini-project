//including all required libraries
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define SKIPLIST_MAX_LEVEL 6

//defining a structure snode
typedef struct snode {
    //variable to store value of key
    int key;
    //variable to store value for specific key
    int value;
    //variable to store address of next variable
    struct snode **forward;
} snode;

//defining structure skiplist
typedef struct skiplist {
    //variable to store level of skiplist
    int level;
    //variable to store size of skiplist
    int size;
    //variable to store address of header
    struct snode *header;
} skiplist;

//function to initialize skiplist
skiplist *skiplist_init(skiplist *list) {
    int i;
    //allocating size of single node and store it in the variable header
    snode *header = (snode *) malloc(sizeof(struct snode));
    //assign it to the header of list
    list->header = header;
    //set header key as INT_MAX
    header->key = INT_MAX;
    //initialize forward of snode
    header->forward = (snode **) malloc(
            sizeof(snode*) * (SKIPLIST_MAX_LEVEL + 1));
    //loop to initialize header of each node of skiplist
    for (i = 0; i <= SKIPLIST_MAX_LEVEL; i++) {
        header->forward[i] = list->header;
    }

    //setting level and size of list
    list->level = 1;
    list->size = 0;

    return list;
}

//function to set random level of the list
static int rand_level() {
    int level = 1;
    //loop which will select random value less than RAND_MAX() and it will be always less than SKIPLIST_MAX_LEVEL
    while (rand() < RAND_MAX / 2 && level < SKIPLIST_MAX_LEVEL)
        level++;
    return level;
}


//function to insert node to skiplist
int skiplist_insert(skiplist *list, int key, int value) {
    //temporary variables to be used to insert values
    snode *update[SKIPLIST_MAX_LEVEL + 1];
    //store header of list into one temporary variable
    snode *x = list->header;
    int i, level;
    //loop to copy all elements of list to update array
    for (i = list->level; i >= 1; i--) {
        while (x->forward[i]->key < key)
            x = x->forward[i];
        update[i] = x;
    }
    //reinitialize x
    x = x->forward[1];

    //if key already exists in the list then exit
    if (key == x->key) {
        x->value = value;
        return 0;
    } else {
        //select random level
        level = rand_level();
        if (level > list->level) {
            //loop which will update the update array by storing list header into each index
            for (i = list->level + 1; i <= level; i++) {
                update[i] = list->header;
            }
            //update list level
            list->level = level;
        }

//reset x
        x = (snode *) malloc(sizeof(snode));
        //set key for x
        x->key = key;
        //set value of x
        x->value = value;
        //loop to set up forward of x
        x->forward = (snode **) malloc(sizeof(snode*) * (level + 1));
        for (i = 1; i <= level; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
    //end of function
    return 0;
}


//function to search key in the list
snode *skiplist_search(skiplist *list, int key) {
    //storing header of list into temporary variable
    snode *x = list->header;
    int i;
    //loop which will compare each key till the given key is less than of it
    for (i = list->level; i >= 1; i--) {
        while (x->forward[i]->key < key)
            x = x->forward[i];
    }
    //now we will check one forward to be equal of key
    //it it is return that node otherwise return NULL
    if (x->forward[1]->key == key) {
        return x->forward[1];
    } else {
        return NULL;
    }
    return NULL;
}

//freeing up memory used by a node
static void skiplist_node_free(snode *x) {
    //getting address of node
    //and using free function releasing a memory
    if (x) {
        free(x->forward);
        free(x);
    }
}


//function to delete a node
//this function works same as insert function
//just a difference of last part which deletes the element
int skiplist_delete(skiplist *list, int key) {
    int i;
    snode *update[SKIPLIST_MAX_LEVEL + 1];
    snode *x = list->header;
    for (i = list->level; i >= 1; i--) {
        while (x->forward[i]->key < key)
            x = x->forward[i];
        update[i] = x;
    }

    x = x->forward[1];
    if (x->key == key) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->forward[i] != x)
                break;
            update[i]->forward[1] = x->forward[i];
        }
        skiplist_node_free(x);

        while (list->level > 1 && list->header->forward[list->level]
                == list->header)
            list->level--;
        return 0;
    }
    return 1;
}

//function to print list
static void skiplist_dump(skiplist *list) {
    snode *x = list->header;
    while (x && x->forward[1] != list->header) {
        printf("%d[%d]->", x->forward[1]->key, x->forward[1]->value);
        x = x->forward[1];
    }
    printf("NIL\n");
}


int main() {

    //creating list
    skiplist list;
    //initializing list
    skiplist_init(&list);
    int n = 0;

    while(n != 5){
        printf("1) Insert an element\n");
        printf("2) Search an element\n");
        printf("3) Delete an element\n");
        printf("4) Print the list\n");
        printf("5) Exit\n");
        printf("Enter Choice: ");
        scanf("%d",&n);
        if(n==1){
            int value = 0;
            printf("Enter Value : ");
            scanf("%d",&value);
            /*
            int key=0;
            //uncomment this if you want to read key as user input
            printf("Enter Key : ");
            scanf("%d",&key);
            */
            //replace first value with key if you want key as user input
            skiplist_insert(&list,value,value);
        }else if(n==2){
            int key =0;
            printf("Enter Key : ");
            scanf("%d",&key);
            snode *x = skiplist_search(&list, key);
            if (x) {
                printf("key = %d, value = %d\n", key, x->value);
            } else {
                printf("key = %d, not fuound\n", key);
            }
        }else if(n==3){
            int key =0;
            printf("Enter Key : ");
            scanf("%d",&key);
            skiplist_delete(&list, key);
        }else if(n==4){
            skiplist_dump(&list);
        }
    }

    return 0;
}
