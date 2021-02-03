#include "my_malloc.h"
#include "assert.h"
// Performance study function
unsigned long get_data_segment_size(){
  return data_segment_size;
}
unsigned long get_data_segment_free_space_size(){
  return free_space_segment_size;
}

//First Fit malloc/free



void add_to_List(node_t *ptr){
  if(ptr == NULL || ptr->free == 1) return;
  ptr->free = 1;
  if(head == NULL){
    head = ptr;
    free_space_segment_size += ptr->size + sizeof(node_t);
    return;
  }
  
  ptr->next = head;
  head = ptr;
  free_space_segment_size += ptr->size + sizeof(node_t);

  node_t *p = head;
  size_t b_ptr = ptr->size + sizeof(node_t);
  size_t n_ptr = (size_t)ptr;
  //mem address of the next;
  size_t next = n_ptr + b_ptr;
  // Iterate to find the suitable location in a sorted list
  while(p != NULL && p->next != NULL){
    //size_t bytes = p->size + sizeof(node_t);
    void * p_temp = p->next;
    if(p_temp == NULL) break;
    size_t temp = (size_t) p_temp;
    // If I find the next node, then break
    if(temp >= next){
      break;
    }
    p = p_temp;
  }

  // if the ptr is the highest address
  if(p->next == NULL){
    head = head->next;
    ptr->next = p->next;
    p->next = ptr;
  }
  else if(p == head){
    node_t * n_ptr_next = (node_t*)((char*)ptr + ptr->size + sizeof(node_t));
    if(n_ptr_next == ptr->next){
      ptr->size += (sizeof(node_t) + ptr->next->size);
      ptr->next = ptr->next->next;
    }
    return;
  }
  else{
    size_t bytes = p->size + sizeof(node_t);
    size_t temp = (size_t)p;
    // Check if the two are adjacent the former one and the ptr
    if(temp + bytes == (size_t)ptr){
      p->size += (b_ptr);
      head = head->next;
      node_t * p_next = p->next;
      node_t* n_p_next = (node_t*)((char*)p + p->size + sizeof(node_t));
      if(n_p_next == p->next){
        // Modify the size and the next pointer
        p->size += (sizeof(node_t) + p_next->size);
        p->next = p_next->next;
      }
    }
    else{
      head = head->next;
      ptr->next = p->next;
      p->next = ptr;
      // check if we need to merge the next one
      node_t * n_ptr_next = (node_t*)((char*)ptr + ptr->size + sizeof(node_t));
      if(n_ptr_next == ptr->next){
        //modify the size and the next pointer
        ptr->size += (sizeof(node_t) + ptr->next->size);
        ptr->next = ptr->next->next;
      }
    }
  }

  //Recheck the List in order to merge the newly one
  /*p = head;
  while(p != NULL && p->next != NULL){
    size_t bytes = p->size + sizeof(node_t);
    size_t temp = (size_t)p;
    size_t next = (size_t)(p->next);
    if(bytes + temp == next){
      p->size += (sizeof(node_t) + p->next->size);
      p->next = p->next->next;
    }
    else{
      p = p->next;
    }
    }*/
}
// Plus and Minus Operation will add 1 or minus 1 for void*
void ff_free(void *ptr){
  if(ptr == NULL) return;
  node_t *node = ptr - sizeof(node_t);
  add_to_List(node);
}

void bf_free(void *ptr){
  if(ptr == NULL) return;
  node_t * node = ptr - sizeof(node_t);
  add_to_List(node);
}
node_t* split(node_t * ptr, size_t size){
  node_t * res = (node_t *)((char *)ptr + ptr->size - size );
  res->size = size;
  res->free = 0;
  res->next = NULL;
  ptr->size = ptr->size - size - sizeof(node_t);
  // In malloc func we will return res + 1
  return res;
}

node_t* find_ff(size_t size){
  node_t * curr = head;
  while(curr != NULL){
    if(curr->size >= size){
      return curr;
    }
    curr = curr->next;
    
  }
  return curr;
}

node_t* find_bf(size_t size){
  node_t * curr = head;
  node_t * res = NULL;
  unsigned int min = 65535;
  
  while(curr != NULL){
    if(curr->size >size + sizeof(node_t)){
      if(res == NULL || curr->size < res->size){
	res = curr;
      }
    }
    else if(curr->size >= size){
      return curr;
    }
    curr = curr->next;
    /*if(curr->free == 0 || curr->size < size){
      curr = curr->next;
      continue;
    }
    else if(curr->size == size){
      return curr;
    }
    else if(curr->size - size < min){
      min = curr->size - size;
      res = curr;
    }
    curr = curr->next;*/
  }
  return res;
}
void *ff_malloc(size_t size){
   node_t *res=NULL;
   if(size == 0) return res;
   // The bytes includes metadata
   size_t actual_size = size + sizeof(node_t);
   // find__ff will use size as in this func we compare p->size with size
   node_t *ff = find_ff(size);
   if(head == NULL || ff == NULL){
     res=sbrk(actual_size);
     res->size = size;
     res->free = 0;
     res->next = NULL;
     if(res != NULL){
       data_segment_size += actual_size;
     }
     // The res will skip over the Metadata
     return res == NULL ? NULL : res + 1;
   }
   else{
     // Check if we can split the corresponding node
     if(ff->size > actual_size){
       res =  split(ff, size);
       free_space_segment_size -= actual_size;
     }
     // If we can not split it, we just return the ff;
     else if(ff->size <= (size + sizeof(node_t))){
       res = ff;
       free_space_segment_size -= (ff->size + sizeof(node_t));
       node_t * p = head;
       if(p == res){
         head = head->next;
         res->free = 0;
         res->next = NULL;
         p = NULL; // Then we can skip over the next iteration
       }
       while(p != NULL){
         if(p->next == res){
           p->next = res->next;
           res->free = 0;
           res->next = NULL;
           break;
         }
         p = p->next;
       }
     }
   }
   //if(res != NULL){
   //   data_segment_size += actual_size;
   //}
   // We skip over the metadata
   return res + 1;
}
void *bf_malloc(size_t size){
  node_t *res = NULL;
  if(size == 0) return res;
  size_t actual_size = size + sizeof(node_t);
  node_t *bf = find_bf(size);
  if(head == NULL || bf == NULL){
    res = sbrk(actual_size);
    res->size = size;
    res->free = 0;
    res->next = NULL;
    if(res != NULL){
      data_segment_size += actual_size;
    }
    return res = NULL ? NULL : res + 1;
  }
  else{
    if(bf->size > actual_size){
      res = split(bf, size);
      free_space_segment_size -= actual_size;
    }
    else if(bf->size <= actual_size){
      res = bf;
      free_space_segment_size -= (bf->size + sizeof(node_t));
      // Remove the node from the List
      node_t * p = head;
      if(p == res){
        head = head->next;
        res->free = 0;
        res->next = NULL;
        p = NULL; // Then we can skip over the next iteration
      }
      while(p != NULL){
        if(p->next == res){
          p->next = res->next;
          res->free = 0;
          res->next = NULL;
          break;
        }
        p = p->next;
      }
    }
  }
  //if(res != NULL){
  //  free_space_segment_size -= actual_size;
  //}
  return res + 1;
}
