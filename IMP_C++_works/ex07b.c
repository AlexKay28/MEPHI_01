#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mycom.h"
#include "mynet.h"
#include <pthread.h>

typedef struct tag_data_t {
  int n, nt, mt;
  double a, b, s, *sum;
} data_t;

int nt = 2, mt;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_t* threads;
data_t* data;

int np, mp, nl, ier, lp;
char pname[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;
double tick, t1, t2, t3;

double a = 0;
double b = 1;
int ni = 1000000000;
double sum = 0;

//сама функция которую нужно проигнтегрировать
double f1(double x);
double f1(double x) { return 4.0/(1.0+x*x); }

//метод(функция) интегрирующая выраженин методом центральных прямоугольников

// double myjob(int mp);
// double myjob(int mp)
// {
//   int n1; double a1, b1, h1, s1;

//   n1 = ni / np;
//   h1 = (b - a) / np;
//   a1 = a + h1 * mp;
//   if (mp<np-1) b1 = a1 + h1; else b1 = b;

//   s1 = integrate(f1,a1,b1,n1);

//   return s1;
// }

void* myjobt(void* d);
void* myjobt(void* d)
{
  int n1; double a1, b1, h1;
  data_t* dd = (data_t *)d; 

  n1 = dd->n / dd->nt;

  h1 = (dd->b - dd->a) / dd->nt;

  a1 = dd->a + h1 * dd->mt;

  if (dd->mt<dd->nt-1) b1 = a1 + h1; else b1 = dd->b;

  dd->s = integrate(f1,a1,b1,n1);

  printf("mt=%d a1=%le b1=%le n1=%d s1=%le\n",dd->mt,a1,b1,n1,dd->s);

  pthread_mutex_lock(&mut); // lock

  *dd->sum += dd->s;

  pthread_mutex_unlock(&mut); // unlock

  return 0;
}

void ThreadInit(np);
void ThreadInit(np)
{
  int i;

  if (!(threads = (pthread_t*) malloc(nt*sizeof(pthread_t))))
    myerr("Not enough memory",1);

  if (!(data = (data_t*) malloc(nt*sizeof(data_t))))
    myerr("Not enough memory",1);

  for (i=np-1; i == np; i++){
    (data+i)->a=a;
    (data+i)->b=b;
    (data+i)->n=ni;
    (data+i)->nt=nt;
    (data+i)->mt=i;
    (data+i)->sum = &sum;
    if (pthread_create(threads+i, 0, myjobt, (void*)(data+i)))
      myerr("Can not create thread",2);
  }

  return;
}

void ThreadDone(np);
void ThreadDone(np)
{
  int i;

  for (i=np-1; i == np; i++)
    if (pthread_join(threads[i],0))
      myerr("Can not wait thread",3);

  free(data);

  free(threads);

  return;
}

int main(int argc, char* argv[])
{
  //what's this?? where can I find some code of it??
  MyNetInit(&argc,&argv,&np/2,&mp,&nl,pname,&tick); 

  fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
  sleep(1);

  if (np<2) {
    t1 = MPI_Wtime();
    nt = 2; mt = 0;
    ThreadInit();
    ThreadDone();
    t2 = MPI_Wtime();
    t3 = t2;
  }
  else {
    int i; double p;

    t1 = MPI_Wtime();
    nt = 2; mt = 0;
    ThreadInit();
    ThreadDone();
    t2 = MPI_Wtime();

    if (mp==0)
      for (i=1; i<np; i++) {
        MPI_Recv(&p, 1, MPI_DOUBLE, i, MY_TAG, MPI_COMM_WORLD, &status);
        sum = sum + p;
      }
    else
      MPI_Send(&sum, 1, MPI_DOUBLE, 0, MY_TAG, MPI_COMM_WORLD);

    //ожидание окончания процесса до того момента пока он не будем вызван всеми 
    //процессами коммуникатора
    MPI_Barrier(MPI_COMM_WORLD);
    t3 = MPI_Wtime();
  }

  t1 = t2 - t1;
  t2 = t3 - t2;
  t3 = t1 + t2;

  fprintf(stderr,"mp=%d t1=%lf t2=%lf t3=%lf int=%22.15le\n",mp,t1,t2,t3,sum);

  MPI_Finalize();
  return 0;
}
