/* This defines the navigation structure to be able to monitor
   the nav variables easily.  Section 6 should be linked at the
   beginning of the navigation structure                          */

#define DATASEG 6

double rlng,alt,vn,ve,vd,rlgpa,rltpa;
double a11,a12,a13,a21,a22,a23,a31,a32,a33;
double a11rt,a12rt,a13rt,a21rt,a22rt,a23rt,a31rt,a32rt,a33rt;
double t11,t12,t13,t21,t22,t23,t31,t32,t33;
double c11,c12,c13,c21,c22,c23,c31,c32,c33;
double lgin,lngin,altin;
double lg,lng;
double snlt,cslt,sclt,tnlt,snlt2,cslt2;
double omeclt,omeslt,gnp,gdp;

double dvnn,dven,dvdn;
double dthn,dthe,dthd,dphx,dphy,dphz,dvxi,dvyi,dvzi;
double dphxl,dphyl,dphzl;
double dphx1,dphy1,dphz1;
double dthx,dthy,dthz;
double dthx1,dthy1,dthz1,dthx2,dthy2,dthz2;
float adatx,adaty,adatz;
float rxx,rxy,rxz,ryx,ryy,ryz,rzx,rzy,rzz;
float rtxak,rtyak,rtzak;
float rtxgkp,rtxrgkp,rtygkp,rtzgkp,rtxgkn,rtxrgkn,rtygkn,rtzgkn;
float alfx,alfy,alfz;
float xab,yab,zab;
float xgb,ygb,zgb,xgbin,ygbin,zgbin;
float prtxgkp,prtxrgkp,prtygkp,prtzgkp;
float prtxgkn,prtxrgkn,prtygkn,prtzgkn,prtxak,prtyak,prtzak;
float tdxgb,tdygb,tdzgb;
float salf,calf,dtv,dtv2;
float tmpstab,tmpxy,tmpxrz;
float tmpxa,tmpya,tmpza;
float tmpgaf,tmpaaf;
float rmrd;

unsigned long nsec,asec,rtime;
long xcwtot,xrcwtot,xccwtot,xrccwtot,xsstot,xrsstot,axcaltot;
long ixgtot,ixrgtot,iygtot,izgtot,ixatot,iyatot,izatot;
long talo,tasep;

short int irate,onesec,foursec,eightsec,calcnt;
short int rmad[16],indis;
short int modcnt0,modcnt1,modcnt2,modcnt3,modcnt4,modcnt5,
          modcnt6,modcnt8,modcnt9;
short int mode,rimscmd,rmstat;
short int ixgn,ixrgn,iygn,izgn,ixan,iyan,izan;
short int ixgn1,ixrgn1,iygn1,izgn1,ixan1,iyan1,izan1;
short int ixan2,iyan2,izan2;
short int ixgp, ixrgp, iygp, izgp;
short int ixgp1, ixrgp1, iygp1, izgp1;
short int ixrg_16, ixg_16, iyg_16, izg_16;
short int ixa_16, iya_16, iza_16;
short int out1dis,out2dis;

short int cntflg,skpflg,rimsup,rimsstr;
short int sepcnt,sepdet,lnchdet;
short int intpwr,intpwron,launch;
short int load_tm,snum;

