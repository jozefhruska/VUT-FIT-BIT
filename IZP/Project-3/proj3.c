/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX


// Redefine EXIT MACROS
#undef EXIT_FAILURE
#undef EXIT_SUCCESS
#define EXIT_FAILURE    -1
#define EXIT_SUCCESS    1

// Define error messages
#define ERROR_BASE "Error: "
#define ERROR_BAD_FILE ERROR_BASE "File does not exist, is not readable, damaged or badly formatted.\n"
#define ERROR_BAD_ARGUMENTS ERROR_BASE "Arguments are badly formatted or missing.\n"
#define ERROR_ITEMS ERROR_BASE "Less items than expected or badly formatted.\n"
#define ERROR_UNIQUE_IDS ERROR_BASE "Item IDs are not unique.\n"

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap) {
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(sizeof(struct obj_t) * cap);
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c) {
    c->capacity = 0;
    c->size = 0;
    free(c->obj);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t *) arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj) {
    if (c->capacity > c->size) {
        c->obj[c->size] = obj;
        c->size++;
    } else {
        c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
        c->obj[c->size] = obj;
        c->size++;
    }
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i < c2->size; i++) {
        append_cluster(c1, c2->obj[i]);
    }

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) {
    assert(idx < narr);
    assert(narr > 0);

    // Remove given cluster from cluster array
    clear_cluster(&carr[idx]);

    // Regorganize cluster array
    for (int i = idx; i < narr - 1; i++) {
        carr[i].capacity = carr[i + 1].capacity;
        carr[i].size = carr[i + 1].size;
        carr[i].obj = carr[i + 1].obj;
    }

    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2) {
    assert(o1 != NULL);
    assert(o2 != NULL);

    return sqrtf(pow(o1->x - o2->x, 2) + pow(o1->y - o2->y, 2));
}

/**
 * Calculates centroid of given cluster
 * @param cluster Two-dimensional cluster
 * @return Centroid of given cluster (obj_t)
 */
struct obj_t getCentroid(struct cluster_t *cluster) {
    float sumX = 0;
    float sumY = 0;

    for (int i = 0; i < cluster->size; i++) {
        sumX += cluster->obj[i].x;
        sumY += cluster->obj[i].y;
    }

    struct obj_t centroid = {0, sumX / cluster->size, sumY / cluster->size};
    return centroid;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    struct obj_t c1_Centroid = getCentroid(c1);
    struct obj_t c2_Centroid = getCentroid(c2);

    return obj_distance(&c1_Centroid, &c2_Centroid);
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {
    assert(narr > 0);

    float lowestDistance = INT_MAX;
    float lastDistance;
    int c1_Index, c2_Index;

    for (int a = 0; a < narr; a++) {
        for (int b = a + 1; b < narr; b++) {
            lastDistance = cluster_distance(&carr[a], &carr[b]);
            if (lastDistance < lowestDistance) {
                lowestDistance = lastDistance;
                c1_Index = a;
                c2_Index = b;
            }
        }
    }

    *c1 = c1_Index;
    *c2 = c2_Index;
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b) {
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *) a;
    const struct obj_t *o2 = (const struct obj_t *) b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++) {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr) {
    assert(arr != NULL);

    FILE *file;
    int itemsCount;

    // Check if file can be opened, if not fail
    if ((file = fopen(filename, "r")) == NULL) {
        *arr = NULL;
        printf(ERROR_BAD_FILE);
        return (EXIT_FAILURE);
    }

    // Check if items count is specified, if not fail
    if (fscanf(file, "count=%d", &itemsCount) != 1) {
        fclose(file);
        printf(ERROR_BAD_FILE);
        return (EXIT_FAILURE);
    }

    // Allocate memory for all items
    *arr = malloc(sizeof(struct cluster_t) * itemsCount);

    // Iterate through items
    for (int i = 0; i < itemsCount; i++) {
        int id, x, y;

        // Check if we won't run out of items before itemsCount is reached
        if (fscanf(file, "%d %d %d", &id, &x, &y) == EOF) {
            fclose(file);

            return (EXIT_FAILURE);
        }

        // Check if both X & Y are within bounds
        if (x < 0 || y < 0 || x > 1000 || y > 1000) {
            fclose(file);
            return (EXIT_FAILURE);
        }

        // Check if every item has an unique ID
        for (int r = 0; r < i; r++) {
            if ((*arr)[r].obj->id != id) continue;
            else {
                fclose(file);
                printf(ERROR_UNIQUE_IDS);
                return (EXIT_FAILURE);
            }
        }

        struct cluster_t cluster;
        init_cluster(&cluster, 1);

        struct obj_t object = {id, x, y};
        append_cluster(&cluster, object);

        (*arr)[i] = cluster;
    }

    fclose(file);
    return itemsCount;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr) {
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++) {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]) {
    struct cluster_t *clusters;

    int itemsCount = 0;
    int totalItemsCount = 1;

    if (argc == 2) {
        // do nothing, totalItemsCount is already set by default :)
    } else if (argc == 3) {
        totalItemsCount = atoi(argv[2]);
        if (totalItemsCount <= 0) {
            printf(ERROR_BAD_ARGUMENTS);
            return (EXIT_FAILURE);
        }
    } else {
        printf(ERROR_BAD_ARGUMENTS);
        return (EXIT_FAILURE);
    }

    // Load cluster array
    itemsCount = load_clusters(argv[1], &clusters);
    if (itemsCount == -1 || itemsCount < totalItemsCount) {
        printf(ERROR_ITEMS);
        free(clusters);
        return (EXIT_FAILURE);
    } else if (clusters == NULL) {
        printf(ERROR_ITEMS);
        return (EXIT_FAILURE);
    }

    while (itemsCount > totalItemsCount) {
        int index1, index2;
        find_neighbours(clusters, itemsCount, &index1, &index2);
        merge_clusters(&clusters[index1], &clusters[index2]);
        itemsCount = remove_cluster(clusters, itemsCount, index2);
    }

    print_clusters(clusters, itemsCount);

    for(int i = 0; i < totalItemsCount; i++) {
        clear_cluster(&clusters[i]);
    }
    free(clusters);

    return (EXIT_SUCCESS);
}