String sunrise = "5:25 am";
String sunset = "5:49 pm";

String[]sunriseParse = split(sunrise, ":");
String[]sunsetParse = split(sunset, ":");

int sunriseHour, sunriseMinute, sunsetHour, sunsetMinute;

void setup() {
  sunriseParse[1] = sunriseParse[1].substring(0, 2);
  sunsetParse[1] = sunsetParse[1].substring(0, 2);

  sunriseHour = int(sunriseParse[0]);
  sunriseMinute = int(sunriseParse[1]);

  sunsetHour = int(sunsetParse[0])+12; //To compensate for API PM mode
  sunsetMinute = int(sunsetParse[1]); 

  if (hour()<sunriseHour || hour()==sunriseHour && minute()<=sunriseMinute) {
    println("case1");
    println("SUN IS DOWN");
  } else if (hour()>sunsetHour || hour()==sunsetHour && minute()>=sunsetMinute) { 
    println("case2");
    println("SUN IS DOWN");
  } else {
    println("case3");
    println("SUN IS UP");
  }
}


void draw() {
    if (hour()<sunriseHour || hour()==sunriseHour && minute()<=sunriseMinute) {
    println("case1");
    println("SUN IS DOWN");
  } else if (hour()>sunsetHour || hour()==sunsetHour && minute()>=sunsetMinute) { 
    println("case2");
    println("SUN IS DOWN");
  } else {
    println("case3");
    println("SUN IS UP");
  }
}