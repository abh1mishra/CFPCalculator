void get_rounded_frac(double frac,int arr[]){

// int frac_dec=1;
double frac_copy=frac;
// int input_dec_count=0;
// while (((int)(frac*10)>0)&&(input_dec_count<8)){
// frac=frac*10.0;
// printf("\n num10 is %f \n",frac );
// frac_dec*=10;
// printf("\n kam of %f is %d \n",frac,(int)frac );
// frac=frac-(int)frac;
// printf("\n num is %f \n",frac );
// input_dec_count++;
// }
// printf("\n input_dec count %d \n",input_dec_count);
// printf("frac_dec is %d",frac_dec);
// int k=get_nearest_power_of_two(frac_dec);
//int k=get_nearest_power_of_two(frac_large);
//printf("\n %d \n",k );
// printf("nearpowerof two is %d \n",k);
// for(int jk=0;jk<input_dec_count;jk++){
//   frac_copy*=10;
// }
// frac_copy=(int)frac_copy;

// for(int jk=0;jk<input_dec_count;jk++){
//   frac_copy/=10;
// }
// printf("\n frac_copy is %f \n",frac_copy);
// int j=0;
// frac_copy*=k;
frac_copy*=(1<<16);
// while(k!=0){
// k=k>>1;
// j++;
// }
// printf("\n j is %d \n",j);
arr[1]=16;
// if((frac_copy - (int) frac_copy)<0.5){
// arr[0]=(int) frac_copy;
// return;
// }

// arr[0]=(int) frac_copy +1;
arr[0]=(int) frac_copy;
}
