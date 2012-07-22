#include <ptlib.h>
#include "ptclib\paec.h"
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
#define ECHO_TEST_NUM 5
#define MIN_ECHO_LEVEL 500
#define SAMPLE_LEN 8
#define ECHO_DELAY 240


int16_t sample[240] = {5000,20000,10000,-15000,-30000,-15000,10000,20000};

PAec::PAec(int _algo)
{
 algo = _algo;

 FILE *fs=NULL;
 fs=fopen("aec_test.wav","rb"); 
 if(fs)
 {
  fseek(fs,0,SEEK_END); 
  t_sz=ftell(fs)-58; 
  fseek(fs,58,SEEK_SET); t_buffer=new char[t_sz];                    
  fread(t_buffer,1,t_sz,fs); fclose(fs);
 }
 else
 {
  t_sz=10000;
  t_buffer=new char[t_sz];
  memset(t_buffer,0,10000);
 }
 t_pos=0;

 aec_disable = 0;
 if(algo==0 || algo>2) aec_disable = 1;
 st = NULL;
 den1 = NULL;
 oslecC = echo_can_create(512,0x3f);
 spos = 0;
 rpos = 960;
 mode = ECHO_TEST_NUM; // in test mode while mode != 0 
 tstamp = 0;
 tcount = 0;
 memset(t_buf,0,60000*4);
 t_len = 0;
 t_num = 0;
 s_sum = 0;
 memset(s_buf,0,20*4);
 s_pos = 0;
 memset(s_trk,0,60000*4*2);
 s_trk_pos = 0;
 

 PTRACE(3, "AEC\tcreated AEC" );
}


PAec::~PAec()
{
 if(algo==1 && st!=NULL)
 {
   speex_echo_state_destroy(st);
   speex_preprocess_state_destroy(den);
   speex_echo_state_destroy(st1);

 }
 
 if(den1!=NULL) speex_preprocess_state_destroy(den1);
 echo_can_free(oslecC);
 delete(t_buffer);
}

void PAec::Receive(BYTE * buffer, unsigned & length)
{
 if(length == 0 || length > 480)
   return;

 if(buffer==NULL) return;

 if(mode == ECHO_TEST_NUM) // sending channel is not starting yet
  return;

 if(den1 == NULL)
 {
  int k=1;
  den1 = speex_preprocess_state_init(length>>1, 8000);
  speex_preprocess_ctl(den1, SPEEX_PREPROCESS_SET_VAD, &k);
 }

 if(mode > 0) // in testing mode
 {
  int16_t *buf=(int16_t *)buffer;
  int dt = (length >> 1)/mode;
  int nt = dt/SAMPLE_LEN;
  for(int i=0;i<mode;i++)
  {
   memcpy(buf,sample,dt); buf+=dt;
   t_buf[t_len]=1; t_len++; 
   for(int t=1;t<nt;t++) { t_buf[t_len]=0; t_len++; }
   t_num++;
  }
 }
 else if(mode > -20) // silence mode
 {
  memset(buffer,0,length);                                             
 }
 else
 {
/*	 
  if(t_pos+length<t_sz)
  {
   memcpy(buffer,&t_buffer[t_pos],length); t_pos+=length;
  }
  else
  {
   memcpy(buffer,&t_buffer[t_pos],t_sz-t_pos);
   memcpy(&buffer[t_sz-t_pos],t_buffer,length-(t_sz-t_pos)); t_pos=length-(t_sz-t_pos);
  }
*/  
 }

 int len=length;
 int16_t *buf=(int16_t *)buffer;
 //for(int i=0;i<len;i++) buf[i] = echo_can_hpf_tx(oslecC, buf[i]);
 //speex_preprocess_run(den1, (spx_int16_t *)buffer);

 if(rpos+length >= BSIZE) 
 {
  memcpy(&rbuf[rpos],buffer,BSIZE-rpos);
  len = rpos+length-BSIZE; rpos = 0;
 }
 memcpy(&rbuf[rpos],&buffer[length-len],len); rpos+=len;
 //if(mode<-20)
 //memset(buffer,0,length);
}

void PAec::Send(BYTE * buffer, unsigned & length)
{
 int16_t tmp_buf[1024];

 int16_t *tbuf = (int16_t *)buffer;
 int16_t *cbuf = (int16_t *)rbuf;

 int16_t *buf = (int16_t *)buffer;
 int len=length>>1;


 if(aec_disable != 0) return;


 if(mode > -20) // in test mode
 {
  mode--;
  int16_t *buf = (int16_t *)buffer;
  int len = length >> 1;
  for(int i=0;i<len;i++)
  {
   s_sum-=s_buf[s_pos]; s_buf[s_pos]=abs(buf[i]); s_sum+=s_buf[s_pos]; 
   s_pos++; if(s_pos == SAMPLE_LEN) s_pos=0;
   if(s_sum > MIN_ECHO_LEVEL)
   {
    if(tcount-s_trk[s_trk_pos][0] > (SAMPLE_LEN<<1)) // start new peak
     { s_trk_pos++; s_trk[s_trk_pos][0]=tcount; s_trk[s_trk_pos][1]=s_sum; }
    else if(s_sum > s_trk[s_trk_pos][1])
     { s_trk[s_trk_pos][0]=tcount; s_trk[s_trk_pos][1]=s_sum; }
   }
   if(tstamp == 0 && cbuf[spos] == -30000) tstamp = tcount; // first testing sample
   tcount++; spos++; if((spos << 1) >= BSIZE) spos = 0;
  }
  memset(buffer,0,length);
  return;
 }
 
 if(mode == -20) // calibrating
 {
  mode--;
  int n = s_trk_pos-t_num;
  int best_sum = 0, best_time = 0;
  if(n < 0) // no echo, disabling aec
   { aec_disable = 1; return ; }
  for(int s=1;s<n;s++)
  {
   int sum = s_trk[s][1];
   int btime = s_trk[s][0];
   int pos = s+1;
   for(int i=1;i<t_len;i++)
   {
    if(t_buf[i]!=0)
    {
     int dt=s_trk[pos][0]-btime;
     int tt=i*SAMPLE_LEN;

     while(tt>dt+(SAMPLE_LEN>>1) && pos<=s_trk_pos)
      { pos++; dt=s_trk[pos][0]-btime; }
     if(pos > s_trk_pos) break;

     if(abs(tt-dt)<=(SAMPLE_LEN>>1)) { sum+=s_trk[pos][1]; pos++; }
    }
   }
   if(sum>best_sum) {best_sum=sum; best_time=btime; };
  }
  if(best_time == 0)
   { aec_disable = 1; return ; }
  
  spos-= (best_time - tstamp - ECHO_DELAY);
  while(spos < 0) spos+=(BSIZE>>1);
 }
 
 int sum=0; int svol;
 for(int i=0;i<len;i++) sum+=buf[i];
 svol=sum/len; sum=0;
 for(int i=0;i<len;i++) sum+=abs(buf[i]-svol);
 svol=sum/len;
 PTRACE(6, "PAEC Send frame volume: " << svol);
/*
 sum=0; int cvol; 
 int pos=spos+ECHO_DELAY; if(pos*2 >= BSIZE) pos -= (BSIZE>>1);
 for(int i=0;i<len;i++) { sum+=cbuf[pos]; pos++; if(pos*2 >= BSIZE) pos = 0; }
 cvol=sum/len; sum=0;
 pos=spos+ECHO_DELAY; if(pos*2 >= BSIZE) pos -= (BSIZE>>1);
 for(int i=0;i<len;i++) { sum+=abs(cbuf[pos]-cvol); pos++; if(pos*2 >= BSIZE) pos = 0; }
 cvol=sum/len;
 PTRACE(6, "PAEC Check frame volume: " << cvol);
*/

 if(algo==2) //Oslec
 {
  for(int i=0;i<len;i++)
  {
   tbuf[i] = echo_can_update(oslecC, cbuf[spos], tbuf[i]);
   spos++; if((spos << 1) >= BSIZE) spos = 0;
  }
 }
 else
 {
//  int tp=spos-240; if(tp<0) tp+=(BSIZE>>1);
  if(BSIZE-(spos<<1) > length) { memcpy(rframe,&cbuf[spos],length); spos+=len; }
  else
  {
   int fraglen=BSIZE-(spos<<1);
   memcpy(rframe,&cbuf[spos],fraglen);
   memcpy(&rframe[fraglen],cbuf,length-fraglen);
   spos=(length-fraglen)>>1;
  }
  if(algo==1) //Speex
  {
   if(st == NULL)
   {
    int sampleRate=8000;
    int k=1;
    st = speex_echo_state_init(length>>1, 3072);
    den = speex_preprocess_state_init(length>>1, sampleRate);
    speex_echo_ctl(st, SPEEX_ECHO_SET_SAMPLING_RATE, &sampleRate);
	k=-90;
	speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS, &k);
	k=-60;
	speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS_ACTIVE, &k);
    speex_preprocess_ctl(den, SPEEX_PREPROCESS_SET_ECHO_STATE, st);
   }
   memcpy(tmp_buf,buffer,length);
   speex_echo_cancellation(st, tmp_buf, (const spx_int16_t *)rframe, (spx_int16_t *)buffer);
   speex_preprocess_run(den, (spx_int16_t *)buffer);
  }
 }

 sum=0; int pvol,evol;
 for(int i=0;i<len;i++) sum+=buf[i];
 pvol=sum/len; sum=0;
 for(int i=0;i<len;i++) sum+=abs(buf[i]-pvol);
 pvol=sum/len; evol=svol-pvol;
 PTRACE(6, "PAEC Pure send frame volume: " << pvol);
// evol - громкость эха, pvol - громкость чистого звука, cvol - громкость декодированного звука.

// float ratio=1.0*pvol/(evol+1.0);
// ratio*=ratio;
/*
 if(pvol<300 && evol>200)
   for(int i=0;i<len;i++) tbuf[i] = 0;
*/
}
