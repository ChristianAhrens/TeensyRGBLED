/*
 Fading

 This example shows how to fade an LED using the analogWrite() function.

 The circuit:
 * LED attached from digital pin 9 to ground.

 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe

 http://arduino.cc/en/Tutorial/Fading

 This example code is in the public domain.

 */
#define INTERRUPT_PIN 6
 
#define RED     0xFF0000
#define GREEN   0x00FF00
#define BLUE    0x0000FF
#define WHT     0xFFFFFF
#define BLK     0x000000
#define YELLOW  0xFFFF00
#define MAGENTA 0xFF00FF
#define CYAN    0x00FFFF

#define MOONLIGHT 0x867E73 
#define SUNLIGHT  0xFFB54C
 
static const int g_ledPin1R = 4;
static const int g_ledPin1G = 5;
static const int g_ledPin1B = 9;
int g_ledPin1R_val = 0;
int g_ledPin1G_val = 0;
int g_ledPin1B_val = 0;

static const int g_ledPin2R = 10;
static const int g_ledPin2G = 12;
static const int g_ledPin2B = 14;
int g_ledPin2R_val = 0;
int g_ledPin2G_val = 0;
int g_ledPin2B_val = 0;

#define PLAY_RGB       0
#define PLAY_YMC       1
#define SWEEP_RGB      2
#define PLAY_RAND      3
#define SWEEP_MOON_SUN 4

volatile unsigned short g_Mode = PLAY_RGB;
volatile bool g_bAction = false;
volatile bool g_bExcessiveAction = false;
volatile int g_iLastActionTime = 0;
volatile int g_iLastFallingInterrupt = 0;

int extractR( unsigned long ulColor )
{
  unsigned long tmp = ulColor&0xFF0000;

  tmp = tmp>>16;

  return int(tmp);
}

int extractG( unsigned long ulColor )
{
  unsigned long tmp = ulColor&0x00FF00;

  tmp = tmp>>8;

  return int(tmp);
}

int extractB( unsigned long ulColor )
{
  unsigned long tmp = ulColor&0x0000FF;

  tmp = tmp<<0;

  return int(tmp);
}

bool setColor_LED1( unsigned long ulColor )
{
  int tmpR = extractR( ulColor );
  int tmpG = extractG( ulColor );
  int tmpB = extractB( ulColor );

  if( tmpR<0 || tmpR>255
   || tmpG<0 || tmpG>255
   || tmpB<0 || tmpB>255 )
  {
    g_ledPin1R_val = 0;
    g_ledPin1G_val = 0;
    g_ledPin1B_val = 0;
  
    analogWrite( g_ledPin1R, g_ledPin1R_val );
    analogWrite( g_ledPin1G, g_ledPin1G_val );
    analogWrite( g_ledPin1B, g_ledPin1B_val );
    
    return false;
  }

  g_ledPin1R_val = tmpR;
  g_ledPin1G_val = tmpG;
  g_ledPin1B_val = tmpB;

  analogWrite( g_ledPin1R, g_ledPin1R_val );
  analogWrite( g_ledPin1G, g_ledPin1G_val );
  analogWrite( g_ledPin1B, g_ledPin1B_val );

  return true;
}

bool setColor_LED2( unsigned long ulColor )
{
  int tmpR = extractR( ulColor );
  int tmpG = extractG( ulColor );
  int tmpB = extractB( ulColor );

  if( tmpR<0 || tmpR>255
   || tmpG<0 || tmpG>255
   || tmpB<0 || tmpB>255 )
  {
    g_ledPin2R_val = 0;
    g_ledPin2G_val = 0;
    g_ledPin2B_val = 0;
  
    analogWrite( g_ledPin2R, g_ledPin2R_val );
    analogWrite( g_ledPin2G, g_ledPin2G_val );
    analogWrite( g_ledPin2B, g_ledPin2B_val );
    
    return false;
  }

  g_ledPin2R_val = tmpR;
  g_ledPin2G_val = tmpG;
  g_ledPin2B_val = tmpB;

  analogWrite( g_ledPin2R, g_ledPin2R_val );
  analogWrite( g_ledPin2G, g_ledPin2G_val );
  analogWrite( g_ledPin2B, g_ledPin2B_val );

  return true;
}

inline void playRGB()
{
  setColor_LED1( RED );
  setColor_LED2( RED );
    delay( 1000 );
  setColor_LED1( GREEN );
  setColor_LED2( GREEN );
    delay( 1000 );
  setColor_LED1( BLUE );
  setColor_LED2( BLUE );
    delay( 1000 );
  setColor_LED1( BLK );
  setColor_LED2( BLK );
}

inline void playYMC()
{
  setColor_LED1( YELLOW );
  setColor_LED2( YELLOW );
    delay( 1000 );
  setColor_LED1( MAGENTA );
  setColor_LED2( MAGENTA );
    delay( 1000 );
  setColor_LED1( CYAN );
  setColor_LED2( CYAN );
    delay( 1000 );
  setColor_LED1( BLK );
  setColor_LED2( BLK );
}

inline void sweepRGB( int Time=7 )
{
  setColor_LED1( BLK );
  setColor_LED2( BLK );

  for ( ; g_ledPin1R_val <= 255; g_ledPin1R_val ++ ) {
    analogWrite(g_ledPin1R, g_ledPin1R_val);
    analogWrite(g_ledPin2R, g_ledPin1R_val);
    delay(Time);
  }
  for ( ; g_ledPin1R_val >= 0; g_ledPin1R_val -- ) {
    analogWrite(g_ledPin1R, g_ledPin1R_val);
    analogWrite(g_ledPin2R, g_ledPin1R_val);
    delay(Time);
  }

  for ( ; g_ledPin1G_val <= 255; g_ledPin1G_val ++ ) {
    analogWrite(g_ledPin1G, g_ledPin1G_val);
    analogWrite(g_ledPin2G, g_ledPin1G_val);
    delay(Time);
  }
  for ( ; g_ledPin1G_val >= 0; g_ledPin1G_val -- ) {
    analogWrite(g_ledPin1G, g_ledPin1G_val);
    analogWrite(g_ledPin2G, g_ledPin1G_val);
    delay(Time);
  }

  for ( ; g_ledPin1B_val <= 255; g_ledPin1B_val ++ ) {
    analogWrite(g_ledPin1B, g_ledPin1B_val);
    analogWrite(g_ledPin2B, g_ledPin1B_val);
    delay(Time);
  }
  for ( ; g_ledPin1B_val >= 0; g_ledPin1B_val -- ) {
    analogWrite(g_ledPin1B, g_ledPin1B_val);
    analogWrite(g_ledPin2B, g_ledPin1B_val);
    delay(Time);
  }
}

inline void playRand()
{
  setColor_LED1( (rand()&0xFF)*0x010000 + (rand()&0xFF)*0x000100 + (rand()&0xFF)*0x000001 );
  setColor_LED2( (rand()&0xFF)*0x010000 + (rand()&0xFF)*0x000100 + (rand()&0xFF)*0x000001 );
  delay(400);
}

inline void sweepColor( unsigned long ulColor, int iRampStepCnt, int iRampStepTime, unsigned short usLed )
{ 
  int iComponentR = extractR( ulColor );
  int iComponentG = extractG( ulColor );
  int iComponentB = extractB( ulColor );

  /*int iMaxAllowedSteps = min( min(iComponentR,iComponentG), iComponentB );
  if( iMaxAllowedSteps<iRampStepCnt )
    iRampStepCnt = iMaxAllowedSteps;*/

  int iStepWidthR = iComponentR / iRampStepCnt;
  int iStepWidthG = iComponentG / iRampStepCnt;
  int iStepWidthB = iComponentB / iRampStepCnt;

  int tmpR(0), tmpG(0), tmpB(0);
  while( tmpR<iComponentR
      || tmpG<iComponentG
      || tmpB<iComponentB )
  {
    if( tmpR<=iComponentR ) tmpR+=iStepWidthR;
    if( tmpG<=iComponentG ) tmpG+=iStepWidthG;
    if( tmpB<=iComponentB ) tmpB+=iStepWidthB;

    if( usLed & 0x1 )
      setColor_LED1( tmpR*0x10000 + tmpG*0x100 + tmpB );
    if( usLed & 0x2 )
      setColor_LED2( tmpR*0x10000 + tmpG*0x100 + tmpB );

    delay(iRampStepTime);
  }
  while( tmpR>0
      || tmpG>0
      || tmpB>0 )
  {
    if( tmpR>0 ) tmpR-=iStepWidthR;
    if( tmpG>0 ) tmpG-=iStepWidthG;
    if( tmpB>0 ) tmpB-=iStepWidthB;

    if( usLed & 0x1 )
      setColor_LED1( tmpR*0x10000 + tmpG*0x100 + tmpB );
    if( usLed & 0x2 )
      setColor_LED2( tmpR*0x10000 + tmpG*0x100 + tmpB );

    delay(iRampStepTime);
  }
}

inline void sweepMoonSun()
{
  int RampSteps = 64;
  int RampStepTime = 20;

  sweepColor( MOONLIGHT, RampSteps, RampStepTime, 0x1 );
  sweepColor( SUNLIGHT, RampSteps, RampStepTime, 0x2 );
}

inline void randSweep()
{
  int i=rand();
  switch( i%6 )
  {
  case 0:
    sweepColor( RED, 36, 2, 0x1 );
    break;
  case 1:
    sweepColor( RED, 36, 2, 0x2 );
    break;
  case 2:
    sweepColor( GREEN, 36, 2, 0x1 );
    break;
  case 3:
    sweepColor( GREEN, 36, 2, 0x2 );
    break;
  case 4:
    sweepColor( BLUE, 36, 2, 0x1 );
    break;
  case 5:
    sweepColor( BLUE, 36, 2, 0x2 );
    break;
  default:
    sweepColor( WHT, 36, 2, 0x3 );
    break;
  }
}

void handle_interrupt()
{
  if( digitalRead(INTERRUPT_PIN)==HIGH )
  {
    g_Mode = rand()%5;
    g_bAction = true;
  }
  else
  {  
    g_iLastFallingInterrupt = millis();
  }
}

void setup()
{
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), handle_interrupt, CHANGE);

  sweepColor( RED, 48, 2, 0x1 );
  sweepColor( GREEN, 48, 2, 0x2 );
  sweepColor( BLUE, 48, 2, 0x1 );
  sweepColor( YELLOW, 48, 2, 0x2 );
  sweepColor( MAGENTA, 48, 2, 0x1 );
  sweepColor( CYAN, 48, 2, 0x2 );

  g_Mode = SWEEP_RGB;
  g_bAction = false;
  g_iLastActionTime = millis();
}

void loop()
{
  // Tasterzustand pruefen
  if( digitalRead(INTERRUPT_PIN)==LOW )
  {
    int iTime = millis();
    if( iTime-g_iLastFallingInterrupt > 3000 )
    {
      g_bExcessiveAction = g_bExcessiveAction ? false : true;
      g_bAction = false;
      g_iLastFallingInterrupt = iTime;
    }
  }

  if( g_bExcessiveAction )
  {
    randSweep();
    g_iLastActionTime = millis();
  }
  else
  {
    // wenn Aktion durch Taster getriggert
    if( g_bAction )
    {
      g_bAction = false;
      switch( g_Mode%5 )
      {
        case PLAY_RAND:
          playRand();
          g_bAction = true;
          break;
        case PLAY_RGB:
          playRGB();
          break;
        case PLAY_YMC:
          playYMC();
          break;
        case SWEEP_RGB:
          sweepRGB();
          break;
        case SWEEP_MOON_SUN:
        default:
          sweepMoonSun();
          break;
      }
      
      g_iLastActionTime = millis();
    }
  
    // wenn lange nichts passiert ist, hinweisblinker
    int iTime = millis();
    if( g_iLastActionTime > iTime )
    {
      g_iLastActionTime = millis();
    }
    if( iTime-g_iLastActionTime > 10000 )
    {
      randSweep();
      g_iLastActionTime = millis();
    }
  }
}


