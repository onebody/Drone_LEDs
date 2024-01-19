#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 32

#define startButton   3                                             //按钮
#define buttonON      LOW                                           //按钮按下时为低电平
 
struct Button {
  int buttonState = !buttonON;                                      //按钮状态变量，与按钮按下时的状态取反
  int lastButtonState = !buttonON;                                  //按钮状态初始化，与按钮按下时的状态取反
  long lastDebounceTime = 0;                                        //记录抖动变量
  long debounceDelay = 1;                                          //抖动时间变量
  bool flag = false;                                                //按钮flag
};
const int buttonPins[1] = {startButton};
Button button, buttons[1];                                          //新建1个按钮

int index = 0;
/****************************************set up and loop part*********************************/


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  initButtons();   

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)


}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {

 getButton(startButton); 

  if (index >= 4){
    index = 0;
  }


  if (index==3){
    colorWipe(strip.Color(255,   0,   0), 50); // 红
  }

 if (index==1){
    colorWipe(strip.Color(  0, 255,   0), 50); // 绿

  }

 if (index==2){
    colorWipe(strip.Color(  0,   0, 255), 50); // 蓝

  }

 if (index==0){
    colorWipe(strip.Color(  255,   255, 0), 50); // 黄
  }  


}


/****************************************按键 part****************************************/
/*初始化按键串口*/
void initButtons() {
  pinMode(startButton, INPUT_PULLUP);                              //适用于单个按钮
}
/*读取按键的状态*/
void getButton(int _buttonPin) {
  int reading = digitalRead(_buttonPin);                           //读取状态
  if (reading != button.lastButtonState) {                         //如果状态改变
    button.lastDebounceTime = millis();                            //更新时间
  }                                                                //如果等待大于debounceDelay
  if ((millis() - button.lastDebounceTime) > button.debounceDelay) {
    if (reading != button.buttonState) {                           //读取状态不等于按钮状态
      button.buttonState = reading;                                //更新状态
    
     if (button.buttonState == buttonON) {                        //判断按钮是否真的按下
        Serial.println("button is pressed");                       //输出按钮按下的文字
        button.flag = true;                                        //按钮flag为真
        index= index + 1;
        if (index >= 4){
          index = 0;
        }
      }
      else {
        button.flag = false;                                       //按钮flag为假
      }
    }
  }
  button.lastButtonState = reading;                                //更新last状态
} 


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
