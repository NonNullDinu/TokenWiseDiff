#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;

int digit_value(char c);

char digit_number(int n);

int longDivision(char* number, int divisor, int base, char ans[]) 
{
  int idx = 0;
  int k = 0;
  int temp = digit_value(number[idx]);
  while (temp < divisor && number[idx] != '\0')
    temp = temp * base + digit_value(number[++idx]);
  while (number[idx] != '\0')
    { 
      ans[k++] = digit_number(temp / divisor); 
      temp = (temp % divisor) * base + digit_value(number[++idx]);
    }
   temp /= base;
   
  if (k == 0)
    strcpy(ans, "0");
    else{
      ans[k] = '\0';
    }
  return temp%divisor;
}

int digit_value(char c){
  if(c == '\0')
    return 0;
  if(c <= '9')
    return c - '0';
  else
    return c - 'A' + 10;
}

char digit_number(int n){
  if(n < 10)return '0' + n;
  else return 'A' + n - 10;
}
ofstream fout("base_converter.out");

void show(char* n, int b, int c){
  char div_ans[2000];
  int mod = longDivision(n, c, b, div_ans);
  if(strcmp(div_ans, "0") != 0)
    show(div_ans, b, c);
  fout << digit_number(mod%c);
}

int main(){
  char n[2000];
  int b, c;
  ifstream fin("base_converter.in");
  fin >> n >> b >> c;
  show(n, b, c);
}