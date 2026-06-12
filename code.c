#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 8

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} RingBuffer;

void init_buffer(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

int is_full(RingBuffer *rb) {
    if (rb->count == BUFFER_SIZE) {
        return 1;
    }
    return 0;
}

int is_empty(RingBuffer *rb) {
    if (rb->count == 0) {
        return 1;
    }
    return 0;
}

uint16_t get_count(RingBuffer *rb) {
    return rb->count;
}

int write_byte(RingBuffer *rb, uint8_t data) {
    if (is_full(rb)) {
        return 0;
    }
    
    rb->buffer[rb->head] = data;

    rb->head = (rb->head + 1) & (BUFFER_SIZE - 1);
    rb->count++;
    
    return 1;
}

int read_byte(RingBuffer *rb, uint8_t *data_out) {
    if (is_empty(rb)) {
        return 0;
    }
    
    *data_out = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) & (BUFFER_SIZE - 1);
    rb->count--;
    
    return 1;
}

int main(void) {
    RingBuffer my_buf;
    init_buffer(&my_buf);

    uint8_t val;
    for (val = 0x41; val <= 0x48; val++) {
        if (write_byte(&my_buf, val) == 1) {
            printf("[WRITE] 0x%02X -> OK (count=%u)", val, get_count(&my_buf));
            if (is_full(&my_buf) == 1) {
                printf(" FULL\n");
            } else {
                printf("\n");
            }
        }
    }

    if (write_byte(&my_buf, 0x99) == 0) {
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");
    }

    uint8_t read_val;
    int i;
    for (i = 0; i < 3; i++) {
        if (read_byte(&my_buf, &read_val) == 1) {
            printf("[READ] -> 0x%02X (count=%u)\n", read_val, get_count(&my_buf));
        }
    }

    for (val = 0x49; val <= 0x4B; val++) {
        if (write_byte(&my_buf, val) == 1) {
            printf("[WRITE] 0x%02X -> OK (count=%u)", val, get_count(&my_buf));
            if (is_full(&my_buf) == 1) {
                printf(" FULL\n");
            } else {
                printf("\n");
            }
        }
    }

    while (is_empty(&my_buf) == 0) {
        if (read_byte(&my_buf, &read_val) == 1) {
            printf("[READ] -> 0x%02X (count=%u)\n", read_val, get_count(&my_buf));
        }
    }

    if (read_byte(&my_buf, &read_val) == 0) {
        printf("[READ] (empty) -> FAIL (buffer empty)\n");
    }

    return 0;
}