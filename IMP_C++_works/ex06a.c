#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
  int i, ii, np, mp, nl, nc=10000;
  char pname[MPI_MAX_PROCESSOR_NAME];
  double t0, t1, a;

  //проверка ининциализации буфера, возврат 1 - хорошо, 0 - ошибка
  //пока инициализации не производилось
  MPI_Initialized(&ii); 
  fprintf(stderr,"Before MPI_Init ii=%d\n",ii);

  //создаем буфера
  MPI_Init(&argc,&argv);

  //инициализация пройдена 
  MPI_Initialized(&ii);
  fprintf(stderr,"Between MPI_Init & MPI_Finalize ii=%d\n",ii);

  MPI_Comm_size(MPI_COMM_WORLD,&np); //обозначаем число MPI прцоессов
  MPI_Comm_rank(MPI_COMM_WORLD,&mp); //обозначам логические номера MPI
  MPI_Get_processor_name(pname,&nl); //возврат доменного имени комп или IP адрес

  t0 = MPI_Wtick(); //измерение времени

  fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",
          np,mp,pname,t0);

  t1 = MPI_Wtime(); //запуск времени 

  a = 0;
  for (i=0; i<nc; i++)
   {a = a + 1.23*i; if (a>1000) a = a / 1000;}

  t1 = MPI_Wtime()-t1; //контроль времени на данном этапе

  fprintf(stderr,"mp=%d, time=%12le res=%12le\n",mp,t1,a);

  MPI_Finalize(); //обратная init функция для процесса 

  MPI_Initialized(&ii); //проверка того что процесса отключен
  fprintf(stderr,"After MPI_Finalize ii=%d\n",ii);t

  return 0;
}
