#include <libellul.h>
#include <stdlib.h>
#include <assert.h>



typedef struct {
 int value;
 link_t link;
} int_node_t;

static void deque_NULL(void) {
 int_node_t *node = NULL;
 deque_t dq;

 test_suite("NULL as a deque");

 // Déque non initialisée
 test_assert(0 == (node ? 1 : 0), "NULL node behaves like empty");
 test_abort(deque_deletefront(&dq), "Cannot delete front on uninitialized deque");
 test_abort(deque_deleteback(&dq), "Cannot delete back on uninitialized deque");

 test_suite("NULL variable as a deque");

 deque_reset(&dq);
 test_assert(dq.length == 0, "Deque reset results in empty deque");
 test_assert(deque_first(&dq) == NULL, "First element is NULL");
 test_assert(deque_last(&dq) == NULL, "Last element is NULL");
}

int_node_t* rand_nodes(size_t n) {
 int_node_t *vec = calloc(n, sizeof(*vec));
 assert(vec);

 for (size_t i = 0; i < n; i++) vec[i].value = rand() % 50;

 return vec;
}

void deque_stack(size_t n) {
 int_node_t *truth = rand_nodes(n);

 deque_t dq;
 deque_init(&dq, int_node_t, link);

 test_assert(dq.length == 0, "New deque is empty");

 // Push back
 for (size_t i = 0; i < n; i++) {
 deque_pushback(&dq, &truth[i]);
 int_node_t *first = (int_node_t*)deque_first(&dq);
 int_node_t *last = (int_node_t*)deque_last(&dq);
 test_assert(first != NULL && last != NULL, "First and last exist after push");
 }

 // Pop back
 for (size_t i = 0; i < n; i++) {
 int_node_t *last = (int_node_t*)deque_last(&dq);
 test_assert(last->value == truth[n - 1 - i].value, "Last element matches truth");
 deque_deleteback(&dq);
 }

 test_assert(dq.length == 0, "Deque is empty after pops");

 free(truth);
}

int main(int argc, char *argv[]) {
 unit_test(argc, argv);

 deque_NULL();
 deque_stack(13);

 exit(EXIT_SUCCESS);
}

