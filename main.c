#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define NN 100000
static inline int randInt() { return rand() % 3315; }
typedef struct name_ {
  int a;
  int b;
  int c;
  int d;
  int e;
} name;
void meow(size_t N, name arr[]) {
  for (size_t i = 0; i < N; ++i) {
    arr[i].a = randInt();
    arr[i].b = randInt();
    arr[i].c = randInt();
    arr[i].d = arr[i].e = 0;
  }
}
// a+b=d
void *FirstFunc(void *args) {
  // sleep(1);
  name *arr = (name *)args;
  for (size_t i = 0; i < NN; ++i) {
    arr[i].d = arr[i].a + arr[i].b;
  }
  // pthread_exit(NULL);
}
// b+c=e
void *SecondFunc(void *args) {
  // sleep(1);
  name *arr = (name *)args;
  for (size_t i = 0; i < NN; ++i) {
    arr[i].e = arr[i].c + arr[i].b;
  }
  // pthread_exit(NULL);
}
void printer(name *arr) {
  for (size_t i = 0; i < NN; ++i) {
    printf("%lu) %d %d %d %d %d\n", i, arr[i].a, arr[i].b, arr[i].c, arr[i].d,
           arr[i].e);
  }
}
int main(void) {
  struct timespec begin, end;

  clock_gettime(CLOCK_REALTIME, &begin);

  name arr[NN];
  size_t N = sizeof(arr) / sizeof(name);
  meow(N, arr);
  pthread_t threadFirst, threadSecond;
  int firstId = pthread_create(&threadFirst, NULL, (void *)FirstFunc, arr);
  int secontId = pthread_create(&threadSecond, NULL, (void *)SecondFunc, arr);
  pthread_join(threadFirst, NULL);
  pthread_join(threadSecond, NULL);
  // printer(arr);
  clock_gettime(CLOCK_REALTIME, &end);

  long seconds = end.tv_sec - begin.tv_sec;
  long nanoseconds = end.tv_nsec - begin.tv_nsec;
  double elapsed = seconds + nanoseconds * 1e-9;

  printf("Threads: The elapsed time is %.3f milliseconds\n%d", elapsed * 1000);

  struct timespec begin1, end1;

  clock_gettime(CLOCK_REALTIME, &begin1);

  name arr1[NN];
  size_t N1 = sizeof(arr1) / sizeof(name);
  meow(N1, arr1);
  FirstFunc(arr1);
  SecondFunc(arr1);
  // printer(arr1);

  clock_gettime(CLOCK_REALTIME, &end1);

  long secondss = end1.tv_sec - begin1.tv_sec;
  long nanosecondss = end1.tv_nsec - begin1.tv_nsec;
  double elapseds = secondss + nanosecondss * 1e-9;

  printf("Without: The elapsed time is %.3f milliseconds\n", elapseds * 1000);

  return 0;
}
