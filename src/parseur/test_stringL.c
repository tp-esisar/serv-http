#include "stringL.h"
#include <string.h>
#include <stdio.h>

int main() {
  
  int ok = 0;
  
  StringL stringLEqTest1 = {"12345",5};
  StringL stringLEqTest2 = {"12345",5};
  StringL stringLEqTest3 = {"123456",6};
  StringL stringLEqTest4 = {"12534",5};
  if (stringLEq(stringLEqTest1,stringLEqTest2) != 1) ok++;
  if (stringLEq(stringLEqTest1,stringLEqTest3) != 0) ok++;
  if (stringLEq(stringLEqTest1,stringLEqTest4) != 0) ok++;
  if (ok == 0) printf("tests fonction stringLEq ok\n");
  else printf("tests fonction stringLEq erreur\n");
  
  char* regularString = "12345";
  StringL customStringHand = {"12345",5};
  StringL customGenerated = fromRegularString(regularString);
  if (stringLEq(customGenerated,customStringHand) != 1) ok++;
  if (ok == 0) printf("tests fonction fromRegularString ok\n");
  else printf("tests fonction fromRegularString erreur\n");
  
  StringL customString = {"12345",5};
  char* regularStringHand = "12345";
  char* regularGenerated = toRegularString(customString);
  if (strcmp(regularGenerated,regularStringHand) != 0) ok++;
  if (ok == 0) printf("tests fonction toRegularString ok\n");
  else printf("tests fonction toRegularString erreur\n");
  
  return ok;
}