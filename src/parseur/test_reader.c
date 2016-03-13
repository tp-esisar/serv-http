#include "reader.h"
#include "stringL.h"
#include <string.h>
#include <stdio.h>


int test(char* name, syntaxe_elem se,read_state state, char* buffIn, char* expect, char* buffOut) {
  StringL buffInSL = fromRegularString(buffIn);
  StringL buffOutSL = fromRegularString(buffOut);
  StringL expectSL = fromRegularString(expect);
  reader r = read(se,&buffInSL);
  read_return rr = CALL_CLOSURE(r);
  int ok = 0;
  if(rr.state != state) {
    fprintf(stderr, "test \"%s\" FAIL STATE RETURN\n",name);
    return 1;
  }
  if( (!stringLEq(buffInSL, buffOutSL)) && (state == SUCC) ) {
    char* modifBuffIn = toRegularString(buffInSL);
    fprintf(stderr, "test \"%s\" FAIL IN/OUT -> \n\tin: \"%s\" out: \"%s\" instead of \"%s\"\n",name,buffIn,modifBuffIn,buffOut);
    free(modifBuffIn);
    ok+=1;
  }
  if( (!stringLEq(rr.string, expectSL)) && (state == SUCC)) {
    char* strReturn = toRegularString(rr.string);
    fprintf(stderr, "test \"%s\" FAIL EXPECT RETURN -> \n\tgot: \"%s\" instead of \"%s\"\n",name,strReturn,expect);
    free(strReturn);
    ok+=1;
  }
  if(ok == 0) {
    printf("test \"%s\" OK",name);
  }
  free(buffInSL.s);
  free(buffOutSL.s);
  free(expectSL.s);
  return ok;
}



int main() {

  reader tempr;
  read_return temprr;
  
  //Test SP
  StringL test_SP = fromRegularString(" A B   ");
  tempr = read(SP, &test_SP);
  temprr = CALL_CLOSURE(tempr);
  if(temprr.state != SUCC) {
    perror("test fail reader SP state\n");
    exit(2);
  }
  if( !stringLEq(temprr.string, fromRegularString(" ")) ) {
    fprintf(stderr, "test fail reader SP string: %s\n", toRegularString(temprr.string));
    exit(2);
  }
  if( !stringLEq(test_SP, fromRegularString("A B   "))) {
    fprintf(stderr, "test fail reader SP test_SP: %s\n", toRegularString(test_SP));
    exit(2);
  }
  tempr = read(SP, &test_SP);
  temprr = CALL_CLOSURE(tempr);
  if(temprr.state != FAIL) {
    perror("test fail reader SP state\n");
    exit(2);
  }
  //test de OWS permet de tester kleene et or
  
  StringL test_OWS = fromRegularString("A    BC");
  tempr = read(OWS, &test_OWS);
  temprr = CALL_CLOSURE(tempr);
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
  
  
  StringL test_OWS2 = fromRegularString("     BC");
  tempr = read(OWS, &test_OWS2);
  temprr = CALL_CLOSURE(tempr);
  if(temprr.state != SUCC) {
    perror("test fail reader OWS state\n");
    exit(2);
  }
  if( !stringLEq(temprr.string, fromRegularString("     ")) ) {
    fprintf(stderr, "test fail reader OWS string: %s\n", toRegularString(temprr.string));
    exit(2);
  }
  if( !stringLEq(test_OWS2, fromRegularString("BC"))) {
    fprintf(stderr, "test fail reader OWS test_SP: %s\n", toRegularString(test_OWS2));
    exit(2);
  }
  
  
  
  //permet de test charIn et concat
  StringL test_day = fromRegularString("906F45678");
  tempr = read(day, &test_day);
  temprr = CALL_CLOSURE(tempr);
  if(temprr.state != SUCC) {
    perror("test fail reader day state S\n");
    exit(2);
  }
  if( !stringLEq(temprr.string, fromRegularString("90")) ) {
    fprintf(stderr, "test fail reader day string: %s\n", toRegularString(temprr.string));
    exit(2);
  }
  if( !stringLEq(test_day, fromRegularString("6F45678"))) {
    fprintf(stderr, "test fail reader OWS test_SP: %s\n", toRegularString(test_day));
    exit(2);
  }
  temprr = CALL_CLOSURE(tempr);
  if(temprr.state != FAIL) {
    perror("test fail reader day state F\n");
    exit(2);
  }
  if( !stringLEq(test_day, fromRegularString("6F45678"))) {
    fprintf(stderr, "test fail reader OWS test_SP: %s\n", toRegularString(test_day));
    exit(2);
  }
  //Test CRLF
  StringL test_CRLF = fromRegularString("\r\nsqfq");
  tempr = read(CRLF, &test_CRLF);
  temprr = CALL_CLOSURE(tempr);
  if(temprr.state != SUCC) {
    perror("test fail reader CRLF state\n");
    exit(2);
  }
  if( !stringLEq(temprr.string, fromRegularString("\r\n")) ) {
    fprintf(stderr, "test fail reader CLRF string: %s\n", toRegularString(temprr.string));
    exit(2);
  }
  if( !stringLEq(test_CRLF, fromRegularString("sqfq"))) {
    fprintf(stderr, "test fail reader CRLF test_CRLF: %s\n", toRegularString(test_CRLF));
    exit(2);
  }
  
  test("SP 1",SP,SUCC," A B   "," ","A B   ");
  test("SP 2",SP,FAIL,"A B   ","","");
  test("OWS 1",OWS,SUCC,"RASCASSE","","RASCASSE");
  test("OWS 2",OWS,SUCC," RASCASSE"," ","RASCASSE");
  test("OWS 2",OWS,SUCC,"   RASCASSE","   ","RASCASSE");
  
  
  
  
  printf("tests passed OK\n");
  
  return 0;
  
}