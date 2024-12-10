#ifndef __PRIO_QUEUE_H__
#define __PRIO_QUEUE_H__

#include <stddef.h>

typedef struct prio_queue_t prio_queue_t;

prio_queue_t* new_prio_queue(size_t n,
                             int (*compare)(const void*, const void*),
                             size_t* (*position)(void*));

void init_prio_queue(prio_queue_t* q, void* b, size_t s);

void pq_insert(prio_queue_t* q, void *x);
size_t pq_size(prio_queue_t* q);
void* pq_min(prio_queue_t* q);

#endif