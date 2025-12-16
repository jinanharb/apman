#include <libellul.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Benchmark: int_stack_array vs int_stack_deque
 *
 * Medimos o tempo para:
 *   - empilhar N elementos
 *   - desempilhar N elementos
 * para alguns valores de N, e com algumas repetições.
 */

/* ===== Especializações de stack (reaproveitando o template) ===== */

/* Pilha de int baseada em array dinâmico */
#define T_STACK_TAG     int_stack_array
#define T_STACK_ELEMENT int
#define T_IMPL_STACK_ARRAY
#include <libellul/type/stack.h>

/* Pilha de int baseada em deque */
#define T_STACK_TAG     int_stack_deque
#define T_STACK_ELEMENT int
#define T_IMPL_STACK_DEQUE
#include <libellul/type/stack.h>



static double
elapsed_sec(clock_t t0, clock_t t1)
{
    return (double)(t1 - t0) / (double)CLOCKS_PER_SEC;
}



static double
bench_stack_array(size_t n_elems, int rounds)
{
    clock_t t0, t1;
    double total = 0.0;

    for (int r = 0; r < rounds; ++r) {
        int_stack_array_t s = int_stack_array_new();

        t0 = clock();


        for (size_t i = 0; i < n_elems; ++i) {
            int_stack_array_push(&s, (int)i);
        }

    
        for (size_t i = 0; i < n_elems; ++i) {
            int tmp;
            (void)int_stack_array_pop(&s, &tmp);
        }

        t1 = clock();
        total += elapsed_sec(t0, t1);

        int_stack_array_delete(&s);
    }

    return total / (double)rounds;
}

static double
bench_stack_deque(size_t n_elems, int rounds)
{
    clock_t t0, t1;
    double total = 0.0;

    for (int r = 0; r < rounds; ++r) {
        int_stack_deque_t s = int_stack_deque_new();

        t0 = clock();

       
        for (size_t i = 0; i < n_elems; ++i) {
            int_stack_deque_push(&s, (int)i);
        }


        for (size_t i = 0; i < n_elems; ++i) {
            int tmp;
            (void)int_stack_deque_pop(&s, &tmp);
        }

        t1 = clock();
        total += elapsed_sec(t0, t1);

        int_stack_deque_delete(&s);
    }

    return total / (double)rounds;
}


int
main(void)
{
    
    const size_t sizes[] = {
        1u << 10,  
        1u << 15, 
        1u << 18   
    };
    const int n_sizes = (int)(sizeof(sizes) / sizeof(sizes[0]));
    const int rounds  = 5;

    printf("# N  time_array[s]  time_deque[s]\n");

    for (int i = 0; i < n_sizes; ++i) {
        size_t n = sizes[i];

        double t_array = bench_stack_array(n, rounds);
        double t_deque = bench_stack_deque(n, rounds);

        
        printf("%zu %.6f %.6f\n", n, t_array, t_deque);
    }

    return EXIT_SUCCESS;
}
