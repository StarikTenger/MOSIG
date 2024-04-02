#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define NB_SONG 3

struct ParamPamPam {
	int repeats;
	char *str;
};

void *supporter(void *arg) {
	char *str = ((struct ParamPamPam *)arg)->str;
	int i;
	int pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();

	for (i = 0; i < ((struct ParamPamPam *)arg)->repeats; i++) {
		printf("Process %d Thread %x : %s \n", pid, (unsigned int)tid, str);
	}
	return (void *)tid;
}

int main(int argc, char **argv) {

	int team1;
	int team1_repeats;
	int team2;
	int team2_repeats;
	int i;
	int nb_threads = 0;
	pthread_t *tids;

	if (argc != 4 + 1) {
		fprintf(stderr, "usage : %s team1 team2\n", argv[0]);
		exit(-1);
	}

	team1 = atoi(argv[1]);
	team1_repeats = atoi(argv[2]);
	team2 = atoi(argv[3]);
	team2_repeats = atoi(argv[4]);
	nb_threads = team1 + team2;
	tids = malloc(nb_threads * sizeof(pthread_t));

	struct ParamPamPam param1;
	param1.repeats = team1_repeats;
	param1.str = "Allons enfants de la patrie";

	struct ParamPamPam param2;
	param2.repeats = team2_repeats;
	param2.str = "Swing low, sweet chariot";

	/* Create the threads for team1 */
	for (i = 0; i < team1; i++) {
		pthread_create(&tids[i], NULL, supporter, &param1);

	}
	/* Create the other threads (ie. team2) */
	for (; i < nb_threads; i++) {
		pthread_create(&tids[i], NULL, supporter, &param2);
	}

	printf("QQQ\n");

	/* Wait until every thread ended */
	for (i = 0; i < nb_threads; i++) {
		pthread_join(tids[i], NULL);
	}

	printf("WWW\n");

	free(tids);
	return EXIT_SUCCESS;
}
