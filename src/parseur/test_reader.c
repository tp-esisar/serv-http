#include "reader.h"
#include "StringL.h"
#include <string.h>
#include <stdio.h>


int test(char* name, syntaxe_elem se,read_state state, char* buffIn, char* expect) {
  StringL buffInSL = fromRegularString(buffIn);
  StringL buffInSLSave = buffInSL;
  StringL buffOutSL = fromRegularString(buffIn+strlen(expect));
  StringL expectSL = fromRegularString(expect);
  reader r = get_reader(se,&buffInSL);
  read_return rr = CALL_CLOSURE(r);
  int ok = 0;
  if(rr.state != state) {
    fprintf(stderr, "test \"%s\" FAIL STATE RETURN\n",name);
    return 1;
  }
  if( (!stringLEq(buffInSL, buffOutSL)) && (state == SUCC) ) {
    char* modifBuffIn = toRegularString(buffInSL);
    char* buffOut = toRegularString(buffOutSL);
    fprintf(stderr, "test \"%s\" FAIL IN/OUT -> \n\tin: \"%s\" out: \"%s\" instead of \"%s\"\n",name,buffIn,modifBuffIn,buffOut);
    free(modifBuffIn);
    free(buffOut);
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
  
  printf("------------debut tests readers------------\n");
  
  test("SP 1",SP,SUCC," A B   "," ");
  test("SP 2",SP,FAIL,"A B   ","");
  test("OWS 1",OWS,SUCC,"RASCASSE","");
  test("OWS 2",OWS,SUCC," RASCASSE"," ");
  test("OWS 3",OWS,SUCC,"   RASCASSE","   ");
  test("day 1",day,SUCC,"12RASCASSE","12");
  test("day 2",day,FAIL,"1RASCASSE","");
  test("CRLF 1",CRLF,SUCC,"\r\nR","\r\n");
  test("CRLF 2",CRLF,FAIL,"\rRRR","");
  test("token 1",token,SUCC,"jesuisUnsup333r-token* tokenSuivant","jesuisUnsup333r-token*");
  test("token 2",token,FAIL," ","");
  
  test("method 1", method, SUCC, "GET / HTTP/1.1", "GET");
  
  test("dec_octet 1",dec_octet,SUCC,"1RRR","1");
  test("dec_octet 2",dec_octet,SUCC,"23RRR","23");
  test("dec_octet 3",dec_octet,SUCC,"234RRR","234");
  test("IPv4address",IPv4address,SUCC,"192.168.1.1RRR","192.168.1.1");
  test("reg_name",reg_name,SUCC,"www.mangeDesPatates&.com/frites/ketchup RRR","www.mangeDesPatates&.com");
  test("IPvFuture 1",IPvFuture,SUCC,"vA42.miniIP: RRR","vA42.miniIP:");
  test("IPvFuture 2",IPvFuture,FAIL,"vA42 .miniIP: RRR","");
  //test("IPv6address",IPv6address,SUCC,"fe80::200:4:AC43 RRR","fe80::200:4:AC43");
  
  test("host 1",host,SUCC,"192.168.1.1 RRR","192.168.1.1");
  
  test("authority 1",authority,SUCC,"192.168.1.1 RRR","192.168.1.1");
  
  test("path_absolute 1",path_absolute,SUCC,"/ RRR","/");
  test("path_absolute 2",path_absolute,SUCC,"/hello/friend RRR","/hello/friend");
  
  test("request_target 1",request_target,SUCC,"/ HTTP/1.1","/");
  test("request_target 2",request_target,SUCC,"/niNjA/chuck/maurice/ HTTP/9.1","/niNjA/chuck/maurice/");
  
  
  
  
  printf("------------fin tests readers------------\n");
  
  return 0;
  
}