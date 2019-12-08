//
// Poisson equation:
//
// u_xx + u_yy = - f(x,y), (x,y) in D = [0,1]x[0,1], u = 0, (x,y) in dD.
//
// Method: Fourier Transform
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "mycom.h"
#include "mynet.h"
#include "myio.h"

 int np, mp, nl, ier, lp;
 int np1, np2, mp1, mp2;
 int mp_l, mp_r, mp_b, mp_t;
 char pname[MPI_MAX_PROCESSOR_NAME];
 char vname[10] = "ex15a";
 char sname[20];
 MPI_Status status;
 union_t buf;
 double tick, t1, t2, t3;

 FILE *Fi = NULL;
 FILE *Fo = NULL;

 int n1, n2;
 double pi2;

double f(double x1, double x2);
double f(double x1, double x2) {
  return pi2*dsin(pi*x1)*dsin(pi*x2);
}

double u(double x1, double x2);
double u(double x1, double x2) {
  return dsin(pi*x1)*dsin(pi*x2);
}

int main(int argc, char *argv[])
{
  int i1, i2, k1, k2, n1p, n2p, n12p;
  int n, m, i11, i12, i21, i22, nc1, nc2, nc1m, nc2m, nc12;
  double h1, h2, h12, h22, h0, s0, s1, s2;
  double *xx1, *xx2, *cv1, *cv2, *gg, *yy;

  MyNetInit(&argc,&argv,&np,&mp,&nl,pname,&tick);

  fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
  sleep(1);

  sprintf(sname,"%s.p%02d",vname,mp);
  ier = fopen_m(&Fo,sname,"wt");
  if (ier!=0) mpierr("Protocol file not opened",1);

  fprintf(Fo,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);

  if (mp==0) {
    sprintf(sname,"%s.d",vname);
    ier = fopen_m(&Fi,sname,"rt");
    if (ier!=0) mpierr("Data file not opened",2);
    fscanf(Fi,"n1=%d\n",&n1);
    fscanf(Fi,"n2=%d\n",&n2);
    fscanf(Fi,"lp=%d\n",&lp);
    fclose_m(&Fi);
    if (argc>1) sscanf(argv[1],"%d",&n1);
    if (argc>2) sscanf(argv[2],"%d",&n2);
    if (argc>3) sscanf(argv[3],"%d",&lp);
  }

  if (np>1) {
    if (mp==0) {
      buf.idata[0] = n1;
      buf.idata[1] = n2;
      buf.idata[2] = lp;
    }
    MPI_Bcast(buf.ddata,2,MPI_DOUBLE,0,MPI_COMM_WORLD);
    if (mp>0) {
      n1 = buf.idata[0];
      n2 = buf.idata[1];
      lp = buf.idata[2];
    }
  }

  My2DGrid(np,mp,n1,n2,&np1,&np2,&mp1,&mp2);
//
// mp = np1 * mp2 + mp1
//
  if (mp1 ==     0) mp_l = -1; else mp_l = mp - 1;
  if (mp1 == np1-1) mp_r = -1; else mp_r = mp + 1;
  if (mp2 ==     0) mp_b = -1; else mp_b = mp - np1;
  if (mp2 == np2-1) mp_t = -1; else mp_t = mp + np1;

  MyRange(np1,mp1,0,n1,&i11,&i12,&nc1); nc1m = nc1-1;
  MyRange(np2,mp2,0,n2,&i21,&i22,&nc2); nc2m = nc2-1;
  nc12 = nc1 * nc2;

  fprintf(Fo,"Grid=%dx%d coord=(%d,%d)\n",np1,np2,mp1,mp2);
  fprintf(Fo,"i11=%d i12=%d nc1=%d\n",i11,i12,nc1);
  fprintf(Fo,"i21=%d i22=%d nc2=%d\n",i11,i12,nc2);

  n1p = n1+1; n2p = n2+1; n12p = n1p*n2p;

  h1 = 1.0/n1; h2 = 1.0/n2;
  h12 = 1.0/(h1*h1); h22 = 1.0/(h2*h2); h0 = 2.0*h1*h2;
  pi2 = 2.0*pi*pi;

  fprintf(Fo,"n1=%d n2=%d h1=%le h2=%le\n",n1,n2,h1,h2);

  if (mp == 0) {
    fprintf(stderr,"n1=%d n2=%d h1=%le h2=%le\n",n1,n2,h1,h2);
    fprintf(stderr,"Grid=%dx%d\n",np1,np2);
  }

  t1 = MPI_Wtime();

  xx1 = (double*)(malloc(sizeof(double)*n1p));
  xx2 = (double*)(malloc(sizeof(double)*n2p));

  cv1 = (double*)(malloc(sizeof(double)*n1p));
  cv2 = (double*)(malloc(sizeof(double)*n2p));

  gg = (double*)(malloc(sizeof(double)*n12p));
  yy = (double*)(malloc(sizeof(double)*n12p));

  for (i1=0; i1<=n1; i1++) xx1[i1] = h1 * i1;
  for (i2=0; i2<=n2; i2++) xx2[i2] = h2 * i2;

  for (i1=0; i1<=n1; i1++) cv1[i1] = 1.0 - dcos(pi*i1*h1);
  for (i2=0; i2<=n2; i2++) cv2[i2] = 1.0 - dcos(pi*i2*h2);

//
// step 1:
//
  if (np<2) {
    for (i2=0; i2<=n2; i2++) {
      for (i1=0; i1<=n1; i1++) {
        m = n1p * i2 + i1;
        yy[m] = f(xx1[i1],xx2[i2]);
      }
    }
  }
  else {
    for (i2=0; i2<=n2; i2++) {
      for (i1=0; i1<=n1; i1++) {
        m = n1p * i2 + i1;
        s0 = 0.0;
        if ((i11<=i1) && (i1<=i12) && (i21<=i2) && (i2<=i22)) 
          s0 = f(xx1[i1],xx2[i2]);
        gg[m] = s0;
      }
    }

    MPI_Allreduce(gg,yy,n12p,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
  }

  if (lp>0) {
    for (i2=0; i2<=n2; i2++) {
      for (i1=0; i1<=n1; i1++) {
        m = n1p * i2 + i1;
        fprintf(Fo,"i1=%8d i2=%8d y=%le\n",i1,i2,yy[m]);
      }
    }
  }
//
// step 2:
//
  if (np<2) {
    for (k2=0; k2<=n2; k2++) {
      for (k1=0; k1<=n1; k1++) {
        n = n1p * k2 + k1;
        s0 = 0.0;
        for (i2=1; i2<n2; i2++) {
          for (i1=1; i1<n1; i1++) {
            m = n1p * i2 + i1;
            s1 = dsin(pi*k1*xx1[i1]);
            s2 = dsin(pi*k2*xx2[i2]);
            s0 += yy[m]*s1*s2;
          }
        }
        s1 = h12*cv1[k1] + h22*cv2[k2];
        s0 = s0 * h0 / s1;
        gg[n] = s0;
      }
    }
  }
  else {
    for (k2=0; k2<=n2; k2++) {
      for (k1=0; k1<=n1; k1++) {
        n = n1p * k2 + k1;
        s0 = 0.0;

        if ((i11<=k1) && (k1<=i12) && (i21<=k2) && (k2<=i22)) {
          for (i2=1; i2<n2; i2++) {
            for (i1=1; i1<n1; i1++) {
              m = n1p * i2 + i1;
              s1 = dsin(pi*k1*xx1[i1]);
              s2 = dsin(pi*k2*xx2[i2]);
              s0 += yy[m]*s1*s2;
            }
          }
          s1 = h12*cv1[k1] + h22*cv2[k2];
          s0 = s0 * h0 / s1;
        }

        gg[n] = s0;
      }
    }

    MPI_Allreduce(gg,yy,n12p,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

    for (n=0; n<n12p; n++) gg[n] = yy[n];
  }

  if (lp>0) {
    for (k2=0; k2<=n2; k2++) {
      for (k1=0; k1<=n1; k1++) {
        n = n1p * k2 + k1;
        fprintf(Fo,"k1=%8d k2=%8d g=%le\n",k1,k2,gg[n]);
      }
    }
  }
//
// step 3:
//
  if (np<2) {
    for (i2=0; i2<=n2; i2++) {
      for (i1=0; i1<=n1; i1++) {
        m = n1p * i2 + i1;
        s0 = 0.0;
        for (k2=1; k2<n2; k2++) {
          for (k1=1; k1<n1; k1++) {
            n = n1p * k2 + k1;
            s1 = dsin(pi*k1*xx1[i1]);
            s2 = dsin(pi*k2*xx2[i2]);
            s0 += gg[n]*s1*s2;
          }
        }
        yy[m] = s0;
      }
    }
  }
  else {
    for (i2=0; i2<=n2; i2++) {
      for (i1=0; i1<=n1; i1++) {
        m = n1p * i2 + i1;
        s0 = 0.0;

        if ((i11<=i1) && (i1<=i12) && (i21<=i2) && (i2<=i22)) {
          for (k2=1; k2<n2; k2++) {
            for (k1=1; k1<n1; k1++) {
              n = n1p * k2 + k1;
              s1 = dsin(pi*k1*xx1[i1]);
              s2 = dsin(pi*k2*xx2[i2]);
              s0 += gg[n]*s1*s2;
            }
          }
        }

        yy[m] = s0;
      }
    }

    MPI_Allreduce(yy,gg,n12p,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);

    for (n=0; n<n12p; n++) yy[n] = gg[n];
  }

  if (lp>0) {
    for (i2=0; i2<=n2; i2++) {
      for (i1=0; i1<=n1; i1++) {
        m = n1p * i2 + i1;
        fprintf(Fo,"i1=%8d i2=%8d y=%le\n",i1,i2,yy[m]);
      }
    }
  }
//
// fine:
//
  t1 = MPI_Wtime() - t1;

  s2 = 0.0;

  for (i2=0; i2<=n2; i2++) {
    for (i1=0; i1<=n1; i1++) {
      m = n1p * i2 + i1;
      s0 = u(xx1[i1],xx2[i2]);
      s1 = dabs(yy[m]-s0);
      s2 = dmax(s2,s1);
      if (lp>0)
        fprintf(Fo,"x1=%le x2=%le y=%le u=%le d=%le\n",
          xx1[i1],xx2[i2],yy[m],s0,s1);
    }
  }

  fprintf(Fo,"t1=%le dmax=%le\n",t1,s2);
  if (mp==0)
    fprintf(stderr,"np=%d (%dx%d) n1=%8d n2=%8d dmax=%le time=%le\n",
      np,np1,np2,n1,n2,s2,t1);

  if (mp==0 && lp>0) {
    sprintf(sname,"%s_%02d.dat",vname,np);
    OutFun2D(sname,0,n1p,n2p,xx1,xx2,yy);
  }

  MPI_Finalize();
  return 0;
}
