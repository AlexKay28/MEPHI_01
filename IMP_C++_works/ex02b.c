#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include "mycom.h"

double f(double x);
double f(double x)
{
  return (4.0 / (1.0 + x*x));
}

double f1(double x);
double f1(double x)
{
  return (4.0 / (1.0 + x+x*x));
}

double f2(double x);
double f2(double x)
{
  return (4.0 / (1.0 + x*x - x));
}

double f3(double x);
double f3(double x)
{
  return (4.0 / (1.0 + x*pow(x, 2)));
}

double f4(double x);
double f4(double x)
{
  return (4.0 / (1.0 + x*exp(x)));
}

double f5(double x);
double f5(double x)
{
  return (4.0 / (1.0 + x*log(x)));
}

double f6(double x);
double f6(double x)
{
  return (4.0 / (1.0 + x*sin(x)));
}

int main(int argc, char *argv[])
{	
  int nc=1000000000;
  double t1, t2, dt, sum;
  
  // 1st comparing
  t1 = mytime(0); sum = integrate(f,0.0,1.0,nc); t1 = mytime(1);
  t2 = mytime(0); sum = integrate(f1,0.0,1.0,nc); t2 = mytime(1);
  dt = 1.0/dabs(t2-t1);
  printf("Time: %lf %lf sec || SUM. perf.: %le GFlops\n",t1,t2,dt);
  
  // 2nd comparing
  t1 = mytime(0); sum = integrate(f,0.0,1.0,nc); t1 = mytime(1);
  t2 = mytime(0); sum = integrate(f2,0.0,1.0,nc); t2 = mytime(1);
  dt = 1.0/dabs(t2-t1);
  printf("Time: %lf %lf sec || SUB. perf.: %le GFlops\n",t1,t2,dt);
  
  // 3rd comparing
  t1 = mytime(0); sum = integrate(f,0.0,1.0,nc); t1 = mytime(1);
  t2 = mytime(0); sum = integrate(f3,0.0,1.0,nc); t2 = mytime(1);
  dt = 1.0/dabs(t2-t1);
  printf("Time: %lf %lf sec || POW. perf.: %le GFlops\n",t1,t2,dt);
  
  // 4th comparing
  t1 = mytime(0); sum = integrate(f,0.0,1.0,nc); t1 = mytime(1);
  t2 = mytime(0); sum = integrate(f4,0.0,1.0,nc); t2 = mytime(1);
  dt = 1.0/dabs(t2-t1);
  printf("Time: %lf %lf sec || EXP. perf.: %le GFlops\n",t1,t2,dt);
  
  // 5th comparing
  t1 = mytime(0); sum = integrate(f,0.0,1.0,nc); t1 = mytime(1);
  t2 = mytime(0); sum = integrate(f5,0.0,1.0,nc); t2 = mytime(1);
  dt = 1.0/dabs(t2-t1);
  printf("Time: %lf %lf sec || LOG. perf.: %le GFlops\n",t1,t2,dt);
  
  t1 = mytime(0); sum = integrate(f,0.0,1.0,nc); t1 = mytime(1);
  t2 = mytime(0); sum = integrate(f6,0.0,1.0,nc); t2 = mytime(1);
  dt = 1.0/dabs(t2-t1);
  printf("Time: %lf %lf sec || SIN. perf.: %le GFlops\n",t1,t2,dt);

  return 0;
}
