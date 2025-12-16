#include <libellul.h>
#include <stdlib.h>




#define T_STACK_TAG     int_stack_array
#define T_STACK_ELEMENT int
#define T_IMPL_STACK_ARRAY
#include <libellul/type/stack.h>


#define T_STACK_TAG     int_stack_deque
#define T_STACK_ELEMENT int
#define T_IMPL_STACK_DEQUE
#include <libellul/type/stack.h>



static void
stack_array_basic(void)
{
    test_suite("int_stack_array basic behaviour");

    int_stack_array_t s = int_stack_array_new();

    test_assert(int_stack_array_is_empty(s),
                "new() stack_array is empty");

    for (int i = 0; i < 5; ++i) {
        int_stack_array_push(&s, i);
    }

    test_assert(int_stack_array_length(s) == 5,
                "length after pushes == 5");

    int top = -1;
    test_assert(int_stack_array_top(s, &top),
                "top() on non-empty stack_array");
    test_assert(top == 4,
                "top is last pushed element");

    for (int i = 4; i >= 0; --i) {
        int value = -1;
        test_assert(int_stack_array_pop(&s, &value),
                    "pop() succeeds");
        test_assert(value == i,
                    "LIFO order preserved");
    }

    test_assert(int_stack_array_is_empty(s),
                "stack_array is empty after pops");

    int_stack_array_delete(&s);
    test_assert(s == NULL,
                "delete() sets pointer to NULL");
}

static void
stack_deque_basic(void)
{
    test_suite("int_stack_deque basic behaviour");

    int_stack_deque_t s = int_stack_deque_new();

    test_assert(int_stack_deque_is_empty(s),
                "new() stack_deque is empty");

    for (int i = 0; i < 5; ++i) {
        int_stack_deque_push(&s, i);
    }

    test_assert(int_stack_deque_length(s) == 5,
                "length after pushes == 5");

    int top = -1;
    test_assert(int_stack_deque_top(s, &top),
                "top() on non-empty stack_deque");
    test_assert(top == 4,
                "top is last pushed element");

    for (int i = 4; i >= 0; --i) {
        int value = -1;
        test_assert(int_stack_deque_pop(&s, &value),
                    "pop() succeeds");
        test_assert(value == i,
                    "LIFO order preserved");
    }

    test_assert(int_stack_deque_is_empty(s),
                "stack_deque is empty after pops");

    int_stack_deque_delete(&s);
    test_assert(s == NULL,
                "delete() sets pointer to NULL");
}

int
main(int argc, char *argv[])
{
    unit_test(argc, argv);

    stack_array_basic();
    stack_deque_basic();

    exit(EXIT_SUCCESS);
}