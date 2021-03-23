/*
 * c-chess-cli, a command line interface for UCI chess engines. Copyright 2020 lucasart.
 *
 * c-chess-cli is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * c-chess-cli is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
*/
#pragma once
#include <pthread.h>
#include <cinttypes>
#include <cstdbool>
#include <cstdio>
#include <vector>

#include "str.h"

// Game results
enum {
    RESULT_LOSS,
    RESULT_DRAW,
    RESULT_WIN,
    NB_RESULT
};

// Per thread data
class Worker {
public:
    struct Deadline_t {
        pthread_mutex_t mtx;
        int64_t timeLimit;
        str_t engineName;
        bool set;
        char pad[7];
    };

    Deadline_t deadline;
    FILE *log;
    uint64_t seed;  // seed for prng()
    int id;  // starts at 1 (0 is for main thread)
    char pad[4];

    void worker_init(int id, const char *logName);
    void worker_destroy();

    void deadline_set(const char *engineName, int64_t timeLimit);
    void deadline_clear();
    int64_t deadline_overdue();

    void workers_busy_add(int n);
    int workers_busy_count(void);

};

//extern Worker *Workers;
extern std::vector<Worker> Workers;



