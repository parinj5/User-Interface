#include <Adafruit_TFTLCD.h> 
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define TS_MINX 204
#define TS_MINY 195
#define TS_MAXX 948
#define TS_MAXY 910

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 1
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#define BOXSIZE 70
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int X, Y, Z;
long Num1,shear_stress,frequency,Number;
char action;
boolean result = false;

String symbol[4][4] = {
  { "1", "2", "3", "4" },
  { "5", "6", "7", "8" },
  { "C", "9", "0", "=" },
};

void setup() 
{
  Serial.begin(9600);
  Serial.print("Starting...");
  
  tft.reset();
  tft.begin(0x9325);
  tft.setRotation(1);
  
  tft.fillScreen(BLACK);
  /Draw white frame
  tft.drawRect(0,0,320,240,WHITE);
  
  //Print "Welcome to" Text
  tft.setCursor(80,30);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Welcome to");
  
  //Print "Cellular Biomechanics Lab" text 
  tft.setCursor(80,100);
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.print("Cellular Biomechanics Lab");

  //Create Red Button
  tft.fillRect(60,180, 200, 40, RED);
  tft.drawRect(60,180,200,40,WHITE);
  tft.setCursor(80,188);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("START");
}

  void loop() 
 {
  TSPoint p = ts.waitTouch();  //Get touch point
  
       
   if(p.x>60 && p.x<260 && p.y>180 && p.y<220 && buttonEnabled) 
   // The user has pressed inside the red rectangle
   {
    buttonEnabled = false; //Disable button
        
    //This is important, because the libraries are sharing pins
   pinMode(XM, OUTPUT);
   pinMode(YP, OUTPUT);
    
    //Erase the screen
    tft.fillScreen(BLACK);
    
    //Draw frame
    tft.drawRect(0,0,320,240,WHITE);
    
    tft.setCursor(50,50);
    tft.setTextColor(WHITE);
    tft.setTextSize(4);
    tft.print("Enter shear stress: ");
    delay(300);

    draw_numpad();
    TSPoint p = waitTouch();
    X = p.x; 
    Y = p.y;
    //  Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y);
    detect_button();

    if (result==true) 
      shear_stress = Num1;
    result=false;

    //Erase the screen
    tft.fillScreen(BLACK);    
    //Draw frame
    tft.drawRect(0,0,320,240,WHITE);
    
    tft.setCursor(50,50);
    tft.setTextColor(WHITE);
    tft.setTextSize(4);
    tft.print("Enter frequency: ");
    delay(300);

    draw_numpad();

    TSPoint p = waitTouch();
    X = p.x; 
    Y = p.y;
    //  Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y);
    detect_button();

    if (result==true) 
    frequency = Num1;

    display_result();   
    
    }
  }

TSPoint waitTouch()
 {
  TSPoint p;
  do 
  {
    p = ts.getPoint(); 
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  } while(p.z>ts.pressureThreshhold);
  p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
  p.y = map(p.y, TS_MAXY, TS_MINY, 0, 240);
  return p;
}
    
void draw_numpad()
{//Erase the screen
  tft.fillScreen(BLACK);
    
  //Draw frame
  tft.drawRect(0,0,320,240,WHITE);
    
  //Draw the Result Box
  tft.fillRect(0, 0, 320, 60, CYAN);

  //Draw First Cloumn
  tft.fillRect  (60,0,60,80,BLACK);
  tft.fillRect  (120,0,60,80,BLACK);
  tft.fillRect  (180,0,60,80,RED);
  

 //Draw Second Column 
  tft.fillRect  (60,80,60,80,BLACK);
  tft.fillRect  (120,80,60,80,BLACK);
  tft.fillRect  (180,80,60,80,BLACK);

  //Draw Third Column 
  tft.fillRect  (60,160,60,80,BLACK);
  tft.fillRect  (120,160,60,80,BLACK);
  tft.fillRect  (180,160,60,80,BLACK);

  //Draw Fourth Column 
  tft.fillRect  (60,240,60,80,BLACK);
  tft.fillRect  (120,240,60,80,BLACK);
  tft.fillRect  (180,240,60,80,GREEN);

  //Draw Horizontal Lines
  for (int h=60; h<=240; h+=60)
  tft.drawFastHLine(h, 0, 320, WHITE);

  //Draw Vertical Lines
  for (int v=0; v<=320; v+=80)
  tft.drawFastVLine(60, v, 180, WHITE);

  //Display keypad lables
  for(i=1,i<4,i++)
  for(j=0,j<4,j++)
  {tft.setCursor(30*((2*i)+1), 40*((2*j)+1));
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.println(symbol[i-1][j]);
  }
}

void detect_button()
{
  
  if (Y>0 && Y<80) //Detecting Buttons on Column 1
  {
    if (X>180 && X<240) //If cancel Button is pressed
    { Serial.println ("Button Cancel"); 
      Number=Num1=Num2=0; 
      result=false;
    }
    
    if (X>120 && X<180) //If Button 5 is pressed
    { Serial.println ("Button 5"); 
      if (Number==0)
          Number=5;
      else
        Number = (Number*10) + 5; //Pressed twice
    }
    
    if (X>60 && X<120) //If Button 1 is pressed
    { Serial.println ("Button 1"); 
      if (Number==0)
        Number=1;
      else 
        Number = (Number*10) + 1; //Pressed twice
    }
  }

  if (Y<160 && Y>80) //Detecting Buttons on Column 2
  {
    if (X>180 && X<240)
    { Serial.println ("Button 9"); //Button 9 is Pressed
      if (Number==0)
          Number=9;
      else
          Number = (Number*10) + 9; //Pressed twice
    }

    if (X>120 && X<180)
    { Serial.println ("Button 6"); //Button 6 is Pressed
      if (Number==0)
          Number=6;
      else
         Number = (Number*10) + 6; //Pressed twice
    }
    
    if (X>60 && X<120)
    { Serial.println ("Button 2"); //Button 2 is Pressed
      if (Number==0)
          Number=2;
      else
          Number = (Number*10) + 2; //Pressed twice
    }
  }  
       
  if (Y>160 && Y<240) //Detecting Buttons on Column 3
  {
      
    if (X>180 && X<240)
    { Serial.println ("Button 0"); //Button 0 is pressed
      if (Number==0)
          Number=0;
      else
          Number = (Number*10) + 0; //Pressed twice
    }
    
    if (X>120 && X<180)
    { Serial.println ("Button 7"); //Button 7 is pressed
    if (Number==0)
        Number=7;
    else
        Number = (Number*10) + 7; //Pressed twice
    }
    
    if (X>60 && X<120)
    { Serial.println ("Button 3"); //Button 3 is pressed
    if (Number==0)
        Number=3;
    else
        Number = (Number*10) + 3; //Pressed twice
    }   
  }

  if (Y>240 && Y<320) //Detecting Buttons on Column 3
  { 
    if (X>180 && X<240)
    { Serial.println ("Button Equal"); 
    Num2=Number;
    result = true;
    }

    if (X>120 && X<180)
    { Serial.println ("Button 8"); //Button 8 is pressed
    if (Number==0)
        Number=8;
    else
        Number = (Number*10) + 8; //Pressed twice
    } 

    if (X>60 && X<120)
    { Serial.println ("Button 4"); //Button 4 is pressed
    if (Number==0)
        Number=4;
    else
        Number = (Number*10) + 4; //Pressed twice
    } 

  } 
  Num1 = Number;    
  Number = 0;  
  delay(300);
}  
void display_result()
{
  //Erase the screen
  tft.fillScreen(BLACK);    
  //Draw frame
  tft.drawRect(0,0,320,240,WHITE);
    
  tft.setCursor(100,60);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print("Shear stress: "); 
  tft.print(shear_stress);

  tft.setCursor(100,180);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print("Frequency: "); 
  tft.print(frequency);
}
