#include "reader.h"
#include "StringL.h"
#include <string.h>
#include <stdio.h>


int test(char* name, syntaxe_elem se,read_state state, char* buffIn, char* expect, char* buffOut) {
  StringL buffInSL = fromRegularString(buffIn);
  StringL buffInSLSave = buffInSL;
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
    printf("test \"%s\" OK\n",name);
  }
  if(buffInSLSave.len != 0) free(buffInSLSave.s);
  if(buffOutSL.len != 0) free(buffOutSL.s);
  if(expectSL.len != 0) free(expectSL.s);
  return ok;
}



int main() {

  
  test("SP 1",SP,SUCC," A B   "," ","A B   ");
  test("SP 2",SP,FAIL,"A B   ","","");
  test("OWS 1",OWS,SUCC,"RASCASSE","","RASCASSE");
  test("OWS 2",OWS,SUCC," RASCASSE"," ","RASCASSE");
  test("OWS 3",OWS,SUCC,"   RASCASSE","   ","RASCASSE");
  test("day 1",day,SUCC,"12RASCASSE","12","RASCASSE");
  test("day 2",day,FAIL,"1RASCASSE","","");
  test("CRLF 1",CRLF,SUCC,"\r\nR","\r\n","R");
  test("CRLF 2",CRLF,FAIL,"\rRRR","","");
  test("token 1",token,SUCC,"jesuisUnsup333r-token* tokenSuivant","jesuisUnsup333r-token*"," tokenSuivant");
  test("token 2",token,FAIL," ","","");
  
  
  printf("tests passed OK\n");
  
  return 0;
  
}