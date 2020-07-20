// Sachin Iyer

// libraries
#include <Arduino.h>
#include <unity.h>
#include <MIDI.h>
#include <LiquidCrystal_I2C.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
// #include <PlaySynthMusic.h>
#include <USBHost_t36.h>

// initializations
// initializing lcd screens
LiquidCrystal_I2C lcd1(0x27,20,4);
LiquidCrystal_I2C lcd2(0x27,20,4);
LiquidCrystal_I2C lcd3(0x27,20,4);

// initializing pins
int usb_selector = 0;
int buttreg[] = {1,2,3,4,5};
int butt1[] = {26,27,28,29};
int butt2[] = {30,31,32,33};
int potentiometers[5];
int buttons[15];
int switches[5];
int sbuttons[12];
int tones[32][3];
int tonescomp[32];

// control values
const int control[][] = {{0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},{0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0},{0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},{0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1}};
const int sig1 = 24;
const int sig2 = 25;
const int controlChanges[] = {9,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};
const int voltage = 14;
const double scale127 = 0.007874;

// effect controls
int effectvolcontrol = 0;
double effectvol = 0;
// reverb
double reverbamount = 0;
double reverbamountcomp = 0;
// delay
int delaychannels = 0;
int delaychannelscomp = 0;
// bitcrusher
int bitbits = 0;
int bitbitscomp = 0;
// chorus
#define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
short chorusdelayline1[CHORUS_DELAY_LENGTH];
short chorusdelayline2[CHORUS_DELAY_LENGTH];
int chorusvoices = 2;
int chorusvoicescomp = 2;
// flanger
#define FLANGE_DELAY_LENGTH (6*AUDIO_BLOCK_SAMPLES)
short flangedelayline1[FLANGE_DELAY_LENGTH];
short flangedelayline2[FLANGE_DELAY_LENGTH];
int flangeoffsetnum = 4;
int flangeoffsetnumcomp = 4;
int flangedepthnum = 4;
int flangedepthnumcomp = 4;
int flangeoffset = FLANGE_DELAY_LENGTH / flangeoffsetnum;
int flangedepth = FLANGE_DELAY_LENGTH / flangedepthnum;
double flangefreq = 0.5;
double flangefreqcomp = 0.5;
// granular
#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];
double granularspeed = 0;
double granularspeedcomp = 0;
// audio shield sdcard
#define SDCARD_CS_PIN	 10
#define SDCARD_MOSI_PIN	 7
#define SDCARD_SCK_PIN	 14

// audio mapping
AudioSynthWaveformSine   sine13;         //xy=55,543.0000057220459
AudioSynthWaveformSine   sine14;         //xy=56,586.0000057220459
AudioSynthWaveformSine   sine11;         //xy=57,451.0000057220459
AudioSynthWaveformSine   sine12;         //xy=57,498.0000057220459
AudioSynthWaveformSine   sine15;         //xy=57,627.0000057220459
AudioSynthWaveformSine   sine10;         //xy=58,409.0000057220459
AudioSynthWaveformSine   sine9;          //xy=60,369.0000057220459
AudioSynthWaveformSine   sine17;         //xy=58.75,728.7500114440918
AudioSynthWaveformSine   sine16;         //xy=59,671.0000057220459
AudioSynthWaveformSine   sine6;          //xy=61,241.0000057220459
AudioSynthWaveformSine   sine7;          //xy=61,283.0000057220459
AudioSynthWaveformSine   sine18;         //xy=60,771.2500114440918
AudioSynthWaveformSine   sine8;          //xy=62,326.0000057220459
AudioSynthWaveformSine   sine3;          //xy=63,114.0000057220459
AudioSynthWaveformSine   sine4;          //xy=63,156.0000057220459
AudioSynthWaveformSine   sine22;         //xy=60,943.7500152587891
AudioSynthWaveformSine   sine23;         //xy=60,983.7500152587891
AudioSynthWaveformSine   sine1;          //xy=64,30.0000057220459
AudioSynthWaveformSine   sine2;          //xy=64,71.0000057220459
AudioSynthWaveformSine   sine20;         //xy=61.25,858.7500133514404
AudioSynthWaveformSine   sine5;          //xy=64,197.0000057220459
AudioSynthWaveformSine   sine24;         //xy=61.25,1023.7500152587891
AudioSynthWaveformSine   sine19;         //xy=62.5,815.0000123977661
AudioSynthWaveformSine   sine21;         //xy=62.5,902.5000143051147
AudioSynthWaveformSine   sine25;         //xy=62.5,1063.750015258789
AudioSynthWaveformSine   sine26;         //xy=63.75,1105.0000171661377
AudioSynthWaveformSine   sine28;         //xy=63.75,1186.2500171661377
AudioSynthWaveformSine   sine32;         //xy=63.75,1343.7500200271606
AudioSynthWaveformSine   sine31;         //xy=65.00000381469727,1303.7500190734863
AudioSynthWaveformSine   sine27;         //xy=66.25,1145.0000171661377
AudioSynthWaveformSine   sine29;         //xy=66.25,1223.7500190734863
AudioSynthWaveformSine   sine30;         //xy=66.25000381469727,1262.5000190734863
AudioEffectEnvelope      envelope11;     //xy=189,451.0000057220459
AudioEffectEnvelope      envelope14;     //xy=190,586.0000057220459
AudioEffectEnvelope      envelope4;      //xy=192,153.0000057220459
AudioEffectEnvelope      envelope10;     //xy=191,409.0000057220459
AudioEffectEnvelope      envelope16;     //xy=190,669.0000057220459
AudioEffectEnvelope      envelope5;      //xy=192,196.0000057220459
AudioEffectEnvelope      envelope12;     //xy=191,497.0000057220459
AudioEffectEnvelope      envelope2;      //xy=193,71.0000057220459
AudioEffectEnvelope      envelope3;      //xy=193,115.0000057220459
AudioEffectEnvelope      envelope15;     //xy=191,626.0000057220459
AudioEffectEnvelope      envelope13;     //xy=192,542.0000057220459
AudioEffectEnvelope      envelope9;      //xy=193,369.0000057220459
AudioEffectEnvelope      envelope6;      //xy=194,240.0000057220459
AudioEffectEnvelope      envelope1;      //xy=195,29.0000057220459
AudioEffectEnvelope      envelope7;      //xy=194,283.0000057220459
AudioEffectEnvelope      envelope8;      //xy=194,325.0000057220459
AudioEffectEnvelope      envelope18;     //xy=192.50000190734863,771.2500114440918
AudioEffectEnvelope      envelope19;     //xy=192.5,817.5000123977661
AudioEffectEnvelope      envelope23;     //xy=192.5,983.7500152587891
AudioEffectEnvelope      envelope24;     //xy=192.5,1027.500015258789
AudioEffectEnvelope      envelope17;     //xy=193.75,728.7500104904175
AudioEffectEnvelope      envelope25;     //xy=192.5,1065.0000171661377
AudioEffectEnvelope      envelope20;     //xy=193.75,860.0000123977661
AudioEffectEnvelope      envelope22;     //xy=193.75,943.7500133514404
AudioEffectEnvelope      envelope26;     //xy=193.75,1105.0000171661377
AudioEffectEnvelope      envelope28;     //xy=193.75,1185.0000176429749
AudioEffectEnvelope      envelope21;     //xy=195,902.5000133514404
AudioEffectEnvelope      envelope27;     //xy=195.00000190734863,1145.0000171661377
AudioEffectEnvelope      envelope32;     //xy=195.00000381469727,1343.7500190734863
AudioEffectEnvelope      envelope29;     //xy=196.25000381469727,1225.0000190734863
AudioEffectEnvelope      envelope30;     //xy=197.5,1265.0000190734863
AudioEffectEnvelope      envelope31;     //xy=198.75000381469727,1303.7500190734863
AudioMixer4              sinemixa3;         //xy=344,445.7500057220459
AudioMixer4              sinemixa2;         //xy=352,275.7500057220459
AudioMixer4              sinemixa4;         //xy=351.75000762939453,618.5000095367432
AudioMixer4              sinemixa1;         //xy=358,103.7500057220459
AudioMixer4              sinemixa5;         //xy=356.2500057220459,793.7500114440918
AudioMixer4              sinemixa7;        //xy=360.00000381469727,1127.500015258789
AudioMixer4              sinemixa6;         //xy=362.5000057220459,967.5000152587891
AudioMixer4              sinemixa8;        //xy=362.50000762939453,1286.2500190734863
AudioInputI2S            i2s1;           //xy=522.7500152587891,678.2500095367432
AudioMixer4              sinemixb1;        //xy=555.0000076293945,320.00000381469727
AudioMixer4              sinemixb2;        //xy=565.0000076293945,1003.7500152587891
AudioMixer4              effectmixa2;         //xy=654.5000114440918,796.0000076293945
AudioMixer4              effectmixa1;         //xy=657.5000114440918,596.0000076293945
AudioEffectDelay         delay2;         //xy=844.5000114440918,1052.0000076293945
AudioEffectFlange        flange2;        //xy=846.5000114440918,913.0000076293945
AudioEffectChorus        chorus2;        //xy=851.5000114440918,864.0000076293945
AudioEffectGranular      granular2;      //xy=851.5000114440918,1187.0000076293945
AudioEffectFreeverb      freeverb2;      //xy=853.5000114440918,958.0000076293945
AudioEffectBitcrusher    bitcrusher2;    //xy=855.5000114440918,1139.0000076293945
AudioEffectDelay         delay1;         //xy=871.5000114440918,340.00000762939453
AudioEffectFlange        flange1;        //xy=873.5000114440918,211.00000762939453
AudioEffectChorus        chorus1;        //xy=875.5000114440918,168.00000762939453
AudioEffectGranular      granular1;      //xy=874.5000114440918,489.00000762939453
AudioEffectFreeverb      freeverb1;      //xy=879.5000114440918,254.00000762939453
AudioEffectBitcrusher    bitcrusher1;    //xy=879.5000114440918,432.00000762939453
AudioMixer4              delaymixa4;        //xy=966.5000114440918,1085.0000076293945
AudioMixer4              delaymixa3;        //xy=967.5000114440918,1020.0000076293945
AudioMixer4              delaymixa1;         //xy=994.5000114440918,308.00000762939453
AudioMixer4              delaymixa2;        //xy=996.5000114440918,373.00000762939453
AudioMixer4              delaymixb2;        //xy=1096.5000114440918,1054.0000076293945
AudioMixer4              delaymixb1;        //xy=1127.5000114440918,338.00000762939453
AudioMixer4              effectmixb4;        //xy=1232.5000114440918,1128.0000076293945
AudioMixer4              effectmixb3;        //xy=1233.5000114440918,977.0000076293945
AudioMixer4              effectmixb1;        //xy=1265.5000114440918,241.00000762939453
AudioMixer4              effectmixb2;        //xy=1275.5000114440918,402.00000762939453
AudioMixer4              effectmixc2;        //xy=1391.5000114440918,1047.0000076293945
AudioMixer4              effectmixc1;        //xy=1432.5000114440918,317.00000762939453
AudioOutputI2S           i2s2;           //xy=1546.5000114440918,610.0000076293945
AudioConnection          patchCord1(sine13, envelope13);
AudioConnection          patchCord2(sine14, envelope14);
AudioConnection          patchCord3(sine11, envelope11);
AudioConnection          patchCord4(sine12, envelope12);
AudioConnection          patchCord5(sine15, envelope15);
AudioConnection          patchCord6(sine10, envelope10);
AudioConnection          patchCord7(sine9, envelope9);
AudioConnection          patchCord8(sine17, envelope17);
AudioConnection          patchCord9(sine16, envelope16);
AudioConnection          patchCord10(sine6, envelope6);
AudioConnection          patchCord11(sine7, envelope7);
AudioConnection          patchCord12(sine18, envelope18);
AudioConnection          patchCord13(sine8, envelope8);
AudioConnection          patchCord14(sine3, envelope3);
AudioConnection          patchCord15(sine4, envelope4);
AudioConnection          patchCord16(sine22, envelope22);
AudioConnection          patchCord17(sine23, envelope23);
AudioConnection          patchCord18(sine1, envelope1);
AudioConnection          patchCord19(sine2, envelope2);
AudioConnection          patchCord20(sine20, envelope20);
AudioConnection          patchCord21(sine5, envelope5);
AudioConnection          patchCord22(sine24, envelope24);
AudioConnection          patchCord23(sine19, envelope19);
AudioConnection          patchCord24(sine21, envelope21);
AudioConnection          patchCord25(sine25, envelope25);
AudioConnection          patchCord26(sine26, envelope26);
AudioConnection          patchCord27(sine28, envelope28);
AudioConnection          patchCord28(sine32, envelope32);
AudioConnection          patchCord29(sine31, envelope31);
AudioConnection          patchCord30(sine27, envelope27);
AudioConnection          patchCord31(sine29, envelope29);
AudioConnection          patchCord32(sine30, envelope30);
AudioConnection          patchCord33(envelope11, 0, sinemixa3, 2);
AudioConnection          patchCord34(envelope14, 0, sinemixa4, 1);
AudioConnection          patchCord35(envelope4, 0, sinemixa1, 3);
AudioConnection          patchCord36(envelope10, 0, sinemixa3, 1);
AudioConnection          patchCord37(envelope16, 0, sinemixa4, 3);
AudioConnection          patchCord38(envelope5, 0, sinemixa2, 0);
AudioConnection          patchCord39(envelope12, 0, sinemixa3, 3);
AudioConnection          patchCord40(envelope2, 0, sinemixa1, 1);
AudioConnection          patchCord41(envelope3, 0, sinemixa1, 2);
AudioConnection          patchCord42(envelope15, 0, sinemixa4, 2);
AudioConnection          patchCord43(envelope13, 0, sinemixa4, 0);
AudioConnection          patchCord44(envelope9, 0, sinemixa3, 0);
AudioConnection          patchCord45(envelope6, 0, sinemixa2, 1);
AudioConnection          patchCord46(envelope1, 0, sinemixa1, 0);
AudioConnection          patchCord47(envelope7, 0, sinemixa2, 2);
AudioConnection          patchCord48(envelope8, 0, sinemixa2, 3);
AudioConnection          patchCord49(envelope18, 0, sinemixa5, 1);
AudioConnection          patchCord50(envelope19, 0, sinemixa5, 2);
AudioConnection          patchCord51(envelope23, 0, sinemixa6, 2);
AudioConnection          patchCord52(envelope24, 0, sinemixa6, 3);
AudioConnection          patchCord53(envelope17, 0, sinemixa5, 0);
AudioConnection          patchCord54(envelope25, 0, sinemixa7, 0);
AudioConnection          patchCord55(envelope20, 0, sinemixa5, 3);
AudioConnection          patchCord56(envelope22, 0, sinemixa6, 1);
AudioConnection          patchCord57(envelope26, 0, sinemixa7, 1);
AudioConnection          patchCord58(envelope28, 0, sinemixa7, 3);
AudioConnection          patchCord59(envelope21, 0, sinemixa6, 0);
AudioConnection          patchCord60(envelope27, 0, sinemixa7, 2);
AudioConnection          patchCord61(envelope32, 0, sinemixa8, 3);
AudioConnection          patchCord62(envelope29, 0, sinemixa8, 0);
AudioConnection          patchCord63(envelope30, 0, sinemixa8, 1);
AudioConnection          patchCord64(envelope31, 0, sinemixa8, 2);
AudioConnection          patchCord65(sinemixa3, 0, sinemixb1, 2);
AudioConnection          patchCord66(sinemixa2, 0, sinemixb1, 1);
AudioConnection          patchCord67(sinemixa4, 0, sinemixb1, 3);
AudioConnection          patchCord68(sinemixa1, 0, sinemixb1, 0);
AudioConnection          patchCord69(sinemixa5, 0, sinemixb2, 0);
AudioConnection          patchCord70(sinemixa7, 0, sinemixb2, 2);
AudioConnection          patchCord71(sinemixa6, 0, sinemixb2, 1);
AudioConnection          patchCord72(sinemixa8, 0, sinemixb2, 3);
AudioConnection          patchCord73(i2s1, 0, effectmixa1, 3);
AudioConnection          patchCord74(i2s1, 1, effectmixa2, 0);
AudioConnection          patchCord75(sinemixb1, 0, effectmixa1, 0);
AudioConnection          patchCord76(sinemixb2, 0, effectmixa2, 3);
AudioConnection          patchCord77(effectmixa2, chorus2);
AudioConnection          patchCord78(effectmixa2, flange2);
AudioConnection          patchCord79(effectmixa2, delay2);
AudioConnection          patchCord80(effectmixa2, bitcrusher2);
AudioConnection          patchCord81(effectmixa2, granular2);
AudioConnection          patchCord82(effectmixa2, freeverb2);
AudioConnection          patchCord83(effectmixa1, chorus1);
AudioConnection          patchCord84(effectmixa1, flange1);
AudioConnection          patchCord85(effectmixa1, delay1);
AudioConnection          patchCord86(effectmixa1, bitcrusher1);
AudioConnection          patchCord87(effectmixa1, granular1);
AudioConnection          patchCord88(effectmixa1, freeverb1);
AudioConnection          patchCord89(delay2, 0, delaymixa3, 0);
AudioConnection          patchCord90(delay2, 1, delaymixa3, 1);
AudioConnection          patchCord91(delay2, 2, delaymixa3, 2);
AudioConnection          patchCord92(delay2, 3, delaymixa3, 3);
AudioConnection          patchCord93(delay2, 4, delaymixa4, 0);
AudioConnection          patchCord94(delay2, 5, delaymixa4, 1);
AudioConnection          patchCord95(delay2, 6, delaymixa4, 2);
AudioConnection          patchCord96(delay2, 7, delaymixa4, 3);
AudioConnection          patchCord97(flange2, 0, effectmixb3, 1);
AudioConnection          patchCord98(chorus2, 0, effectmixb3, 0);
AudioConnection          patchCord99(granular2, 0, effectmixb4, 2);
AudioConnection          patchCord100(freeverb2, 0, effectmixb3, 2);
AudioConnection          patchCord101(bitcrusher2, 0, effectmixb4, 1);
AudioConnection          patchCord102(delay1, 0, delaymixa1, 0);
pppAudioConnection          patchCord103(delay1, 1, delaymixa1, 1);
AudioConnection          patchCord104(delay1, 2, delaymixa1, 2);
AudioConnection          patchCord105(delay1, 3, delaymixa1, 3);
AudioConnection          patchCord106(delay1, 4, delaymixa2, 0);
AudioConnection          patchCord107(delay1, 5, delaymixa2, 1);
AudioConnection          patchCord108(delay1, 6, delaymixa2, 2);
AudioConnection          patchCord109(delay1, 7, delaymixa2, 3);
AudioConnection          patchCord110(flange1, 0, effectmixb1, 1);
AudioConnection          patchCord111(chorus1, 0, effectmixb1, 0);
AudioConnection          patchCord112(granular1, 0, effectmixb2, 2);
AudioConnection          patchCord113(freeverb1, 0, effectmixb1, 2);
AudioConnection          patchCord114(bitcrusher1, 0, effectmixb2, 1);
AudioConnection          patchCord115(delaymixa4, 0, delaymixb2, 3);
AudioConnection          patchCord116(delaymixa3, 0, delaymixb2, 0);
AudioConnection          patchCord117(delaymixa1, 0, delaymixb1, 0);
AudioConnection          patchCord118(delaymixa2, 0, delaymixb1, 3);
AudioConnection          patchCord119(delaymixb2, 0, effectmixb4, 0);
AudioConnection          patchCord120(delaymixb1, 0, effectmixb2, 0);
AudioConnection          patchCord121(effectmixb4, 0, effectmixc2, 3);
AudioConnection          patchCord122(effectmixb3, 0, effectmixc2, 0);
AudioConnection          patchCord123(effectmixb1, 0, effectmixc1, 0);
AudioConnection          patchCord124(effectmixb2, 0, effectmixc1, 3);
AudioConnection          patchCord125(effectmixc2, 0, i2s2, 1);
AudioConnection          patchCord126(effectmixc1, 0, i2s2, 0);
AudioControlSGTL5000 audioShield;

AudioSynthWaveformSine sines[] = {sine1, sine2, sine3, sine4, sine5, sine6, sine7, sine8, sine9, sine10, sine11, sine12, sine13, sine14, sine15, sine16, sine17, sine18, sine19, sine20, sine21, sine22, sine23, sine24, sine25, sine26, sine27, sine28, sine29, sine30, sine31, sine32};
AudioEffectEnvelope envelopes[] = {envelope1, envelope2, envelope3, envelope4, envelope5, envelope6, envelope7, envelope8, envelope9, envelope10, envelope11, envelope12, envelope13, envelope14, envelope15, envelope15, envelope16, envelope17, envelope18, envelope19, envelope20, envelope21, envelope22, envelope23, envelope24, envelope25, envelope26, envelope27, envelope28. envelope29, envelope30, envelope31. envelope32};


// setup

void setup() {
  // start serial monitor
  Serial.begin(115200);

  // audio shield initialization
  AudioNoInterrupts();
  AudioMemory(40);
  audioShield.enable();
  audioShield.volume(0.7);
  audioShield.autoVolumeEnable();

  // initialize effects
  chorus1.begin(chorusdelayline1, CHORUS_DELAY_LENGTH, chorusvoices);
  chorus2.begin(chorusdelayline2, CHORUS_DEALY_LENGTH, chorusvoices);
  flange1.begin(flangedelayline1, FLANGE_DELAY_LENGTH, flangeoffset, flangedepth, flangefreq);
  flange2.begin(flangedelayline2, FLANGE_DELAY_LENGTH, flangeoffset, flangedepth, flangefreq);
  freeverb1,damping(0.2);
  freeverb2.damping(0.2);
  bitcrusher1.sampleRate(44100);
  bitcrusher2.sampleRate(44100);
  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  granular2.begin(granularMemory, GRANULAR_MEMORY_SIZE);

  // initialize various inputs
  pinMode(usb_selector, INPUT_PULLUP);
  pinMode(sig1, INPUT_PULLUP);
  pinMode(sig2, INPUT_PULLUP);

  // initialize pins
  pinMode(voltage, INPUT_PULLUP);
  for (int i = 0; i < 5; i++) {
    pinMode(buttreg[i], INPUT_PULLUP);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(butt1[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(butt2[i], OUTPUT);
  }

  // initialize lcds
  lcd1.init();
  lcd1.backlight();
  lcd1.setCursor(1,0);
  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(1,0);
  lcd3.init();
  lcd3.backlight();
  lcd3.setCursor(1,0);
}


// reading values
void setValues() {
  addeffectvalues();
  int temp1[16];
  int temp2[16];
  for (int i = 0; i < 16; i++){
    for (int j = 0; j < 4, j++) {
      if (control[i][j] == 1) {
	digitalWrite(butt1[j], HIGH);
	digitalWrite(butt2[j], HIGH);
      }
    }
    temp1[i] = digitalRead(sig1);
    temp2[i] = digitalRead(sig2);
    for (int j = 0; j < 4; j++) {
      digitalWrite(butt1[j], LOW);
      digitalWrite(butt2[j], LOW);
    }
  }
  for (int i = 0; i < 5; i++) {
    potentiometers[i] = temp1[i];
    switches[i] = temp2[i];
  }
  for (int i = 5; i < 15; i++) {
    buttons[i] = temp1[i];
  }
  for (int i = 5; i < 16; i++) {
    sbuttons[i - 5] = temp2[i];
  }

}

// scaling voltage values
void scaleValues() {
  int fullVal = 1027;
  int volt = analogRead(voltage);
  Serial.print(volt);
  double scale = fullVal / volt;
  for (int i = 0; i < 5; i++) {
    potentiometers[i] = potentiometers[i] * scale;
  }
}

// center display
void display1() {
  lcd1.setCursor(1,0);
  lcd1.print("Mode:");
  lcd1.print(usb_selector);
  lcd1.setCursor(2,0);
  lcd1.print("Button1:");
  lcd1.print();
  lcd1.setCursor(3,0);
  lcd1.print("Button2:");
  lcd1.setCursor(4,0);
  lcd1.print();
  lcd1.print("Button3:");
  lcd1.print();
}

// left display
void display2() {
  lcd2.setCursor(1,0);
  if (effectvolcontrol == 0) {
    lcd2.print("chorus");
  }
  else if (effectvolcontrol == 1) {
    lcd2.print("flange");
  }
  else if (effectvolcontrol == 2) {
    lcd2.print("reverb");
  }
  else if (effectvolcontrol == 3) {
    lcd2.print("delay");
  }
  else if (effectvolcontrol == 4) {
    lcd2.print("bitcrush");
  }
  else if (effectvolcontrol == 5) {
    lcd2.print("granular");
  }
  lcd2.setCursor(2,0);
  lcd2.print("But1:");
  lcd2.print();
  lcd2.print(" But2:");
  lcd2.print();
  lcd2.setCursor(3,0);
  lcd2.print("But3:");
  lcd2.print();
  lcd2.print(" But4:");
  lcd2.print();
  lcd2.setCursor(4,0);
  lcd2.print("But5:");
  lcd2.print();
  lcd2.print(" But6:");
  lcd2.print();
}

// right display
void display3() {
  lcd3.setCursor(1,0);
  lcd3.print("P1:");
  lcd3.print();
  lcd3.setCursor(2,0);
  lcd3.pring("P2:");
  lcd3.print();
  lcd3.setCursor(3,0);
  lcd3.print("P3:");
  lcd3.print();
  lcd3.setCursor(4,0);
  lcd3.print("P4:");
  lcd3.print();
  lcd3.print(" P5:");
  lcd3.print();
}

// display values
void displayValues() {
  display1();
  display2();
  display3();
}

// send midi values
void sendMidi() {
  for (int i = 0; i < 5; i++) {
    Serial.print("Potentiometers");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(potentiometers[i]);
    sendControlChange(controlChanges[i], potentiometers[i], 0);
  }

  usbMIDI.send_now();
  for (int i = 0; i < 15; i++) {
    Serial.print("Buttons");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(buttons[i]);
    sendControlChange(controlChanges[i + 5], buttons[i], 0);
  }
  usbMIDI.send_now();
  for (int i = 0; i < 5; i++) {
    Serial.print("Switches");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(switches[i]);
    sendControlChange(controlChanges[i+20], switches[i], 0);
  }
  usbMIDI.send_now();
  for (int i = 0; i < 12; i++) {
    Serial.print("SButtons");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(sbuttons[i]);
    sendControlChange(controlChanges[i+25], sbuttons[i], 0);
  }
  usbMIDI.send_now();
}

// read Midi (tone[0] = note, tone[1] = velocity, tone[2] = aftertouch)
void readMidi() {

}
double returnFrequency(int note) {
  return (440*pow(2, ((note-69)/12)));
}

// start note
void startNote(int voice, int values[]) {
  sines[voice].frequency(returnFrequency[values[0]]);
  sines[voice].amplitude(values[1]*scale127);
  envelopes[voice].noteOn();
}

// stop note
void stopNote(int voice) {
  envelopes[voice].noteOff();
}

// mod sine
void modSine(int voice, int after) {
  envelopes[voice].sustain(after*scale127);
}

// send tones
void sendTone() {
  for (int i = 0; i < 32; i++) {
    if (tones[i] != -1) {
      modSine(i, tones[i][2]);
      if (tones[i][0] != tonescomp[i]) {
	stopNote(i);
      }
    }
    else {
      if (tones[i][0] != tonescomp[i]) {
	startNote(i, tones);
      }
    }
  }
}

// mod effect values
void addeffectvalues() {
  if (digitalRead(sbuttons[0]) == 1) {
    if (effectvol < 1) {
      effectvol = effectvol + 0.1;
    }
  }
  if (digitalRead(sbuttons[1]) == 1) {
    if (effectvol > 0) {
      effectvol = effectvol - 0.1;
    }
  }
  if (digitalRead(sbuttons[2]) == 1) {
    if (effectvolcontrol < 5) {
      effectvolcontrol = effectvolcontrol + 1;
    }
  }
  if (digitalRead(sbuttons[3]) == 1) {
    if (effectvolcontrol > 0) {
      effectvolcontrol = effectvolcontrol - 1;
    }
  }
  if (digitalRead(sbuttons[4]) == 1) {
    if (chorusvoices < 6) {
      chorusvoices = chorusvoices + 1;
    }
  }
  if (digitalRead(sbuttons[5]) == 1) {
    if (chorusvoices > 0) {
      chorusvoices = chorusvoices - 1;
    }
  }
  if (digitalRead(sbuttons[6]) == 1) {
    if (flangeoffsetnum < 8) {
      flangeoffsetnum = flangeoffsetnum + 1;
    }
  }
  if (digitalRead(sbuttons[7]) == 1) {
    if (flangeoffsetnum > 0) {
      flangeoffsetnum = flangeoffsetnum - 1;
    }
  }
  if (digitalRead(sbuttons[8]) == 1) {
    if (flangedepthnum < 8) {
      flangedepthnum = flangedepthnum + 1;
    }
  }
  if (digitalRead(sbuttons[9]) == 1) {
    if (flangedepthnum > 0) {
      flangedepthnum = flangedepthnum - 1;
    }
  }
  if (digitalRead(sbuttons[10]) == 1) {
    if (flangefreq < 1.0) {
      flangefreq = flangefreq + 0.1;
    }
  }
  if (digitalRead(sbuttons[11]) == 1) {
    if (flangefreq > 0) {
      flangefreq = flangefreq - 0.1;
    }
  }
  if (digitalRead(sbuttons[12]) == 1) {
    if (reverbamount < 1) {
      reverbamount = reverbamount + 0.1;
    }
  }
  if (digitalRead(sbuttons[13]) == 1) {
    if (reverbamount > 0) {
      reverbamount = reverbamount - 0.1;
    }
  }
  if (digitalRead(sbuttons[14]) == 1) {
    if (delaychannels < 8) {
      delaychannels = delaychannels + 1;
    }
  }
  if (digitalRead(sbuttons[15]) == 1) {
    if (delaychannels > 0) {
      delaychannels = delaychannels - 1;
    }
  }
  if (digitalRead(sbuttons[16]) == 1) {
    if (bitbits < 16) {
      bitbits = bitbits + 1;
    }
  }
  if (digitalRead(sbuttons[17]) == 1) {
    if (bitbits > 1) {
      bitbits = bitbits - 1;
    }
  }
  if (digitalRead(sbuttons[18]) == 1) {
    if (granularspeed < 8.0) {
      granularspeed = granularspeed + 0.25;
    }
  }
  if (digitalRead(sbuttons[19]) == 1) {
    if (granularspeed > 0.25) {
      granularspeed = granularspeed - 0.25;
    }
  }
}

// chorus
void modchorus() {
  if (chorusvoices != chorusvoicescomp) {
    chorus1.voices(chorusvoices);
    chorus2.voices(chorusvoices);
    chorusvoicescomp = chorusvoices;
  }
}

// flanger
void modflanger() {
  if (flangeoffsetnum != flangeoffsetnumcomp || flangedepthnum != flangedepthnumcomp || flangefreq != flangefreqcomp) {
    flange1.voices(flangeoffset, flangedepth, flangefreq);
    flange2.voices(flangeoffset, flangedepth, flangefreq);
    flangeoffsetnumcomp = flangeoffsetnum;
    flangedepthnum = flangedepthnumcomp;
    flangefreq = flangefreqcomp;
  }
}

// reverb
void modfreeverb() {
  if (reverbamount != reverbamountcomp) {
    freeverb1.roomsize(reverbamount);
    freeverb2.roomsize(reverbamount);
    reverbamountcomp = reverbamount;
  }
}

// delay
void moddelay() {
  int delayamount = 110;
  if (delaychannelscomp != delaychannels) {
    for (int i = 0; i < 8; i++) {
      try {
	delay1.disable(i);
	delay2.disable(i);
      }
      catch (...) {
	Serial.print("Already disabled");
      }
    }
    for (int i = 0; i < delaychannels; i++) {
      delay1.delay(i, delayamount);
      delay2.delay(i, delayamount);
      delayamount = delayamount + 10;
    }
    delaychannelscomp = delaychannels;
  }
}

// bitcrusher
void modbitcrusher() {
  if (bitbits != bitbitscomp) {
    bitcrusher1.bits(bitbits);
    bitcrusher2.bits(bitbits);
    bitbitscomp = bitbits;
  }
}

// grandular
void modgranular() {
  if (granularspeed != granularspeedcomp) {
    granular1.setSpeed(granularspeed);
    granular2.setSpeed(granularspeed);
    granularspeedcomp = granularspeed;
  }
}

void modvolume() {
  if (effectvolcontrol < 3) {
    effectmixb1.gain(effectvolcontrol, effectvol);
    effectmixb3.gain(effectvolcontrol, effectvol);
  }
  else {
    effectmixb2.gain(effectvolcontrol - 3, effectvol);
    effectmixb4.gain(effectvolcontrol - 3, effectvol);

  }
}

// addFilters
void addFilters() {
  modchorus();
  modflanger();
  modfreeverb();
  moddelay();
  modbitcrusher();
  modgranular();
  modvolume();
}

// mod values
void modValues() {
  setValues();
  scaleValues();
  sendValues();
}

// send Values
void sendValues() {
  if (usb_selector == 0) {
    sendMidi();
  }
  else if (usb_selector == 1) {
    readMidi();
    sendTone();
    addFilters();
  }
  else {
    addFilters();
  }
  displayValues();
}

// select USB
void selectUsb() {
  if (usb_selector < 2) {
    usb_selector = usb_selector + 1;
  }
  else {
    usb_selector = 0;
  }
}

// loop
void loop() {
  selectUsb();
  sendValues();
  modValues();
  delay(1);
}
