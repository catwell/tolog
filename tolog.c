#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

FILE *ofile;
char *fn;

static void term_handler(int signum) {
    if (fclose(ofile)) { perror("fclose"); exit(1); }
    exit(0);
}

static void usr1_handler(int signum) {
    if (fclose(ofile)) { perror("fclose"); exit(1); }
    ofile = fopen(fn, "a");
    if (!ofile) { perror("fopen"); exit(1); }
}

int main(int argc, char **argv) {
    size_t n; int i; char buf[BUFSIZ];

    if (argc != 2) {
        fprintf(stderr, "USAGE: %s file.log\n", argv[0]); exit(1);
    }

    fn = argv[1];
    ofile = fopen(fn, "a");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = term_handler;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = usr1_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (fwrite(buf, 1, n, ofile) != n) { perror("fwrite"); exit(1); }
    }
    if (ferror(stdin)) { perror("read from stdin"); exit(1); }

    return 0;
}
