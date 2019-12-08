//
// Poisson equation:
//
// u_xx + u_yy = - f(x,y), (x,y) in D = [0,1]x[0,1], u = 0, (x,y) in dD.
//
// Method: Simple iterative procedure + Chebyshov's speedup
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "mycom.h"
#include "mynet.h"
#include "myrand.h"
#include "myio.h"

 int np, mp, nl, ier, lp;
 int np1, np2, mp1, mp2;
 int mp_l, mp_r, mp_b, mp_t;
 char pname[MPI_MAX_PROCESSOR_NAME];
 int lname;
 char vname[1024];
 char sname[1024];
 union_t buf;
 double tick, t1, t2, t3;

 FILE *Fi = NULL;
 FILE *Fo = NULL;

 int n1, n2, im, nc, mc, itm;
 double pi2, eps;

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
  int i, n, m, i1, i2, k1, k2, n1p, n2p, n12p, it;
  int i11, i12, i21, i22, nc1, nc2, nc1m, nc2m, nc12;
  double h1, h2, h12, tau, tau0, g12, g21, rka, dka;
  double s0, s1, s2, s3, s1m, s1p, s2m, s2p;
  double *rr, *xx1, *xx2, *ff, *yy0, *yy1;
  double *rr_l, *ss_l, *rr_r, *ss_r, *rr_b, *ss_b, *rr_t, *ss_t;   

  MyNetInit(&argc,&argv,&np,&mp,&nl,pname,&tick);

  fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
  sleep(1);

  if (argc>1) strcpy(vname,argv[1]);
  else        strcpy(vname,"ex16a");
  lname = strlen(vname);

  if (mp==0) fprintf(stderr,"Base name is %s\n",vname);

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
    fscanf(Fi,"im=%d\n",&im);
    fscanf(Fi,"lp=%d\n",&lp);
    fclose_m(&Fi);
    if (argc>2) sscanf(argv[2],"%d",&n1);
    if (argc>3) sscanf(argv[3],"%d",&n2);
    if (argc>4) sscanf(argv[4],"%d",&im);
    if (argc>5) sscanf(argv[5],"%d",&lp);
  }

  if (np>1) {
    if (mp==0) {
      buf.idata[0] = n1;
      buf.idata[1] = n2;
      buf.idata[2] = im;
      buf.idata[3] = lp;
    }
    MPI_Bcast(buf.ddata,2,MPI_DOUBLE,0,MPI_COMM_WORLD);
    if (mp>0) {
      n1 = buf.idata[0];
      n2 = buf.idata[1];
      im = buf.idata[2];
      lp = buf.idata[3];
    }
  }

  pi2 = 2.0*pi*pi;

  n1p = n1+1; n2p = n2+1; n12p = n1p*n2p;
  h1 = 1.0/n1; h2 = 1.0/n2;

  s0 = dmin(h1,h2); eps = pow(s0,3); eps = dmax(eps,1e-14);
  s0 = log(1.0/eps); s0 = s0*n12p;
  if (s0>2000000000) itm = 2000000000; else itm = (int)s0;

  tau0 = 0.25;
  if (argc>6) sscanf(argv[6],"%le",&tau0);
  tau = tau0;

  mc = (int)(0.5*log(1.0*n12p)/log(2.0));
  if (argc>7) sscanf(argv[7],"%d",&mc);
  if (mc<1){ mc=0; nc=1; } else nc = 1 << mc;

  g12 = h1/h2; g21 = h2/h1; s0 = g12+g21;
  g12 = 0.5*g12/s0;
  g21 = 0.5*g21/s0;
  h12 = 0.5*h1*h2/s0;

  My2DGrid(np,mp,n1,n2,&np1,&np2,&mp1,&mp2);
//
// mp = np1 * mp2 + mp1
//
  if (mp1 ==     0) mp_l = -1; else mp_l = mp - 1;
  if (mp1 == np1-1) mp_r = -1; else mp_r = mp + 1;
  if (mp2 ==     0) mp_b = -1; else mp_b = mp - np1;
  if (mp2 == np2-1) mp_t = -1; else mp_t = mp + np1;

  MyRange(np1,mp1,0,n1,&i11,&i12,&nc1); nc1m = nc1 - 1;
  MyRange(np2,mp2,0,n2,&i21,&i22,&nc2); nc2m = nc2 - 1;
  nc12 = nc1 * nc2;

  fprintf(Fo,"n1=%d n2=%d mc=%d nc=%d\n",n1,n2,mc,nc);
  fprintf(Fo,"n12=%d itm=%d h1=%le h2=%le eps=%le\n",n12p,itm,h1,h2,eps);
  fprintf(Fo,"Grid=%dx%d coords=(%d,%d)\n",np1,np2,mp1,mp2);
  fprintf(Fo,"mp_l=%d mp_r=%d mp_b=%d mp_t=%d\n",mp_l,mp_r,mp_b,mp_t);
  fprintf(Fo,"i11=%d i12=%d nc1=%d\n",i11,i12,nc1);
  fprintf(Fo,"i21=%d i22=%d nc2=%d\n",i21,i22,nc2);

  if (mp==0){
    fprintf(stderr,"n1=%d n2=%d mc=%d nc=%d n12=%d itm=%d\n",n1,n2,mc,nc,n12p,itm);
    fprintf(stderr,"grid=%dx%d h1=%10.3le h2=%10.3le eps=%10.3le\n",np1,np2,h1,h2,eps);
  }

  rr = (double*)(malloc(sizeof(double)*nc));

  mychebset(mc,nc,rr);

  if (lp>0) for (m=0; m<nc; m++) fprintf(Fo,"m=%d rr=%le\n",m,rr[m]);

  t1 = MPI_Wtime();

  xx1 = (double*)(malloc(sizeof(double)*nc1));
  xx2 = (double*)(malloc(sizeof(double)*nc2));

  ff  = (double*)(malloc(sizeof(double)*nc12));
  yy0 = (double*)(malloc(sizeof(double)*nc12));
  yy1 = (double*)(malloc(sizeof(double)*nc12));

  if (mp_l>=0) {
    rr_l = (double*)(malloc(sizeof(double)*nc2));
    ss_l = (double*)(malloc(sizeof(double)*nc2));
  }

  if (mp_r>=0) {
    rr_r = (double*)(malloc(sizeof(double)*nc2));
    ss_r = (double*)(malloc(sizeof(double)*nc2));
  }

  if (mp_b>=0) {
    rr_b = (double*)(malloc(sizeof(double)*nc1));
    ss_b = (double*)(malloc(sizeof(double)*nc1));
  }

  if (mp_t>=0) {
    rr_t = (double*)(malloc(sizeof(double)*nc1));
    ss_t = (double*)(malloc(sizeof(double)*nc1));
  }

  for (i1=0; i1<nc1; i1++) xx1[i1] = h1 * (i11+i1);
  for (i2=0; i2<nc2; i2++) xx2[i2] = h2 * (i21+i2);

// Initialization of iterations:

  for (m=0; m<nc12; m++)  ff[m] = 0.0;
  for (m=0; m<nc12; m++) yy0[m] = 0.0;
  for (m=0; m<nc12; m++) yy1[m] = 0.0;

  for (k2=0; k2<nc2; k2++) {
    i2 = i21 + k2;
    if ((i2>0) && (i2<n2)) {
      for (k1=0; k1<nc1; k1++) {
        i1 = i11 + k1;
        if ((i1>0) && (i1<n1)) {
          m = nc1 * k2 + k1;
          ff[m] = f(xx1[k1],xx2[k2]);
          yy1[m] = h12 * ff[m];
        }
      }
    }
  }

  if (lp>0) {
    for (k2=0; k2<nc2; k2++) {
      i2 = i21 + k2;
      for (k1=0; k1<nc1; k1++) {
        i1 = i11 + k1;
        m = nc1 * k2 + k1;
        fprintf(Fo,"i1=%d i2=%d x1=%le x2=%le f=%le\n",i1,i2,xx1[k1],xx2[k2],ff[m]);
      }
    }
  }

  it = 0;

// Iterative loop:

  do {

    if (mc>0){ // Chebyshov's speedup
      m = it % nc;
      tau = tau0*rr[m];
    }

    for (m=0; m<nc12; m++) yy0[m] = yy1[m]; // update

    if (np>1) {
      if (mp_l>=0) {
        i1 = 0;
        for (i2=0; i2<nc2; i2++) { m = nc1 * i2 + i1; ss_l[i2] = yy0[m]; }
      }

      if (mp_r>=0) {
        i1 = nc1m; 
        for (i2=0; i2<nc2; i2++) { m = nc1 * i2 + i1; ss_r[i2] = yy0[m]; }
      }

      if (mp_b>=0) {
        i2 = 0;
        for (i1=0; i1<nc1; i1++) { m = nc1 * i2 + i1; ss_b[i1] = yy0[m]; }
      }

      if (mp_t>=0) {
        i2 = nc2m;
        for (i1=0; i1<nc1; i1++) { m = nc1 * i2 + i1; ss_t[i1] = yy0[m]; }
      }

      BndAExch2D(mp_l,nc2,ss_l,rr_l,
                 mp_r,nc2,ss_r,rr_r,
                 mp_b,nc1,ss_b,rr_b,
                 mp_t,nc1,ss_t,rr_t);
    }

    rka = 0.0;

    for (k2=0; k2<nc2; k2++) {
      i2 = i21 + k2;
      if ((i2>0) && (i2<n2)) {
        for (k1=0; k1<nc1; k1++) {
          i1 = i11 + k1;
          if ((i1>0) && (i1<n1)) {
            m = nc1 * k2 + k1;
            s0 = yy0[m];
            if (k1==   0) { if (mp_l>=0) s1m = rr_l[k2]; else s1m = 0.0; } else s1m = yy0[m-1];
            if (k1==nc1m) { if (mp_r>=0) s1p = rr_r[k2]; else s1p = 0.0; } else s1p = yy0[m+1];
            if (k2==   0) { if (mp_b>=0) s2m = rr_b[k1]; else s2m = 0.0; } else s2m = yy0[m-nc1];
            if (k2==nc2m) { if (mp_t>=0) s2p = rr_t[k1]; else s2p = 0.0; } else s2p = yy0[m+nc1];
            s1 = g21 * (s1m + s1p);
            s2 = g12 * (s2m + s2p);
            s3 = h12 * ff[m];
            yy1[m] = s1 + s2 + s3 - s0; // residual
            s0 = dabs(yy1[m]);
            rka = dmax(rka,s0);
          }
        }
      }
    }

    if (np>1) {
      s0 = rka; MPI_Allreduce(&s0,&rka,1,MPI_DOUBLE,MPI_MAX,MPI_COMM_WORLD);
    }

    if ((lp>0) && (mp==0))
      fprintf(stderr,"it=%d tau=%le rka=%le\n",it,tau,rka);

    if (lp>1) {
      fprintf(Fo,"it=%d tau=%le rka=%le\n",it,tau,rka);
      for (k2=0; k2<nc2; k2++) {
        i2 = i21 + k2;
        for (k1=0; k1<nc1; k1++) {
          i1 = i11 + k1;
          m = nc1 * k2 + k1;
          fprintf(Fo,"i1=%8d i2=%8d y0=%le\n",i1,i2,yy0[m]);
        }
      }
    }

    if ((rka<=eps) || (it>=itm)) {
      for (m=0; m<nc12; m++)
        yy1[m] = yy0[m];

      break;
    }

    it = it + 1;

    for (k2=0; k2<nc2; k2++) {
      i2 = i21 + k2;
      if ((i2>0) && (i2<n2)) {
        for (k1=0; k1<nc1; k1++) {
          i1 = i11 + k1;
          if ((i1>0) && (i1<n1)) {
            m = nc1 * k2 + k1;
            yy1[m] = yy0[m] + tau * yy1[m];
          }
        }
      }
    }

  } while (it<=itm);

  dka = 0.0;

  for (k2=0; k2<nc2; k2++) {
    i2 = i21 + k2;
    for (k1=0; k1<nc1; k1++) {
      i1 = i11 + k1;
      m = nc1 * k2 + k1;
      s0 = u(xx1[k1],xx2[k2]);
      s1 = dabs(yy1[m]-s0);
      dka = dmax(dka,s1);
      if (lp>0) fprintf(Fo,"i1=%8d i2=%8d y=%le u=%le d=%le\n",i1,i2,yy1[m],s0,s1);
    }
  }

  if (np>1) {
    s0 = dka; MPI_Allreduce(&s0,&dka,1,MPI_DOUBLE,MPI_MAX,MPI_COMM_WORLD);
  }

  t1 = MPI_Wtime() - t1;

  fprintf(Fo,"it=%d rka=%le dka=%le time=%le\n",it,rka,dka,t1);

  if (mp==0)
    fprintf(stderr,"np=%d (%dx%d) n1=%8d n2=%8d it=%d rka=%le dka=%le time=%le\n",
      np,np1,np2,n1,n2,it,rka,dka,t1);

  sprintf(sname,"%s_%02d.dat",vname,np);
  OutFun2DP(sname,np,mp,nc1,nc2,xx1,xx2,yy1);

  MPI_Finalize();
  return 0;
}
