#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state) {
    size_t serialized = serialize(state);
    insert_at_tail(&q->list, serialized);
}

struct game_state dequeue(struct queue *q) {
    size_t serialized = remove_from_head(&q->list);
    return deserialize(serialized);
}

int number_of_moves(struct game_state start) {
    struct game_state goal = {
        .board = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 0}
        },
        .empty_row = 3,
        .empty_col = 3,
        .moves = 0
    };

    if (memcmp(start.board, goal.board, sizeof(start.board)) == 0) {
        return 0;
    }

    struct queue q = { .list = { .head = NULL, .tail = NULL } };
    enqueue(&q, start);

    #define MAX_VISITED 100000
    size_t visited[MAX_VISITED];
    size_t visited_count = 0;

    while (q.list.head != NULL) {
        struct game_state current = dequeue(&q);
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; i++) {
            int new_row = current.empty_row + directions[i][0];
            int new_col = current.empty_col + directions[i][1];
            if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
                // Create new state by swapping empty space with neighbor
                struct game_state new_state = current;
                new_state.board[current.empty_row][current.empty_col] = 
                    new_state.board[new_row][new_col];
                new_state.board[new_row][new_col] = 0;
                new_state.empty_row = new_row;
                new_state.empty_col = new_col;
                new_state.moves++;
                if (memcmp(new_state.board, goal.board, sizeof(goal.board)) == 0) {
                    free_list(q.list);
                    return new_state.moves;
                }
                size_t serialized = serialize(new_state);
                int found = 0;
                for (size_t j = 0; j < visited_count; j++) {
                    if (visited[j] == serialized) {
                        found = 1;
                        break;
                    }
                }
                if (!found && visited_count < MAX_VISITED) {
                    enqueue(&q, new_state);
                    visited[visited_count++] = serialized;
                }
            }
        }
    }

    free_list(q.list);
    return -1; 

}
