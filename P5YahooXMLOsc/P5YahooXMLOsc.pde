/*
 Loading XML Weather Data V3
 D.R.Patterson
 20/4/2016
 Update every 10 minutes
 
 Location codes:
 http://woeid.rosselliot.co.nz/
 */

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress sendAddress;

import controlP5.*;
ControlP5 cp5;
import static javax.swing.JOptionPane.*;

String server = "http://query.yahooapis.com/v1/public/yql?format=xml&q=select+*+from+weather.forecast+where+woeid=";
String lookupcode = "http://woeid.rosselliot.co.nz/";
// The location code we'll check for
String location = "727232+and+u='C'"; // Amsterdam in C
String thislocation = location;

// The URL for the XML document
String url;
// http://query.yahooapis.com/v1/public/yql?format=xml&q=select+*+from+weather.forecast+where+woeid=727232+and+u='C'"
XML xml;
int ystep;
int linespace = 18;
PFont font;
byte firsttime = 0;
int timenow, oldtime;
byte errcount = 0;


//Global variables we want to send to our program
int _temperature = 0;
float _windDirection = 0;
float _windSpeed = 0;
String _weatherType = "";
int _rising = 0;
int _counter = 0;

void setup() {
  font = createFont("arial.ttf", 14);
  textFont(font);
  size(500, 500);
  fill(#FFFF00);
  stroke(#FFFF00);
  background(#4040FF);


  sendAddress = new NetAddress("localhost", 12345);
  oscP5 = new OscP5(this, 12347);


  ystep = 24;
  cp5 = new ControlP5(this);
  PImage[] imgs = {loadImage("a.png"), loadImage("b.png"), loadImage("c.png")};
  cp5.addButton("changelocation")
    .setValue(1)
    .setPosition(width -imgs[0].width -20, height - imgs[0].height - 20)
    .setImages(imgs)
    .updateSize()
    ;
  PImage[] img = {loadImage("d.png"), loadImage("e.png"), loadImage("f.png")};
  cp5.addButton("showWebpage")
    .setValue(2)
    .setPosition(width - imgs[0].width - img[0].width - 30, height - img[0].height - 20)
    .setImages(img)
    .updateSize()
    ;
  oldtime = millis();
}

void draw() {
  //background(#4040FF);
  if (firsttime == 0) {
    // Put 1st time code here
    Cprintme("Loading: " + thislocation);
    firsttime = 1;
  } else {
    timenow = millis();
    if (firsttime == 1 || timenow - oldtime >= 600000) {
      oldtime = timenow;
      firsttime = 2;
      ystep = 24;
      // Load the XML document
      url = server + thislocation;
      xml = loadXML(url);
      background(#4040FF);
      //
      Cprintme("Location: " + thislocation); 
      ystep = ystep + linespace;
      // Grab the element we want
      XML myval = xml.getChild("results/channel/yweather:location");
      if (myval == null) {
        noLoop();
        errcount++;
        if (errcount<4) {
          // reset to default url
          thislocation = location;
          firsttime = 1;          
          javax.swing.JOptionPane.showMessageDialog(null, "The last location returned null.\r\nThe default location has been applied.");
          loop();
        } else {
          javax.swing.JOptionPane.showMessageDialog(null, "Error limit exceeded, stopping");
        }
      } else {
        try {
          // Get the attributes we want
          String city = myval.getString("city");
          Cprintme(city);
          ystep = ystep + linespace;

          myval = xml.getChild("results/channel/yweather:units");
          String tunits = trim( myval.getString("temperature") );
          String sunits = trim( myval.getString("speed") );

          myval = xml.getChild("results/channel/item/yweather:condition");
          String general = myval.getString("text");
          int temperature = myval.getInt("temp");
          //String date =  myval.getString("date");
          //Lprintme(date + ":", 20);   
          
          //GET THE WEATHER CODE HERE
          //<yweather:condition xmlns:yweather="http://xml.weather.yahoo.com/ns/rss/1.0" code="26" date="Tue, 24 May 2016 02:00 PM CEST" temp="13" text="Cloudy"/>
          int weatherCode = myval.getInt("code");
          
          int[]arrayCodes = {4,4,4,5,5,3,3,3,3,2,2,2,2,3,3,3,3,3,3,1,1,1,1,4,4,0,1,1,1,0,0,0,0,0,0,3,0,5,5,5,2,3,3,3,1,5,3,5,0};
          
          int groupedWeatherCode = arrayCodes[weatherCode];
          
          
          
          Lprintme("Weather Code: " + weatherCode, 20);
          Lprintme("groupedWeatherCode: " + groupedWeatherCode, 20);

          Lprintme("Current Weather: " + general+ ", "+ temperature + " " + tunits, 20);

          myval = xml.getChild("results/channel/yweather:wind");
          float chill = myval.getFloat("chill"); // yahoo now returns in Farenheit!
          chill = ftoc(chill);
          int direction = myval.getInt("direction");
          float speed = myval.getFloat("speed");
          float speedmph = speed;
          String showspeed = "Wind: " + speed + " " + sunits;
          if (sunits.equals("km/h") ) speedmph = speed / 1.60934400061;
          if (speed != speedmph) showspeed += " , " + nf(speedmph, 1, 2)+ " mph";
          Lprintme(showspeed, 20);
          showWind(speedmph, direction);
          Lprintme("Wind direction: " + direction, 20);

          //myval = xml.getChild("results/channel/yweather:astronomy");
          //String sunrise = myval.getString("sunrise");
          //String sunset = myval.getString("sunset");
          //Lprintme("Sunrise: " + sunrise, 20);
          //Lprintme("SunSet: " + sunset, 20);


          //Our global variables we want to send on
          _temperature = temperature;
          _windDirection = direction;
          _windSpeed = speedmph*1.60934400061; //Convert to km/h
          _weatherType = general;
          //_rising = rising;

          ystep = ystep + linespace;

          errcount = 0;
        } // end try
        catch(Exception e) {
          noLoop();
          errcount ++;
          if (errcount<4) {
            // reset to default url
            thislocation = location;
            firsttime = 0;           
            javax.swing.JOptionPane.showMessageDialog(null, "The last location returned null.\r\nThe default location has been applied.");
            loop();
          } else {
            javax.swing.JOptionPane.showMessageDialog(null, "Error limit exceeded, stopping");
          }
        } // end catch
      } // end else
    } // end firsttime = 1 or timeout
  } // end firsttime !=0


  //Sloppy rising indicator - redo this
  if (hour() < 12 ) {
    _rising = 0;
  } else if (hour() == 12 && (minute() < 22) ) {
    _rising = 0;
  } else {
    _rising = 1;
  }
  String[] risingCode = {"Rising (=0)", "Setting (=1)"};

  //Hack
  pushStyle(); //Does not get called??
  noStroke();
  fill(#4040FF);
  rect(0, ystep-15, 200, 25);
  fill(#FFFF00);
  popStyle();
  text("Sun is: " + risingCode[_rising], 20, ystep);
  //Lprintme("Sun is: " + risingCode[_rising], 20);


  //Print our vars every 300th frame (not in the very beginning to avoid variables being zero)
  if (frameCount % 300 == 1 && frameCount > 10) {
    
    println(
      "temperature: " + _temperature +
      "   windDirection: " + _windDirection + 
      "   windSpeed: " + _windSpeed + 
      "   weatherType: " + _weatherType +
      "   rising: " + _rising  +
      "   counter: " + _counter      
      );


    //OSC send
    OscMessage t = new OscMessage("/temperature");
    t.add( _temperature ); 
    oscP5.send(t, sendAddress);

    OscMessage wd = new OscMessage("/windDirection");
    wd.add( int(_windDirection) ); 
    oscP5.send(wd, sendAddress);

    OscMessage ws = new OscMessage("/windSpeed");
    ws.add( int(_windSpeed) ); 
    oscP5.send(ws, sendAddress);

    OscMessage wt = new OscMessage("/weatherType");
    wt.add( _weatherType ); 
    oscP5.send(wt, sendAddress);

    OscMessage r = new OscMessage("/rising");
    r.add( _rising ); 
    oscP5.send(r, sendAddress);
    
    OscMessage c = new OscMessage("/counter");
    c.add( _counter ); 
    oscP5.send(c, sendAddress);
    
    _counter++;
  }
}

float ftoc(float f) {
  f = (f-32) * 5/9;
  f = int(f *10 + 0.5);
  return (f/10);
}

void Cprintme(String temp) {
  float mywidth = textWidth(temp);
  text(temp, (width - mywidth) / 2, ystep);
  ystep = ystep + linespace;
}

void Lprintme(String temp, int margin) {
  text(temp, margin, ystep);
  ystep = ystep + linespace;
}

public void changelocation(int theValue) {
  if (firsttime >0) {
    String tmp = "Enter an Address code,\r\n";
    tmp += "full code for any location,\r\n";
    tmp += "or blank for default.\r\n";
    tmp += "Use the Code page button to find the codes:\r\n";
    tmp += lookupcode + "\r\n";
    tmp += "Invalid codes will reset location to default\r\n ";

    String newval = showInputDialog(tmp);
    if ( (newval == null) || (newval.isEmpty())  || (newval.length() < 4) ) {
      // revert to default
      thislocation = location;
    } else {
      thislocation = newval + "+and+u='C'";
    }
    firsttime = 1; // force reload before timeout
  }
}

public void showWebpage(int theValue) {
  if (firsttime >0) link(lookupcode);
}

void showWind(float speed, int direction) {
  float rcp = float(direction + 180 ) % 360;
  noFill();
  float w = textWidth("W");
  int xc = width - 150;
  int yc = ystep + 21;
  int r = 30;
  float r2 = 8;
  int angle = 40;
  ellipse(xc, yc, 2 * r, 2 * r);
  float xp = xc - r * cos( radians(270 - rcp));
  float yp = yc + r * sin( radians(270 - rcp));
  line(xc, yc, xp, yp );
  float theta = radians(direction + angle - 270 );
  float arrowx =  xp - r2  * cos(theta);
  float arrowy = yp - r2  * sin(theta);
  line(xp, yp, arrowx, arrowy );
  theta = radians(270 - direction + angle );
  arrowx =  xp - r2  * cos(theta);
  arrowy = yp + r2  * sin(theta);
  line(xp, yp, arrowx, arrowy );

  text("N", xc - w / 2, yc - r - 7);
  text("S", xc - w / 2, yc + r + 21);
  text("E", xc + r + w / 2, yc + 7);
  text("W", xc - r - 3 * w / 2, yc + 7);
}