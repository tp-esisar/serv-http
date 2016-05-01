#include "reader.h"
#include "StringL.h"
#include <string.h>
#include <stdio.h>

//printf("\x1b[31mThis is red text\x1b[0m\n");
//printf("\x1b[32mThis is green text\x1b[0m\n");



int test(char* name, syntaxe_elem se,read_state state, char* buffIn, char* expect) {
  StringL buffInSL = fromRegularString(buffIn);
  StringL buffInSLSave = buffInSL;
  StringL buffOutSL = fromRegularString(buffIn+strlen(expect));
  StringL expectSL = fromRegularString(expect);
  reader r = get_reader(se,&buffInSL);
  read_return rr = CALL_CLOSURE(r);
  int ok = 0;
  if(rr.state != state) {
    fprintf(stderr, "\x1b[31mtest \"%s\" FAIL STATE RETURN\x1b[0m\n",name);
    return 1;
  }
  if( (!stringLEq(buffInSL, buffOutSL)) && (state == SUCC) ) {
    char* modifBuffIn = toRegularString(buffInSL);
    char* buffOut = toRegularString(buffOutSL);
    fprintf(stderr, "\x1b[31mtest \"%s\" FAIL IN/OUT -> \n\tin: \"%s\" out: \"%s\" instead of \"%s\"\x1b[0m\n",name,buffIn,modifBuffIn,buffOut);
    free(modifBuffIn);
    free(buffOut);
    ok+=1;
  }
  if( (!stringLEq(rr.string, expectSL)) && (state == SUCC)) {
    char* strReturn = toRegularString(rr.string);
    fprintf(stderr, "\x1b[31mtest \"%s\" FAIL EXPECT RETURN -> \n\tgot: \"%s\" instead of \"%s\"\x1b[0m\n",name,strReturn,expect);
    free(strReturn);
    ok+=1;
  }
  if(ok == 0) {
    printf("\x1b[32mtest \"%s\" OK\x1b[0m\n",name);
  }
  if(buffInSLSave.len != 0) free(buffInSLSave.s);
  if(buffOutSL.len != 0) free(buffOutSL.s);
  if(expectSL.len != 0) free(expectSL.s);
  return ok;
}


int main() {
  
  printf("------------debut tests readers------------\n");

  test("HTTP",HTTP_name,SUCC,"HTTP  ","HTTP");

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

  test("origin_form 1",origin_form,SUCC,"/RaSc4asse RRR","/RaSc4asse");
  test("origin_form 2",origin_form,SUCC,"/RaSc4asse/trololo RRR","/RaSc4asse/trololo");

  test("HTTP_version",HTTP_version,SUCC,"HTTP/9.1\r\nRRR","HTTP/9.1");
  test("field_name",field_name,SUCC,"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0","User-Agent");
  test("header_field",header_field,SUCC,"User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0\r\n","User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0");

  test("Connection_header",Connection_header,SUCC,"Connection: keep-alive je suis invisiiiible","Connection: keep-alive ");
  test("Connection",Connection,SUCC,"keep-alive je suis invisiiiible","keep-alive");

  test("Content_Length_header",Content_Length_header,SUCC,"Content-Length: 348 je suis invisiiiible","Content-Length: 348 ");
  test("Trailer_header",Trailer_header,SUCC,"Trailer: Max-Forwards je suis invisiiiible","Trailer: Max-Forwards ");
  test("Transfer_Encoding_header",Transfer_Encoding_header,SUCC,"Transfer-Encoding: chunked je suis invisiiiible","Transfer-Encoding: chunked ");
  test("Upgrade_header",Upgrade_header,SUCC,"Upgrade: HTTP/2.0, SHTTP/1.3, IRC/6.9, RTA/x11 je suis invisiiiible","Upgrade: HTTP/2.0, SHTTP/1.3, IRC/6.9, RTA/x11 ");
  test("Via_header",Via_header,SUCC,"Via: 1.0 fred, 1.1 example.com (Apache/1.1) je suis invisiiiible","Via: 1.0 fred, 1.1 example.com (Apache/1.1) ");
  test("Age_header",Age_header,SUCC,"Age: 12 je suis invisiiiible","Age: 12 ");
  test("Expires_header",Expires_header,SUCC,"Expires: Thu, 01 Dec 1994 16:00:00 GMT je suis invisiiiible","Expires: Thu, 01 Dec 1994 16:00:00 GMT ");
  test("Date_header",Date_header,SUCC,"Date: Tue, 15 Nov 1994 08:12:31 GMT je suis invisiiiible","Date: Tue, 15 Nov 1994 08:12:31 GMT ");
  test("Location_header",Location_header,SUCC,"Location: http://www.w3.org/pub/WWW/People.html je suis invisiiiible","Location: http://www.w3.org/pub/WWW/People.html ");
  test("Retry_After_header",Retry_After_header,SUCC," je suis invisiiiible","Retry-After: 120 ");
  test("Vary_header",Vary_header,SUCC,"Vary: Accept-Language je suis invisiiiible","Vary: Accept-Language ");
  test("Warning_header",Warning_header,SUCC,"Warning: 199 Miscellaneous warning je suis invisiiiible","Warning: 199 Miscellaneous warning ");
  test("Expect_header",Expect_header,SUCC,"Expect: 100-continue je suis invisiiiible","Expect: 100-continue ");
  test("Host_header",Host_header,SUCC,"Host: en.wikipedia.org:80 je suis invisiiiible","Host: en.wikipedia.org:80 ");
  test("Max_Forwards_header",Max_Forwards_header,SUCC,"Max-Forwards: 10 je suis invisiiiible","Max-Forwards: 10 ");
  test("Pragma_header",Pragma_header,SUCC,"Pragma: no-cache je suis invisiiiible","Pragma: no-cache ");
  test("Range_header",Range_header,SUCC,"Range: bytes=500-999 je suis invisiiiible","Range: bytes=500-999 ");
  test("TE_header",TE_header,SUCC,"TE: trailers, deflate je suis invisiiiible","TE: trailers, deflate ");
  test("If_Match_header",If_Match_header,SUCC,"If-Match: \"737060cd8c284d8af7ad3082f209582d\" je suis invisiiiible","If-Match: \"737060cd8c284d8af7ad3082f209582d\" ");
  test("If_None_Match_header",If_None_Match_header,SUCC,"If-None-Match: \"737060cd8c284d8af7ad3082f209582d\" je suis invisiiiible","If-None-Match: \"737060cd8c284d8af7ad3082f209582d\" ");
  test("If_Modified_Since_header",If_Modified_Since_header,SUCC,"If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT je suis invisiiiible","If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT ");
  test("If_Unmodified_Since_header",If_Unmodified_Since_header,SUCC,"If-Unmodified-Since: Sat, 29 Oct 1994 19:43:31 GMT je suis invisiiiible","If-Unmodified-Since: Sat, 29 Oct 1994 19:43:31 GMT ");
  test("If_Range_header",If_Range_header,SUCC,"If-Range: \"737060cd8c284d8af7ad3082f209582d\" je suis invisiiiible","If-Range: \"737060cd8c284d8af7ad3082f209582d\" ");
  test("Accept_header",Accept_header,SUCC,"Accept: text/plain je suis invisiiiible","Accept: text/plain ");
  test("Accept_Charset_header",Accept_Charset_header,SUCC,"Accept-Charset: utf-8 je suis invisiiiible","Accept-Charset: utf-8 ");
  test("Accept_Encoding_header",Accept_Encoding_header,SUCC,"Accept-Encoding: gzip, deflate je suis invisiiiible","Accept-Encoding: gzip, deflate ");
  test("Accept_Language_header",Accept_Language_header,SUCC,"Accept-Language: en-US je suis invisiiiible","Accept-Language: en-US ");
  test("Authorization_header",Authorization_header,SUCC,"Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ== je suis invisiiiible","Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ== ");
  test("Referer_header",Referer_header,SUCC,"Referer: http://en.wikipedia.org/wiki/Main_Page je suis invisiiiible","Referer: http://en.wikipedia.org/wiki/Main_Page ");
  test("cookie_header",cookie_header,SUCC,"Cookie: $Version=1; Skin=new; je suis invisiiiible","Cookie: $Version=1; Skin=new; ");

  printf("------------fin tests readers------------\n");

  return 0;

}