#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

FILE *ofile;
char *fn;

static void die(const char *s) { perror(s); exit(1); }

static void term_handler(int signum) {
    if (fclose(ofile)) { die("fclose"); }
    exit(0);
}

static void do_open(void) {
    ofile = fopen(fn, "a");
    if (!ofile) { die("fopen"); }
    if (setvbuf(ofile, NULL, _IOLBF, 0)) { die("setvbuf"); }
}


static void usr1_handler(int signum) {
    if (fclose(ofile)) { die("fclose"); }
    do_open();
}

int main(int argc, char **argv) {
    size_t n; int i; char buf[BUFSIZ];

    if (argc != 2) {
        fprintf(stderr, "USAGE: %s file.log\n", argv[0]); exit(1);
    }

    fn = argv[1];
    do_open();

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = term_handler;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = usr1_handler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    if (setvbuf(stdin, NULL, _IONBF, 0)) { die("setvbuf"); }

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (fwrite(buf, 1, n, ofile) != n) { die("fwrite"); }
    }
    if (ferror(stdin)) { die("read from stdin"); }

    return 0;
}
