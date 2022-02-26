class PFLAUSentenceDecoding{
public:
int GPS;
int ALARMLEVEL;
int RELATIVEBEARING;
int ALARMTYPE;
int RELATIVEVERTICAL;
String c;
String convert;
PFLAUSentenceDecoding(String a)
{c=a;
 int startingPFLAU=c.indexOf("$PFLAU");
 int endingPFLAU=c.indexOf('*',startingPFLAU+1);
 c=c.substring(startingPFLAU,endingPFLAU);
 int comma1=c.indexOf(',');
 int comma2=c.indexOf(',',comma1+1);
 int comma3=c.indexOf(',',comma2+1);
 int comma4=c.indexOf(',',comma3+1);
 int comma5=c.indexOf(',',comma4+1);
 int comma6=c.indexOf(',',comma5+1);
 int comma7=c.indexOf(',',comma6+1);
 int comma8=c.indexOf(',',comma7+1);
 int comma9=c.indexOf(',',comma8+1);
 convert=c.substring(comma3+1,comma4);          //GPS is always between third comma and 4 th comma
 GPS=convert.toInt();
 convert=c.substring(comma5+1,comma6);          //Alarm Level is between comma 5 and comma 6
 ALARMLEVEL=convert.toInt();
 convert=c.substring(comma6+1,comma7);
 RELATIVEBEARING=convert.toInt();
convert=c.substring(comma7+1,comma8);
 ALARMTYPE=convert.toInt();
 convert=c.substring(comma8+1,comma9);
 RELATIVEVERTICAL=convert.toInt();
 }
 
};
