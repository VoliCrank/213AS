#include "uthread.h"
#include "uthread_mutex_cond.h"
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf(S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t match;
  uthread_cond_t paper;
  uthread_cond_t tobacco;
  uthread_cond_t smoke;
};

struct Agent *createAgent() {
  struct Agent *agent = malloc(sizeof(struct Agent));
  agent->mutex = uthread_mutex_create();
  agent->paper = uthread_cond_create(agent->mutex);
  agent->match = uthread_cond_create(agent->mutex);
  agent->tobacco = uthread_cond_create(agent->mutex);
  agent->smoke = uthread_cond_create(agent->mutex);
  return agent;
}

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource { MATCH = 1, PAPER = 2, TOBACCO = 4 };
char *resource_name[] = {"", "match", "paper", "", "tobacco"};

int signal_count[5]; // # of times resource signalled
int smoke_count[5];  // # of times smoker with resource smoked

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it
 * does is choose 2 random reasources, signal their condition variables, and
 * then wait wait for a smoker to smoke.
 */
void *agent(void *av) {
  struct Agent *a = av;
  static const int choices[] = {MATCH | PAPER, MATCH | TOBACCO,
                                PAPER | TOBACCO};
  static const int matching_smoker[] = {TOBACCO, PAPER, MATCH};

  uthread_mutex_lock(a->mutex);
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 3;
    signal_count[matching_smoker[r]]++;
    int c = choices[r];
    if (c & MATCH) {
      VERBOSE_PRINT("match available\n");
      uthread_cond_signal(a->match);
    }
    if (c & PAPER) {
      VERBOSE_PRINT("paper available\n");
      uthread_cond_signal(a->paper);
    }
    if (c & TOBACCO) {
      VERBOSE_PRINT("tobacco available\n");
      uthread_cond_signal(a->tobacco);
    }
    VERBOSE_PRINT("agent is waiting for smoker to smoke\n");
    uthread_cond_wait(a->smoke);
  }
  uthread_mutex_unlock(a->mutex);
  return NULL;
}

struct Recursos {
  struct Agent *agente;
  int pape;
  int fos;
  int tob;
};
struct Fumador {
  struct Recursos *r;
  int tipo;
};
struct Recursos *rec;

void crearRec(struct Agent *a) {
  rec = malloc(sizeof(*rec));
  rec->agente = a;
  rec->pape = 0;
  rec->fos = 0;
  rec->tob = 0;
}

void *crearFumador(int tipo) {
  struct Fumador *ret = malloc(sizeof(*ret));
  ret->r = rec;
  ret->tipo = tipo;
  return ret;
}

void *fumar(void *av) {
  struct Fumador *f = av;
  struct Agent *a = rec->agente;
  uthread_mutex_lock(a->mutex);
  while (1) {
    if (f->tipo == MATCH) {
      uthread_cond_wait(a->match);
      rec->fos++;
    } else if (f->tipo == TOBACCO) {
      uthread_cond_wait(a->tobacco);
      rec->tob++;
    } else if (f->tipo == PAPER) {
      uthread_cond_wait(a->paper);
      rec->pape++;
    }
    if (rec->fos > 0) {
      if (rec->pape > 0) {
        smoke_count[TOBACCO]++;
        rec->pape--;
        rec->fos--;
      } else if (rec->tob > 0) {
        smoke_count[PAPER]++;
        rec->tob--;
        rec->fos--;
      }
      uthread_cond_signal(a->smoke);

    } else if (rec->pape > 0 && rec->tob > 0) {
      smoke_count[MATCH]++;
      rec->pape--;
      rec->tob--;
      uthread_cond_signal(a->smoke);
    }
  }
  uthread_mutex_unlock(a->mutex);
  return NULL;
}

//
// TODO
// You will probably need to add some procedures and struct etc.
//

int main(int argc, char **argv) {
  uthread_init(1);
  struct Agent *a = createAgent();
  crearRec(a);
  uthread_t smokerT = uthread_create(fumar, crearFumador(1));
  uthread_t smokerP = uthread_create(fumar, crearFumador(2));
  uthread_t smokerM = uthread_create(fumar, crearFumador(4));
  uthread_join(uthread_create(agent, a), 0);
  assert(signal_count[MATCH] == smoke_count[MATCH]);
  assert(signal_count[PAPER] == smoke_count[PAPER]);
  assert(signal_count[TOBACCO] == smoke_count[TOBACCO]);
  assert(smoke_count[MATCH] + smoke_count[PAPER] + smoke_count[TOBACCO] ==
  NUM_ITERATIONS);
  printf("Smoke counts: %d matches, %d paper, %d tobacco\n", smoke_count[MATCH],
         smoke_count[PAPER], smoke_count[TOBACCO]);
}