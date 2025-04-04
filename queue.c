#include "queue.h"
#include "tile_game.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Temporary stub: check if puzzle is solved
int is_solved(struct game_state state) {
    int expected = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == 3 && j == 3) {
                if (state.tiles[i][j] != 0) return 0;
            } else {
                if (state.tiles[i][j] != expected) return 0;
                expected++;
            }
        }
    }
    return 1;
}

// Temporary stub: generate valid next states by moving the empty tile
size_t next_states(struct game_state state, struct game_state neighbors[4]) {
    size_t count = 0;
    int row = state.empty_row;
    int col = state.empty_col;

    const int d_row[] = {-1, 1, 0, 0}; // Up, down
    const int d_col[] = {0, 0, -1, 1}; // Left, right

    for (int d = 0; d < 4; d++) {
        int new_row = row + d_row[d];
        int new_col = col + d_col[d];
        if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
            struct game_state new_state = state;
            new_state.tiles[row][col] = new_state.tiles[new_row][new_col];
            new_state.tiles[new_row][new_col] = 0;
            new_state.empty_row = new_row;
            new_state.empty_col = new_col;
            neighbors[count++] = new_state;
        }
    }

    return count;
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
                neighbors[i].num_steps = current.num_steps + 1;
                enqueue(&q, neighbors[i]);
            }
        }
    }

    free_list(q.data);
    return -1;
}
