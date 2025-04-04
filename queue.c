#include "queue.h"
#include "tile_game.h"
#include <stdint.h>
#include <stddef.h>

// Temporary stub for linking — remove once real function is available
int is_solved(struct game_state state) {
    return state.tiles[0][0] == 1;
}

// Temporary stub — no valid next states returned
size_t next_states(struct game_state state, struct game_state neighbors[4]) {
    return 0; 
}

void enqueue(struct queue *q, struct game_state state) {
    size_t val = serialize(state);
    insert_at_tail(&q->data, val);
}

struct game_state dequeue(struct queue *q) {
    size_t val = remove_from_head(&q->data);
    return deserialize(val);
}

int number_of_moves(struct game_state start) {
    struct queue q = {0};
    enqueue(&q, start);

    uint8_t visited[65536] = {0}; // 2^16 space for visited states
    visited[serialize(start)] = 1;

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
            if (!visited[encoded]) {
                visited[encoded] = 1;
                enqueue(&q, neighbors[i]);
            }
        }
    }

    free_list(q.data);
    return -1; // Should never happen if puzzle is solvable
}
