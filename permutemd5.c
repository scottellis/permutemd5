#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <mhash.h>

#define MAX_SEED_LEN 32

void permute(char *in, int len, int pos);
void generate_replacement_table();
void hash(const char *s);
char *normalize_target(const char *s);


char *trailers[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "123", "!", "?", "$", "#", "" };
char *rep_table[128]; /* only ascii 0x00 - 0x7f */
char *target;
char *seed;
int append_suffix;
int verbose;

void usage(const char *argv_0)
{
	printf("Usage: %s [-t <target>] [-v] seed\n", argv_0);
	printf("  -t       Target hash. Program stops if hash of the permuted value matches.\n");
	printf("  -v       Verbose output. Always true if no target provided.\n");
	printf("  seed     The seed string to be modified and hashed.\n");

	exit(1);
}

int main(int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, "t:v")) != -1) {
		switch (opt) {
		case 't':
			target = normalize_target(optarg);

			if (!target) {
				fprintf(stderr, "Invalid target, must be all hex: %s\n", optarg);
				exit(1);
			}

			break;

		case 'v':
			verbose = 1;
			break;

		default:
			usage(argv[0]);
			break;
		}
	}

	append_suffix = 1;

	if (optind < argc) {
		/* this length limit is arbitrary */
		if (strlen(argv[optind]) > MAX_SEED_LEN) {
			fprintf(stderr, "Max seed length is %d\n", MAX_SEED_LEN);
			exit(1);
		}

		/* later might accept a list, only accept one seed at a time for now */
		seed = strdup(argv[optind]);

		if (!seed) {
			perror("strdup");
			exit(1);
		}
	}
	else {
		printf("A seed string is required.\n");
		usage(argv[0]);
	}

	if (!target)
		verbose = 1;

	generate_replacement_table();

	permute(seed, strlen(seed), 0);

	return 0;
}

char *normalize_target(const char *s)
{
	int i;
	int len = strlen(s);

	for (i = 0; i < len; i++) {
		if (!isxdigit(s[i]))
			return NULL;
	}

	char *t = strdup(s);

	if (!t) {
		perror("strdup");
		exit(1);
	}

	for (i = 0; i < len; i++)
		t[i] = tolower(t[i]);

	return t;
}

void hash(const char *s)
{
	MHASH td;
	unsigned char *hash;
	char *hbuff;
	int found = 0;

	int hash_size = mhash_get_block_size(MHASH_MD5);

	if (hash_size < 1) {
		fprintf(stderr, "Failed to get hash size\n");
		exit(1);
	}

	td = mhash_init(MHASH_MD5);

	if (td == MHASH_FAILED) {
		fprintf(stderr, "mhash_init failed\n");
		exit(1);
	}

	hash = malloc(hash_size);

	if (!hash) {
		perror("malloc");
		exit(1);
	}

	hbuff = malloc(4 + (hash_size * 2));

	if (!hbuff) {
		perror("malloc");
		exit(1);
	}

	bzero(hbuff, 4 + (hash_size * 2));
	mhash(td, s, strlen(s));
	mhash_deinit(td, hash);

	for (int i = 0; i < hash_size; i++)
		sprintf(&hbuff[i * 2], "%.2x", hash[i]);

	free(hash);

	if (target && !strcmp(target, hbuff)) {
		printf("Success!\n");
		printf("%s : %s\n", s, hbuff);
		found = 1;
	}
	else if (verbose) {
		printf("%s : %s\n", s, hbuff);
	}

	free(hbuff);

	if (found)
		exit(1);
}

void permute(char *in, int len, int pos)
{
	int i, rlen, rpos;
	char tbuff[MAX_SEED_LEN * 2];

	if (pos >= len) {
		hash(in);

		if (append_suffix) {
			bzero(tbuff, sizeof(tbuff));
			strcpy(tbuff, in);
			rlen = strlen(tbuff);

			for (i = 0; *trailers[i] != 0; i++) {
				strcpy(tbuff + rlen, trailers[i]);
				hash(tbuff);
			}
		}
	}
	else {
		permute(in, len, pos + 1);

		rpos = (int) in[pos];

		if (rpos < 0 || rpos > 127)
			return;

		if (islower(rpos)) {
			in[pos] = toupper(rpos);
			permute(in, len, pos + 1);
			in[pos] = (char)rpos;
		}
		else if (isupper(rpos)) {
			in[pos] = tolower(rpos);
			permute(in, len, pos + 1);
			in[pos] = (char)rpos;
		}

		if (rep_table[tolower(rpos)]) {
			rlen = strlen(rep_table[tolower(rpos)]);

			for (i = 0; i < rlen; i++) {
				in[pos] = rep_table[tolower(rpos)][i];
				permute(in, len, pos + 1);
			}

			in[pos] = (char)rpos;
		}
	}
}

/* this is pretty lame */
void generate_replacement_table()
{
	bzero(rep_table, sizeof(rep_table));

	rep_table['e'] = strdup("3");
	rep_table['h'] = strdup("4");
	rep_table['i'] = strdup("1!");
	rep_table['l'] = strdup("1!");
	rep_table['o'] = strdup("0");
	rep_table['s'] = strdup("z5$");
	rep_table['z'] = strdup("s5$");
	rep_table['t'] = strdup("7");
}
