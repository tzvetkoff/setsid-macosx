/*
Copyright (c) 2014, Latchezar Tzvetkoff
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <err.h>
#include <sys/ioctl.h>

#include "version.h"

static void usage(const char *program_invocation_name, FILE *out) {
    fputs("\n", out);
    fputs("Usage:\n", out);
    fprintf(out, "  %s [options] <program> [arguments ...]\n",
        program_invocation_name);

    fputs("\n", out);
    fputs("Options:\n", out);
    fputs("  -w, --wait     wait program to exit, and use the same return\n",
        out);
    fputs("  -c, --ctty     ignored, added for compatibility\n", out);
    fputs("  -h, --help     display this help and exit\n", out);
    fputs("  -V, --version  print version information and exit\n", out);

    fputs("\n", out);
    fputs("For more details see setsid(1).\n", out);
    exit(out == stderr ? EXIT_FAILURE : EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int ch;
    pid_t pid;
    int status = 0;

    static const struct option longopts[] = {
        {"ctty", no_argument, NULL, 'c'},
        {"wait", no_argument, NULL, 'w'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'V'},
        {NULL, 0, NULL, 0}
    };

    while ((ch = getopt_long(argc, argv, "+cwhvV", longopts, NULL)) != -1) {
        switch (ch) {
            case 'c':
                break;
            case 'w':
                status = 1;
                break;
            case 'V':
            case 'v':
                puts(SETSID_VERSION);
                return EXIT_SUCCESS;
            case 'h':
                usage(argv[0], stdout);
            default:
                usage(argv[0], stderr);
        }
    }

    if (argc < 2) {
        usage(argv[0], stderr);
    }

    if (getpgrp() == getpid()) {
        pid = fork();

        switch (pid) {
            case -1:
                err(EXIT_FAILURE, "fork");
            case 0:
                /* child */
                break;
            default:
                /* parent */
                if (!status) {
                    return EXIT_SUCCESS;
                }
                if (wait(&status) != pid) {
                    err(EXIT_FAILURE, "wait");
                }
                if (WIFEXITED(status)) {
                    return WEXITSTATUS(status);
                }
                err(status, "child %d did not exit normally", pid);
        }
    }

    if (setsid() < 0) {
        /* cannot happen */
        err(EXIT_FAILURE, "setsid failed");
    }

    execvp(argv[optind], argv + optind);
    err(EXIT_FAILURE, "failed to execute %s", argv[optind]);
}
