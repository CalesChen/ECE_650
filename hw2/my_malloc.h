#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


// MetaData and ListNode
typedef struct ListNode{
  size_t size;
  struct ListNode *next;
}node_t;


// Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);

// Thread Safe malloc/free: locking version
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);

// Original Function from hw1
void bf_free(void *ptr, node_t ** head_p);
void *bf_malloc(size_t size, node_t ** head_p, int sbrk_lock);

// Addtional Functions
// Add a freed node to the List
void add_to_List(node_t *ptr, node_t ** head_p);

// Split a node with a larger size
node_t * split(node_t * ptr, size_t size);

// Find the Best Fit
node_t * find_bf(size_t size, node_t ** head_p);

// FreeList Head
node_t *head = NULL;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

size_t data_segment_size = 0;
size_t free_space_segment_size = 0;

__thread node_t * head_tls = NULL;
