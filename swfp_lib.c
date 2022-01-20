#include <stdio.h>
#include <stdlib.h>
#include "fp.h"


/* input: Double value to be represented
 * output: Conversation from Double into our swfp_t
 *
 * Follow the Project Documentation for Instructions
 */



/* typedef exponent is equal to 4 bits  
 * mantissa is equal 10 bits
 * sign is 0
 * precision is 16 
 * and also takes account for bias
 */
typedef int swfp_t;
int e=4;
int m=10;
int sign=0;
int bias;
int precision=16;
swfp_t s_inf=(1<<13)|(1<<12)|(1<<11)|(1<<10);
swfp_t s_ninf=(1<<13)|(1<<12)|(1<<11)|(1<<10)|(1<<14);
int s_nan=(1<<13)|(1<<12)|(1<<11)|(1<<10)|(1<<1)|(1<<4);


/* gets rounded frac from a double int then it 
 * gets int nearest power from there it gets the 
 * the int part of a double. It then gets the first  
 * int position and then gets the exponent 
 */
double get_frac_part(double num); 
void get_rounded_frac(double,int []); 
int get_nearest_power_of_two(int); 
int get_int_part(double); 
int get_first_pos_of_one(int); 
int get_E(int,int,int);  



int get_nearest_power_of_two(int k){
/*we get by dividing which is just a lot of shifting
 * and the shifting is based on the frac, sign, mantissa, 
 * exponent, and the precision.  
 */
k--;
k |= k >> 1;
k |= k >> 2;
k |= k >> 4;
k |= k >> 8;
k |= k >> 16;
k++;
return k;
}


// Gets the fractional part like in 5.231 its 0.231
double get_frac_part(double num){
  // Subtract integer part from number to get frac part
return num-(int) num;
}


// It converts fractional part into z/(2^n) format where n=precision
void get_rounded_frac(double frac,int arr[]){
  // Multiply with 2^16 and round it off. so you get z/2^2
frac*=(1<<precision);
arr[1]=precision;
// arr[0]=((frac-(int)frac)>0.5)?(((int)frac)+1):((int) frac);
arr[0]=(int) frac;
}


// Gets the integer part like in 5.231 its 5
int get_int_part(double num){
  // Integer part of number

return (int) num;
}



// For eg. 111001 gives 5
int get_first_pos_of_one(int k){
  // Get by dividing until 1
int i=0;
while(k!=1){
k=k >> 1;
i++;
}
return i;
}



// This part gives the Exponent
int get_E(int int_k, int frac_num,int frac_len ){
if(int_k>0){
return get_first_pos_of_one(int_k); // returns get_first_pos_of_one
}


// Handling zero
if(frac_num==0){
  return (1<<14);
}

// For decimal we tart from right , so we subtract
return get_first_pos_of_one(frac_num)-frac_len;
}



swfp_t double_to_swfp(double val) {
 
  /* Implement this function */
bias=(1 << (e-1))-1;
double abs_val=val;
int sign=0;
int frac_len=0;
int norm_value;
//get the sign
if(val<0){
sign=1;
abs_val=-1*val;
}



// We get integer and fractional part
int int_part=get_int_part(abs_val);
double frac_part=get_frac_part(abs_val);
// arr[0] is rounded frac i.e z in z/2^n and arr[1] is precision or n
 int arr[2];
 get_rounded_frac(frac_part,arr);
 // Started using array so afther when I found its redundant i didnt change.
 int rounded_frac=arr[0];
 frac_len=arr[1];
 int E=get_E(int_part,rounded_frac,frac_len);
 // Handle zero
 if((E>>14)%2==1){
   return 0+(sign<<(m+e));
 }



 // This handles any signs of overflows
 if(E>bias){
 return (frac_part>0)?(s_nan):((sign==1)?(s_ninf):(s_inf));
 }



 // This handles any signs of underflows
 int exp=((E+bias)>=0)?(E+bias):0;
// printf("de %d %d %d\n",exp,E,bias);
 // Sets the bits for exponent and sign
//  norm_value=(E>(1-bias))?((sign<<(m+e))+(exp<<m)):(sign<<(m+e));
 norm_value=(sign<<(m+e))+(exp<<m);

 // Finding frac
 long mixed= (int_part<<frac_len)+rounded_frac;
// printf("rf %d \n",rounded_frac);
 // If the result is of size less than m then pad with zeros to right and if more than shift to right to get size m
 if((frac_len+E)>=m){
   mixed=(mixed>>(frac_len+E-m));
  mixed-=(1<<m);
   norm_value=norm_value+(int)mixed;
   return (swfp_t)norm_value; // returns (swfp_t) normal value 
 }
  if(exp==0){
 norm_value=norm_value+(int)mixed;
 return (swfp_t)norm_value; 
 }
 mixed=(mixed<<(m-(frac_len+E)))-(1<<m); 
//  printf("mx %ld %d %d \n",mixed,frac_len,E);
 // Mixed values of fracs_length
 norm_value=norm_value+(int)mixed;
 return (swfp_t)norm_value; // returns (swfp_t)norn_value 



/* return normalized value*/
}

/* input: swfp_t representation of our floats
 * output: Conversion back to a Double from our swfp_t
 *
 * Follow the Project Documentation for Instructions
 */

// double bin_to_dec(int k){
// while(k>1){
// k*=0.1;
// }
//
// }


double swfp_to_double(swfp_t val) {
  /* Implement this function */
  bias=(1 << (e-1))-1;
  bias=(1 << (e-1))-1;
  // This part handles special values
  if(val==s_nan){
return DOUBLE_NAN;
  }
  if(val==s_inf){
return DOUBLE_INF;
  }
  if(val == s_ninf){
return -DOUBLE_INF;
  }



  // Gets sign exp and mantissa by bit shifting and subtracting previous bits
  swfp_t sign=val>>(m+e);
  swfp_t exp=(val-(sign<<(m+e)))>>m;
  swfp_t frac=val-(sign<<(m+e))-(exp<<m);


  int E=(exp!=0)?(exp-bias):(1-bias);
  // This check for underflows, so if not underflowed den pad with 1 in right of frac
  int mantissa=(exp!=0)?((1<<(m))+frac):(frac);


    double value=(double)mantissa;
    value=(value/(1<<(m-E)));

  return (sign==1)?(-value):(value); // returns sign == 1 and it's value
}



/* input: Two swfp_t representations of our floats
 * output: The multiplication result in our swfp_t representation
 *
 * You must implement this using the algorithm described in class:
 *   Xor the signs: S = S1 ^ S2
 *   Add the exponents: E = E1 + E2
 *   Multiply the Frac Values: M = M1 * M2
 *
 *   You MAY use C's float type to do your M calculation, however, you cannot
 *     convert val1 and val2 to floats, multiply them, and then convert
 *     them back to swfp_t types.  You need to work with these in terms of
 *     M, S, and E components.
 *
 * Follow the Project Documentation for Instructions
 */



swfp_t swfp_mul(swfp_t val1, swfp_t val2) {
  /* Implement this function */
bias=(1 << (e-1))-1;
  //get sign exp and mantissa by bit shifting and subtracting previous bits
swfp_t sign1=val1>>(m+e);
swfp_t exp1=(val1-(sign1<<(m+e)))>>m;
swfp_t frac1=val1-(sign1<<(m+e))-(exp1<<m);
swfp_t sign2=val2>>(m+e);
swfp_t exp2=(val2-(sign2<<(m+e)))>>m;
swfp_t frac2=val2-(sign2<<(m+e))-(exp2<<m);

//m1, m2 are Mantissas
//E1, E2 are exponents
int m1=(exp1!=0)?((1<<m)+frac1):(frac1);
int m2=(exp2!=0)?((1<<m)+frac2):(frac2);
int E1=(exp1!=0)?(exp1-bias):(1-bias);
int E2=(exp2!=0)?(exp2-bias):(1-bias);
int sadd=0;
double madd=0;
int Eadd;
sadd=sign1^sign2;
char special_1=0;
char special_2=0;

/* 28 means NaN;
29 means INF;
30 means -INF;
*/
special_1=0;
special_2=0;


// We see for the 29 ,30 and 31 bit to see for nan, inf or -inf and accordingly set special values
if(val1 == s_nan){
special_1=1;
sign1=0;
}
if(val1 == s_inf){
special_1=2;
sign1=0;
}
if(val1 == s_ninf){
special_1=3;
sign1=1;
}
if(val2 == s_nan){
special_2=1;
sign2=0;
}
if(val2 == s_inf){
special_2=2;
sign2=0;
}
if(val2 == s_ninf){
special_2=3;
sign2=1;
}



/*
1 means NaN
2 means inf
3 means -inf
*/
//1
//handle special rules
//handle for inf and inf and return inf
int ko1=(sign1==1)?(val1-(1<<(m+e))):val1;
int ko2=(sign2==1)?(val2-(1<<(m+e))):val2;
if((special_1==2)&&(special_2==2)){

return (swfp_t)(s_inf);
}
//handle for -inf and zero and return NaN

if(((special_1==3)&&(val2==0))||((val1==0)&&(special_1==3))){

return (swfp_t)(s_nan);
}

//handle for inf and zero and return NaN
if(((special_1==2)&&(val2==0))||((val1==0)&&(special_2==2))){

return (swfp_t)(s_nan);}
//handle 0 and inf to give nan
if(((special_1==2)&&(ko2==0))||((ko1==0)&&(special_2==2))){

return (swfp_t)(s_nan);}

//handle -inf and 0
if(((special_1==3)&&(ko2==0))||((ko1==0)&&(special_2==3))){

return (swfp_t)(s_nan);}
//2
//handle -inf and -inf
if((special_1==3)&&(special_2==3)){

return (swfp_t)(s_inf);
}
//3
// handle inf and -inf and return -inf
if(((special_1==2)&&(special_2==3))||((special_2==2)&&(special_1==3))){
return (swfp_t)(s_ninf);
}
//4
//handle inf and nan
if(((special_1==2)&&(special_2==1))||((special_2==2)&&(special_1==1))){
return (swfp_t)(s_nan);
}
//5
if(((special_1==3)&&(special_2==1))||((special_2==3)&&(special_1==1))){
return (swfp_t)(s_nan);
}
//6

//-0
// -1*inf =-inf
if(((special_1==2)&&(special_2==0))||((special_2==2)&&(special_1==0))){
  swfp_t ko=(sign1==sign2)?(s_inf):(s_ninf);
return ko;
}
//7
//handle -inf and anything other than specials
if(((special_1==3)&&(special_2==0))||((special_2==3)&&(special_1==0))){
  swfp_t ko=(sign1==sign2)?(s_inf):(s_ninf);
return ko;
}
//8
//handle inf and anything other than specials
if(((special_1==1)&&(special_2==0))||((special_2==1)&&(special_1==0))){
return (swfp_t)(s_nan);
}
//9
//handle nan and 0
if(((special_1==1)&&(val2==0))||((val1==0)&&(special_1==1))){
return (swfp_t)(s_nan);
}
//10

//11

//handle -0
if(((special_1==3)&&(val2==0))||((val1==0)&&(special_1==3))){
return (swfp_t)(s_nan);
}
//get effective E and mantissa by integer mult.
Eadd=E1+E2;
madd=(double)(m1*m2);
//divide madd by 2^m

madd/=(1<<(2*m));
// convert to double by multiplying with 2^Eadd, multiply if Eadd is pos or divide else

double value=(sadd==0)?((Eadd>=0)?(madd*(1<<Eadd)):(madd/(1<<(-Eadd)))):((Eadd>=0)?(-madd*(1<<Eadd)):(-madd/(1<<(-Eadd))));
if((value==0)&&((sign1==1)^(sign2==1))){
  return (1<<(m+e));
  //handle zero
  
}
return double_to_swfp(value);
}



/* input: Two swfp_t representations of our floats
 * output: The addition result in our swfp_t representation
 *
 * You must implement this using the algorithm described in class:
 *   If needed, adjust the numbers to get the same exponent E
 *   Add the two adjusted Mantissas: M = M1 + M2
 *
 *   You MAY use C's float type to do your M calculation, however, you cannot
 *     convert val1 and val2 to floats, multiply them, and then convert
 *     them back to swfp_t types.  You need to work with these in terms of
 *     M, S, and E components.
 *
 * Follow the Project Documentation for Instructions
 */
swfp_t swfp_add(swfp_t val1, swfp_t val2) {
  /* Implement this function */
  /*
handle special values;
  */


  bias=(1 << (e-1))-1;
    //get sign exp and mantissa by bit shifting and subtracting previous bits
  swfp_t sign1=val1>>(m+e);
  swfp_t exp1=(val1-(sign1<<(m+e)))>>m;
  swfp_t frac1=val1-(sign1<<(m+e))-(exp1<<m);
  swfp_t sign2=val2>>(m+e);
  swfp_t exp2=(val2-(sign2<<(m+e)))>>m;
  swfp_t frac2=val2-(sign2<<(m+e))-(exp2<<m);

  //m1, m2 are Mantissas
  //E1, E2 are exponents
  int m1=(exp1!=0)?((1<<m)+frac1):(frac1);
  int m2=(exp2!=0)?((1<<m)+frac2):(frac2);
  int E1=(exp1!=0)?(exp1-bias):(1-bias);
  int E2=(exp2!=0)?(exp2-bias):(1-bias);
  int sadd=0;
  int madd=0;
  int Eadd;
  char special_1=0;
  char special_2=0;


  /* 28 means NaN;
29 means INF;
30 means -INF;
  */
special_1=0;
special_2=0;



//look for 29,30 and 31 bit for special values.
if(val1 == s_nan){
special_1=1;
sign1=0;
}
if(val1 == s_inf){
special_1=2;
sign1=0;
}
if(val1 == s_ninf){
special_1=3;
sign1=1;
}
if(val2 == s_nan){
special_2=1;
sign2=0;
}
if(val2 == s_inf){
special_2=2;
sign2=0;
}
if(val2 == s_ninf){
special_2=3;
sign2=1;
}



  /*
1 means NaN
2 means inf
3 means -inf
  */
  //handle special rules
//1
if((special_1==2)&&(special_2==2)){
  return (swfp_t)(s_inf);
}

//2
//handle inf and -inf
if(((special_1==2)&&(special_2==3))||((special_2==2)&&(special_1==3))){
  return (swfp_t)(s_nan);
}
//3
//so on
if(((special_1==2)&&(special_2==1))||((special_2==2)&&(special_1==1))){
  return (swfp_t)(s_nan);
}
//4
if(((special_1==2)&&(special_2==0))||((special_2==2)&&(special_1==0))){
  return (swfp_t)(s_inf);
}
//5
if(((special_1==1)&&(special_2==0))||((special_1==2)&&(special_1==0))){
  return (swfp_t)(s_nan);
}
if((special_1==3)&&(special_2==3)){
  return (swfp_t)(s_ninf);
}
//6
if(((special_1==3)&&(special_2==1))||((special_2==3)&&(special_1==1))){
  return (swfp_t)(s_nan);
}
//7
if(((special_1==3)&&(special_2==0))||((special_2==3)&&(special_1==0))){
  return (swfp_t)(s_ninf);
}



  if(sign1==sign2){
  madd=(E1>=E2)?((m1<<(E1-E2))+m2):((m2<<(E2-E1))+m1);
    sadd=sign1;
}



if(sign1 != sign2){
// Shift lower number untill exponent matches
madd=(E1>=E2)?(m2-(m1<<(E1-E2))):(m1-(m2<<(E2-E1)));
// printf("see\n");
if(E1>E2){
  // printf("see1\n");
  sadd=sign1;
}
if(E1<E2){
  // printf("see2\n");
  sadd=sign2;
}
if(E1==E2){
  // printf("see3\n");
  if(m1>m2){
    sadd=sign1;
  }else{
    sadd=sign2;
  }
}
madd=(madd<0)?(-madd):(madd);
}
// printf("signs are %d %d %d\n",sign1,sign2,sadd);


// Get effective E
Eadd=(E1>E2)?(E1):(E2);
// Shifted by m, now shift my sot to match exponents and then divide 
double val=(double)madd;
int sot=(E1>E2)?(E1-E2):(E2-E1);
val=val/(1<<(m+sot));
// Multiply with 2^Eadd
val=(Eadd>=0)?(val*(1<<Eadd)):(val/(1<<(-Eadd)));
// Look for sign
val=(sadd==0)?(val):(-val);

// Handle -0
if((val==0)&&((sign1==1)&&(sign2==1))){
  return (1<<(m+e));
}
return double_to_swfp(val); // returns double_to_swfp

}
