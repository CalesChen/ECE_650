#include <stdio.h>
#include <unistd.h> // including sbrk()


// Copy from the PDF 
//First Fit malloc/free
void *ff_malloc(size_t size);
void ff_free(void *ptr);

//Best Fit malloc/free
void *bf_malloc(size_t size);
void bf_free(void *ptr);

//Performance study report
unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();

// My own MetaData and ListNode 
typedef struct ListNode{
  size_t size;
  int free;
  struct ListNode *next;
  //struct ListNode *mem_next;
}node_t;

// Additional Functions
// Add a freed node to the List
void add_to_List(node_t *ptr);

// Split a node with a larger size
node_t *split(node_t * ptr, size_t size);

//Find the First Fit Node
node_t * find_ff(size_t size);
//Find the Best Fit Node
node_t * find_bf(size_t size);

// FreeList head
node_t *head = NULL;

size_t data_segment_size = 0;
size_t free_space_segment_size = 0;
