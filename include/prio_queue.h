#ifndef __PRIO_QUEUE_H__
#define __PRIO_QUEUE_H__

#include <stddef.h>

typedef struct prio_queue_t prio_queue_t;

/// @brief Create new priority queue
/// @param n capacity
/// @param compare priority predicate
/// @param position function to determine position in tree
/// @return pointer to new priority queue
prio_queue_t* pq_new(size_t n,
                     int (*compare)(const void*, const void*),
                     size_t* (*position)(void*));

/// @brief Initialize new priority queue from existing collection
/// @param pq pointer to
/// @param b collection
/// @param s size of collection
void pq_init(prio_queue_t* pq, void* b, size_t s);

/// @brief Enqueue element
/// @param pq pointer to the queue
/// @param x element
void pq_insert(prio_queue_t* pq, void *x);

/// @brief Pop the element with highest priority
/// @param pq pointer to the queue
/// @return pointer to the element with highest priority
void* pq_min(prio_queue_t* pq);

/// @brief Get number of elements in queue
/// @param pq pointer to the queue
/// @return number of enqueued elements
size_t pq_size(prio_queue_t* pq);

/// @brief Change priority if x's key was updated externally
/// @param pq pointer to the queue
/// @param x element which's key has changed
void pq_change_priority(prio_queue_t* pq, void* x);

void pq_free(prio_queue_t *pq);

#endif