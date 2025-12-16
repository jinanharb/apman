#include <libellul.h>
#include <stdlib.h>


#define T_QUEUE_TAG     int_queue
#define T_QUEUE_ELEMENT int
#include <libellul/type/queue.h>

static void
queue_basic(void)
{
    test_suite("int_queue basic behaviour");

    int_queue_t q = int_queue_new();

    test_assert(int_queue_is_empty(q),
                "new() queue is empty");

    for (int i = 0; i < 5; ++i) {
        int_queue_push(q, i);
    }

    test_assert(int_queue_length(q) == 5,
                "length after pushes == 5");

    int front = -1, back = -1;
    test_assert(int_queue_front(q, &front),
                "front() on non-empty queue");
    test_assert(int_queue_back(q, &back),
                "back() on non-empty queue");
    test_assert(front == 0, "front == 0");
    test_assert(back  == 4, "back  == 4");

    for (int i = 0; i < 5; ++i) {
        int value = -1;
        test_assert(int_queue_pop(q, &value),
                    "pop() succeeds");
        test_assert(value == i,
                    "FIFO order preserved");
    }

    test_assert(int_queue_is_empty(q),
                "queue is empty after pops");

    int_queue_delete(&q);
    test_assert(q == NULL,
                "delete() sets pointer to NULL");
}

int
main(int argc, char *argv[])
{
    unit_test(argc, argv);

    queue_basic();

    exit(EXIT_SUCCESS);
}
