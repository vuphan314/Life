#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <pthread.h>


#define BIT(_i) ((0x01)<<(_i))


/*******************************************************/
double sTime()
{ static struct  timeval  this_tv;
  static struct  timezone dumbTZ;
  double t;

  gettimeofday(&this_tv, &dumbTZ);
  t = this_tv.tv_sec + 0.000001*this_tv.tv_usec;
  return t;
}


/******************************************************************/
void GOL_iterate(uint8_t *out, uint8_t *in)
/* 'in' is an array of 7 entries representing a GOL board in the
    low 7 bits of each entry; e.g., the first row is the low 7 bits
    of in[0], and so on.
   'out' should be allocated to hold 5 uint8_t's, and it will be the
    resulting 5x5 grid (the middle of the given 7x7) represented in the same way.
    This function is not meant to be efficient at all. It is used:
    (1) To do some precomputations, and
    (2) to validate the fast function we will write for doing the same.
*/
{ int nbrs[5][5], i, j;
  int inX, inY;

  for (i=0; i<5; i++)
    for (j=0; j<5; j++)
      nbrs[i][j] = 0;

  for (i=0; i<5; i++) {
    for (j=0; j<5; j++) {
      inX = j+1;
      inY = i+1;
      /* The row above (i,j): */
      if (in[inY-1]&BIT(inX-1)) nbrs[i][j] += 1;
      if (in[inY-1]&BIT(inX)) nbrs[i][j] += 1;
      if (in[inY-1]&BIT(inX+1)) nbrs[i][j] += 1;
      /* The row of (i,j): */
      if (in[inY]&BIT(inX-1)) nbrs[i][j] += 1;
      if (in[inY]&BIT(inX+1)) nbrs[i][j] += 1;
      /* The row below (i,j): */
      if (in[inY+1]&BIT(inX-1)) nbrs[i][j] += 1;
      if (in[inY+1]&BIT(inX)) nbrs[i][j] += 1;
      if (in[inY+1]&BIT(inX+1)) nbrs[i][j] += 1;
    }
  }
  out[0]=out[1]=out[2]=out[3]=out[4]=0x00;
  for (i=0; i<5; i++) {
    for (j=0; j<5; j++) {
      inX = j+1;
      inY = i+1;
      if (in[inY]&BIT(inX)) {
        /* This cell was previously alive. What happens to it now? */
        if ((nbrs[i][j]==2) || (nbrs[i][j]==3))
          out[i] |= BIT(j);
      } else {
        /* This cell was previously dead. Does it spring to life? */
          if (nbrs[i][j]==3)
            out[i] |= BIT(j);
      }
    }
  }
}

uint8_t *GOL=NULL;

/******************************************************************/
void GOL_fast_init()
{ uint8_t g7[7], g5[5];
  uint8_t r1, r2, r3;
  int     i;

  for (i=0; i<7; i++)
    g7[i]=0x00;

  GOL = (uint8_t *)malloc(128*128*128*sizeof(uint8_t));


  for (r1=0; r1<128; r1++) {
    g7[0]=r1;
    for (r2=0; r2<128; r2++) {
      g7[1]=r2;
      for (r3=0; r3<128; r3++) {
        g7[2]=r3;
        GOL_iterate(g5, g7);
        GOL[r1 + (r2<<7) + (r3<<14)] = g5[0];
      }
    }
  }
}


/******************************************************************/
inline void GOL_iterate_fast(uint8_t *out, uint8_t *in)
{ uint32_t index;

  index = in[0] + (in[1]<<7) + (in[2]<<14);
  out[0] = GOL[index];
  index = (index>>7) + (in[3]<<14);
  out[1] = GOL[index];
  index = (index>>7) + (in[4]<<14);
  out[2] = GOL[index];
  index = (index>>7) + (in[5]<<14);
  out[3] = GOL[index];
  index = (index>>7) + (in[6]<<14);
  out[4] = GOL[index];
}

/******************************************************************/
void GOL_iterate_fast2(uint8_t *out, uint64_t G)
/* G is a 49 bit integer representing a 7x7 grid. */
{ uint64_t index;

  index = G;
  out[0] = GOL[index&0x00000000001FFFFFULL];
  index = (index>>7);
  out[1] = GOL[index&0x00000000001FFFFFULL];
  index = (index>>7);
  out[2] = GOL[index&0x00000000001FFFFFULL];
  index = (index>>7);
  out[3] = GOL[index&0x00000000001FFFFFULL];
  index = (index>>7);
  out[4] = GOL[index&0x00000000001FFFFFULL];
}

/******************************************************************/
int checkAll7x7Images2(uint8_t *hash, uint64_t first, uint64_t stop, int id)
/* 'first' and 'stop' are 49 bit integers, representing 7x7 GOL boards
    as follows: bits 0-6 are row 0, bits 7-13 are row 1,...
    With respect to the
    natural ordering (considering these as 49-bit integers), this function
    will run over all boards G, with first7 <= G < stop7, and
    (i) compute the image I of G,
    (ii) hash out I in the array 'hash' (turn on bit I).
    'hash' is an array of size 2^{25} / 2^3 = 2^{22}.
*/
{ uint64_t G, index;
  uint32_t h;
  uint8_t  image[5], im0, im1, im2, im3, im4;
  int      i, k;
  double   itsPerSecond, t0, t1, timeLeft;


  G=first;
  t0 = sTime();
  k=1;
  for (G=first; G<stop; G++) {
    /* Compute the image: */
    index = G;
    h = GOL[index&0x00000000001FFFFFULL];
    index = (index>>7);
    h *= 32;
    h += GOL[index&0x00000000001FFFFFULL];
    index = (index>>7);
    h *= 32;
    h += GOL[index&0x00000000001FFFFFULL];
    index = (index>>7);
    h *= 32;
    h += GOL[index&0x00000000001FFFFFULL];
    index = (index>>7);
    h *= 32;
    h += GOL[index&0x00000000001FFFFFULL];
    /* hash out the image. */
    hash[h/8] |= BIT(h%8);
//printf("hashed out h=%d\n", h);

    /* Report speed info periodically. */
      if ( (G&0x0FFFFFFF) == 0) { // 0xFFFFFFFF
        t1 = sTime();
        itsPerSecond = (G-first)/(t1-t0);
        timeLeft = ((stop-G)/itsPerSecond)/(3600.0*24.0);
        printf("  CPU %d: expected time remaining to completion %1.4lf days\n", id, timeLeft);
      }
  }
}

/******************************************************************/
void printBoard(FILE *fp, uint8_t *g, int size)
{ int i, j;

  for (i=0; i<size; i++) {
    for (j=0; j<size; j++) {
      if (g[i]&BIT(j))
        fprintf(fp, "1 ");
      else fprintf(fp, "0 ");
    }
    fprintf(fp, "\n");
  }
}

/******************************************************************/
void test_GOL_iterate()
{ int     i, j, numtests=1000000;
  uint8_t g0[7], g1[5], G1[5], g2[5];
  uint64_t G;


#if 0
  g0[0]=0x7f;
  g0[1]=0x00;
  g0[2]=0x00;
  g0[3]=0x7f;
  g0[4]=0x00;
  g0[5]=0x00;
  g0[6]=0x7f;
  G= ((uint64_t)g0[0])+(((uint64_t)g0[1])<<7)+(((uint64_t)g0[2])<<14)+
       (((uint64_t)g0[3])<<21)+(((uint64_t)g0[4])<<28)+(((uint64_t)g0[5])<<35)+
       (((uint64_t)g0[6])<<42);
  GOL_iterate(g1, g0); /* Reference function. */
  GOL_iterate_fast2(G1, G);
  printf("Initial board:\n");
  printBoard(stdout, g0, 7);

  printf("\nReference function gives:\n");
  printBoard(stdout, g1, 5);
  printf("\nFast function gives:\n");
  printBoard(stdout, G1, 5);
#endif


  GOL_iterate_fast2(G1, G);

  for (j=0; j<numtests; j++) {
    for (i=0; i<7; i++)
      g0[i] = lrand48()&0x7F;
    GOL_iterate(g1, g0); /* Reference function. */
    GOL_iterate_fast(G1, g0); /* Function being tested. */
    G= ((uint64_t)g0[0])+(((uint64_t)g0[1])<<7)+(((uint64_t)g0[2])<<14)+
       (((uint64_t)g0[3])<<21)+(((uint64_t)g0[4])<<28)+(((uint64_t)g0[5])<<35)+
       (((uint64_t)g0[6])<<42);
    GOL_iterate_fast2(G1, G);
    if ((G1[0]!=g1[0])||(G1[1]!=g1[1])||(G1[2]!=g1[2])||(G1[3]!=g1[3])||(G1[4]!=g1[4])) {
      printf("test_GOL_iterate() Error, results don't match!\n");
      printf("Initial board:\n");
      printBoard(stdout, g0, 7);

      printf("\nReference function gives:\n");
      printBoard(stdout, g1, 5);
      printf("\nFast function gives:\n");
      printBoard(stdout, G1, 5);
      exit(-1);
    }
  }

}

typedef struct {
  uint64_t start;
  uint64_t stop;
  uint8_t *hash;
  int      id;
} job_t;

/*******************************************/
void *launch_job(void *ptr)
{ job_t *J = (job_t *)ptr;


  checkAll7x7Images2(J->hash, J->start, J->stop, J->id);
  return NULL;
}


#define MAX_CORES 8
/*******************************************/
int main(int argC, char *args[])
{ double    t0, t1;
  job_t     job[MAX_CORES];
  uint32_t  k, hashSize;
  int       i, j, numCores = atoi(args[1]);
  pthread_t threads[MAX_CORES];
  int       retVal[MAX_CORES];
  FILE     *fp;
  uint8_t   board[5];
  long      missing;
  uint64_t  lastBoard;

  srand48(time(NULL));
  printf("Doing precomputations..."); fflush(stdout);
  GOL_fast_init();
  printf("done.\n");

  printf("Testing fast iteration function..."); fflush(stdout);
  test_GOL_iterate();
  printf("passed.\n");

  hashSize = 0x00000001<<22;
  for (i=0; i<numCores; i++) {
    job[i].hash = (uint8_t *)malloc(hashSize);
    memset(job[i].hash, 0x00, hashSize);
  }

  t0 = sTime();

  /* Divide the work equally among available cores. */
  lastBoard =  0x0000000000000001ULL<<49;
  job[0].start = 0;
  job[0].stop = lastBoard/numCores;
  job[0].id = 0;
  for (i=1; i<numCores; i++) {
    job[i].start = job[i-1].stop;
    job[i].stop = job[i].start + lastBoard/numCores;
    job[i].id = i;
  }
  /* but adjust the stopping point on the last one in case of rounding error. */
  job[numCores-1].stop = lastBoard;

  printf("Assigning tasks:\n");
  for (i=0; i<numCores; i++) {
    printf("CPU %d : grids 0x%lx through 0x%lx\n", i, job[i].start, job[i].stop);
  }

  /* Now launch the threads. */
  for (i=0; i<numCores; i++) {
    retVal[i] = pthread_create(&threads[i], NULL, launch_job, (void *)&job[i]);
    if (retVal[i]) {
      fprintf(stderr, "Error: pthread_create() failed with error code %d\n", retVal[i]);
      exit(-1);
    }
  }
  /* Wait for all threads to finish. */
  for (i=0; i<numCores; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Now 'OR' all of the hashes together to see if anything is missing from the image. */
  for (k=0; k<hashSize; k++) {
    for (j=1; j<numCores; j++) {
      job[0].hash[k] |= job[j].hash[k];
    }
  }

  fp = fopen("gol5.out", "a");
  if (fp)
    fprintf(fp, "*********** Run completed **************\n");
  missing = 0;
  for (k=0; k<(0x00000001<<25); k++) {
    if ((job[0].hash[ k/8]&BIT(k%8)) == 0) {
      missing++;
      printf("k=%x is missing from the image\n", k);
      board[0] = (k>>20)&0xFF;
      board[1] = (k>>15)&0xFF;
      board[2] = (k>>10)&0xFF;
      board[3] = (k>>5)&0xFF;
      board[4] = k&0xFF;
      printBoard(stdout, board, 5);
      if (fp) {
        fprintf(fp, "k=%x is missing from the image\n", k);
        printBoard(fp, board, 5);
      }
    }
  }
  printf("There are %ld 5x5 boards not in the image.\n", missing);
  printf("There are %ld 5x5 boards in the image.\n", (0x00000001<<25)-missing);

  if (fp) {
    fprintf(fp, "There are %ld 5x5 boards not in the image.\n", missing);
    fprintf(fp, "There are %ld 5x5 boards in the image.\n", (0x00000001<<25)-missing);
    fclose(fp);
  }

}
