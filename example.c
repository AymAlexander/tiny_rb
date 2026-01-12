#include <stdio.h>

/* header only */
#include "tiny_rb.h"

/* Data type can be arbitrary */
typedef struct {
    int a;
    int b;
} user_type_t;

/* Declare your buffer */
TRB_RB_DEFINE(user_type_t, test, 3);
/* Multiple buffers are fine as long as names are unique */
TRB_RB_DEFINE(int, test1, 10);

int main(int argc, char *argv[])
{
    user_type_t a = {1, 2};
    user_type_t b = {3, 4};
    user_type_t c = {5, 6};
    user_type_t value = {0};

    printf("Capacity: %d\r\n", trb_capacity(test));

    printf("------------FIFO------------\r\n");
    /* Push three items */
    trb_fifo_push(test, &a);
    trb_fifo_push(test, &b);
    trb_fifo_push(test, &c);

    printf("Buffer full, push fails: %d\r\n", trb_fifo_push(test, &a));

    /* You can overwrite */
    trb_fifo_force_push(test, &b);
    trb_fifo_force_push(test, &b);
    trb_fifo_force_push(test, &b);
    printf("Element count: %d\r\n", trb_size(test));
    printf("Remaining capacity: %d\r\n", trb_remaining(test));

    trb_fifo_pop(test, &value);
    printf("Value: {%d, %d}\r\n", value.a, value.b);

    trb_fifo_pop(test, &value);
    printf("Value: {%d, %d}\r\n", value.a, value.b);

    trb_fifo_pop(test, &value);
    printf("Value: {%d, %d}\r\n", value.a, value.b);

    /* Insert an element, then flush */
    trb_fifo_push(test, &a);
    trb_flush(test);

    printf("Buffer empty, pop fails: %d\r\n", trb_fifo_pop(test, &value));
    printf("Element count: %d\r\n", trb_size(test));
    printf("Remaining space: %d\r\n", trb_remaining(test));

    printf("------------LIFO------------\r\n");

    trb_lifo_push(test, &a);
    trb_lifo_push(test, &b);
    trb_lifo_push(test, &c);
    printf("Buffer full, push fails: %d\r\n", trb_lifo_push(test, &a));

    trb_lifo_pop(test, &value);
    printf("Value: {%d, %d}\r\n", value.a, value.b);

    trb_lifo_pop(test, &value);
    printf("Value: {%d, %d}\r\n", value.a, value.b);

    trb_lifo_pop(test, &value);
    printf("Value: {%d, %d}\r\n", value.a, value.b);

    printf("Buffer empty, pop fails: %d\r\n", trb_lifo_pop(test, &value));
}
