/*
 *  c-gomoku-cli, a command line interface for Gomocup engines. Copyright 2021 Chao Ma.
 *  c-gomoku-cli is derived from c-chess-cli, originally authored by lucasart 2020.
 *
 *  c-gomoku-cli is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 *  c-gomoku-cli is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with this
 * program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef __MINGW32__
    #include <sys/types.h>
#endif

#include <cstdio>
#include <cstdint>
#include <string>

class Worker;

// Elements remembered from parsing info lines (for writing PGN comments)
struct Info
{
    int     score, depth;
    int64_t time;
};

// Engine process
class Engine
{
public:
    std::string name;

    Engine(Worker *worker, bool debug, std::string *outmsg);
    Engine(const Engine &) = delete;  // disable copy
    ~Engine();

    void start(const char *cmd, const char *name, int64_t tolerance);
    void terminate(bool force = false);

    bool readln(std::string &line);
    void writeln(const char *buf);

    bool wait_for_ok(bool fatalError);
    bool bestmove(int64_t     &timeLeft,
                  int64_t      maxTurnTime,
                  std::string &best,
                  Info        &info,
                  int          moveply);

    bool is_ok() const { return pid != 0; }
    bool is_crashed() const { return pid && (!in || !out); }

private:
    Worker *const w;
    const bool    isDebug;
    FILE         *in, *out;
    std::string  *messages;
    int64_t       tolerance;

#ifdef __MINGW32__
    long  pid;
    void *hProcess;
#else
    pid_t pid;
#endif

    enum OutputType {
        OT_DIRECT,   // No prefix
        OT_UNKNOWN,  // Output with prefix "UNKNOWN"
        OT_ERROR,    // Output with prefix "ERROR"
        OT_MESSAGE,  // Output with prefix "MESSAGE"
        OT_DEBUG,    // Output with prefix "DEBUG"
        OT_SUGGEST,  // Output with prefix "SUGGEST"
    };

    void       spawn(const char *cwd, const char *run, char **argv, bool readStdErr);
    void       parse_about(const char *fallbackName);
    OutputType process_common_output(const char *line, const char *&tail_out);
    void       parse_thinking_message(const char *line, Info &info);
};
