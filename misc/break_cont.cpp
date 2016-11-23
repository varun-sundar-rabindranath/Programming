#include <stdio.h>

int main() {
char k[100];
int i=0,j=0;
scanf("%s",k);
for(i=10;i>0;i--)
{
printf("%c \n", k[i]);
/*if(k[i]!='\0')
{
  j++;
}*/

}
/*while(k[i] !='\0')
{
j++;
i++;
}*/
printf("\n  %s",k);
printf("the length is  %d",j);
}
