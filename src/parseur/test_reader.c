#include "reader.h"
#include "stringL.h"
#include <string.h>
#include <stdio.h>

//dépends de StringL

int main() {

  reader tempr;
  read_return temprr;
  
  // //Test SP
  // StringL test_SP = fromRegularString(" A B   ");
  // tempr = read(SP, &test_SP);
  // temprr = tempr();
  // if(temprr.state != SUCC) {
  //   perror("test fail reader SP state\n");
  //   exit(2);
  // }
  // if( !stringLEq(temprr.string, fromRegularString(" ")) ) {
  //   fprintf(stderr, "test fail reader SP string: %s\n", toRegularString(temprr.string));
  //   exit(2);
  // }
  // if( !stringLEq(test_SP, fromRegularString("A B   "))) {
  //   fprintf(stderr, "test fail reader SP test_SP: %s\n", toRegularString(test_SP));
  //   exit(2);
  // }
  
  // tempr = read(SP, &test_SP);
  // temprr = tempr();
  // if(temprr.state != FAIL) {
  //   perror("test fail reader SP state\n");
  //   exit(2);
  // }
  
  //test de OWS permet de tester kleene et or
  
  StringL test_OWS = fromRegularString("A    BC");
  tempr = read(OWS, &test_OWS);
  
  temprr = tempr();
  
  if(temprr.state != SUCC) {
    perror("test fail reader OWS state\n");
    exit(2);
  }
  if( !stringLEq(temprr.string, fromRegularString("")) ) {
    fprintf(stderr, "test fail reader OWS string: %s\n", toRegularString(temprr.string));
    exit(2);
  }
  if( !stringLEq(test_OWS, fromRegularString("A    BC"))) {
    fprintf(stderr, "test fail reader OWS test_SP: %s\n", toRegularString(test_OWS));
    exit(2);
  }
  
  // StringL test_OWS2 = fromRegularString("    BC");
  // tempr = read(OWS, &test_OWS2);
  // temprr = tempr();
  // if(temprr.state != SUCC) {
  //   perror("test fail reader OWS state\n");
  //   exit(2);
  // }
  // if( !stringLEq(temprr.string, fromRegularString("    ")) ) {
  //   fprintf(stderr, "test fail reader OWS string: %s\n", toRegularString(temprr.string));
  //   exit(2);
  // }
  // if( !stringLEq(test_OWS2, fromRegularString("BC"))) {
  //   fprintf(stderr, "test fail reader OWS test_SP: %s\n", toRegularString(test_OWS2));
  //   exit(2);
  // }
  
  
  
  printf("tests passed OK\n");
  
}