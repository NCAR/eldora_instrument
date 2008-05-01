main()
{
int num_of_mads[3],i,j;

num_of_mads[0] = 0;
num_of_mads[1] = 2;
num_of_mads[2] = 1;

for(j=0; j<3; j++)
  {
    for(i=0; i<num_of_mads[j]; i++)
      printf("in loop with j= %d\n",j);
  }
}
