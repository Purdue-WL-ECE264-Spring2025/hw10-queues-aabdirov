#include "queue.h"
#include "tile_game.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Temporary stubs (replace when real ones are linked in)
int is_solved(struct game_state state) {
    // Fake condition for testing
    return state.tiles[0][0] == 1;
}

size_t next_states(struct game_state state, struct game_state neighbors[4]) {
    // No neighbors in stub version
    return 0;
}

void enqueue(struct queue *q, struct game_state state) {
    size_t val = serialize(state);
    printf("Enqueue: %zu\n", val);  // Debug
    insert_at_tail(&q->data, val);
}

struct game_state dequeue(struct queue *q) {
    size_t val = remove_from_head(&q->data);
    printf("Dequeue: %zu\n", val);  // Debug
    return deserialize(val);
}

int number_of_moves(struct game_state start) {
    struct queue q = {0};
    enqueue(&q, start);

    uint8_t visited[65536] = {0};
    size_t start_encoded = serialize(start);
    if (start_encoded < 65536) {
        visited[start_encoded] = 1;
    }

    while (q.data.head != NULL) {
        struct game_state current = dequeue(&q);

        if (is_solved(current)) {
            free_list(q.data);
            return current.num_steps;
        }

        struct game_state neighbors[4];
        size_t n = next_states(current, neighbors);

        for (size_t i = 0; i < n; i++) {
            size_t encoded = serialize(neighbors[i]);
            if (encoded < 65536 && !visited[encoded]) {
                visited[encoded] = 1;
                enqueue(&q, neighbors[i]);
            }
        }
    }

    free_list(q.data);
    return -1; // Should never happen for a solvable puzzle
}
