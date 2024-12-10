#include "prio_queue.h"

#include <malloc.h>
#include <memory.h>

struct prio_queue_t {
    int (*compare)(const void*, const void*);
    size_t* (*position)(void*);
    size_t i;
    size_t n;
    void *a[];
};

static void swap(prio_queue_t* q, size_t i, size_t j)
{
    void *x;

    x = q->a[i];
    q->a[i] = q->a[j];
    q->a[j] = x;
    *(*q->position)(q->a[i]) = i;
    *(*q->position)(q->a[j]) = j;
}


static void up(prio_queue_t* q, size_t k)
{
    size_t j;

    if (k > 1) {
        j = k / 2;
        if ((*q->compare)(q->a[j], q->a[k]) > 0) {
            swap(q, k, j);
            up(q, j);
        }
    }
}

static void down(prio_queue_t* q, size_t k)
{
    size_t j;
    if (2 * k > q->i)
        return;
    else if (2 * k == q->i)
        j = 2 * k;
    else if ((*q->compare)(q->a[2*k], q->a[2*k + 1]) < 0)
        j = 2 * k;
    else
        j = 2 * k + 1;

    if ((*q->compare)(q->a[k], q->a[j]) > 0) {
        swap(q, k, j);
        down(q, j);
    }
}

prio_queue_t* new_prio_queue(size_t n,
                             int (*compare)(const void*, const void*),
                             size_t* (*position)(void*))
{
    prio_queue_t* pq;
    size_t s;
    s = sizeof(prio_queue_t) + (n + 1) * sizeof(void*);
    pq = malloc(s);
    memset(pq, 0, s);
    pq->n = n;
    pq->i = 0;
    pq->compare = compare;
    pq->position = position;
    return pq;
}

void init_prio_queue(prio_queue_t* q, void* b, size_t s)
{
    size_t k;
    void* x;

    for (k = 1; k <= q->n; k++)
    {
        q->a[k] = (char*)b + s * (k - 1);
        *(*q->position)(q->a[k]) = k;
    }

    q->i = q->n;
    for (k = q->n / 2; k >= 1; k--)
        down(q, k);
}




void pq_insert(prio_queue_t* q, void *x)
{
    q->i++;
    q->a[q->i] = x;
    *(*q->position)(q->a[q->i]) = q->i;
    up(q, q->i);
}

size_t pq_size(prio_queue_t* q)
{
    return q->i;
}

void* pq_min(prio_queue_t* q)
{
    void* x;

    x = q->a[1];
    q->a[1] = q->a[q->i];
    *(*q->position)(q->a[1]) = 1;
    q->a[q->i] = NULL;
    q->i--;
    down(q, 1);

    return x;
}

