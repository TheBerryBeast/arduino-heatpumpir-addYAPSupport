#include "GreeHeatpumpIR.h"

// This is a protected method, i.e. generic Gree instances cannot be created
GreeHeatpumpIR::GreeHeatpumpIR() : HeatpumpIR()
{
}

GreeGenericHeatpumpIR::GreeGenericHeatpumpIR() : GreeHeatpumpIR()
{
  static const char model[] PROGMEM = "gree";
  static const char info[]  PROGMEM = "{\"mdl\":\"gree\",\"dn\":\"Gree\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
  greeModel = GREE_GENERIC;
}

GreeYANHeatpumpIR::GreeYANHeatpumpIR() : GreeHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyan";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyan\",\"dn\":\"Gree YAN\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
  greeModel = GREE_YAN;
}

// Support for YAA1FB, FAA1FB1, YB1F2 remotes
GreeYAAHeatpumpIR::GreeYAAHeatpumpIR() : GreeHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyaa";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyaa\",\"dn\":\"Gree YAA\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
  greeModel = GREE_YAA;
}

// Support for YAC1FBF remote
GreeYACHeatpumpIR::GreeYACHeatpumpIR() : GreeiFeelHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyac";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyac\",\"dn\":\"Gree YAC\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
  greeModel = GREE_YAC;
}

// Support for YT1F remote
GreeYTHeatpumpIR::GreeYTHeatpumpIR() : GreeiFeelHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyt";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyt\",\"dn\":\"Gree YT\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
  greeModel = GREE_YT;
}

// Support for YAP1F remote
GreeYAPHeatpumpIR::GreeYAPHeatpumpIR() : GreeiFeelHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyap";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyap\",\"dn\":\"Gree YAP\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
  greeModel = GREE_YAP;
}

void GreeHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd)
{
  send(IR, powerModeCmd, operatingModeCmd, fanSpeedCmd, temperatureCmd, swingVCmd, swingHCmd, false);
}

void GreeHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd, bool turboMode)
{
  send(IR, powerModeCmd, operatingModeCmd, fanSpeedCmd, temperatureCmd, swingVCmd, swingHCmd, turboMode, false);
}

void GreeHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd, bool turboMode, bool iFeelMode)
{
  (void)swingVCmd;
  (void)swingHCmd;

  // Sensible defaults for the heat pump mode

  uint8_t powerMode = GREE_AIRCON1_POWER_ON;
  uint8_t operatingMode = GREE_AIRCON1_MODE_HEAT;
  uint8_t fanSpeed = GREE_AIRCON1_FAN_AUTO;
  uint8_t swingV = GREE_VDIR_AUTO;
  uint8_t swingH = GREE_HDIR_AUTO;


  if (powerModeCmd == POWER_OFF)
  {
    powerMode = GREE_AIRCON1_POWER_OFF;
  }
  else
  {
    switch (operatingModeCmd)
    {
      case MODE_AUTO:
        operatingMode = GREE_AIRCON1_MODE_AUTO;
        temperatureCmd = 25;
        break;
      case MODE_HEAT:
        operatingMode = GREE_AIRCON1_MODE_HEAT;
        break;
      case MODE_COOL:
        operatingMode = GREE_AIRCON1_MODE_COOL;
        break;
      case MODE_DRY:
        operatingMode = GREE_AIRCON1_MODE_DRY;
        fanSpeedCmd = FAN_1;
        break;
      case MODE_FAN:
        operatingMode = GREE_AIRCON1_MODE_FAN;
        break;
    }
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = GREE_AIRCON1_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = GREE_AIRCON1_FAN1;
      break;
    case FAN_2:
      fanSpeed = GREE_AIRCON1_FAN2;
      break;
    case FAN_3:
      fanSpeed = GREE_AIRCON1_FAN3;
      break;
  }


  if (greeModel == GREE_YAN)
  {
    switch (swingVCmd)
    {
      case VDIR_AUTO:
      case VDIR_SWING:
        swingV = GREE_VDIR_AUTO;
        break;
      case VDIR_UP:
        swingV = GREE_VDIR_UP;
        break;
      case VDIR_MUP:
        swingV = GREE_VDIR_MUP;
        break;
      case VDIR_MIDDLE:
        swingV = GREE_VDIR_MIDDLE;
        break;
      case VDIR_MDOWN:
        swingV = GREE_VDIR_MDOWN;
        break;
      case VDIR_DOWN:
        swingV = GREE_VDIR_DOWN;
        break;
    }
  }

  if (greeModel == GREE_YAA || greeModel == GREE_YAC || greeModel == GREE_YT || greeModel == GREE_YAP)
  {
    switch (swingVCmd)
    {
      case VDIR_AUTO:
        swingV = GREE_VDIR_AUTO;
        break;
      case VDIR_SWING:
        swingV = GREE_VDIR_SWING;
        break;
      case VDIR_UP:
        swingV = GREE_VDIR_UP;
        break;
      case VDIR_MUP:
        swingV = GREE_VDIR_MUP;
        break;
      case VDIR_MIDDLE:
        swingV = GREE_VDIR_MIDDLE;
        break;
      case VDIR_MDOWN:
        swingV = GREE_VDIR_MDOWN;
        break;
      case VDIR_DOWN:
        swingV = GREE_VDIR_DOWN;
        break;
    }

    if (greeModel == GREE_YAC || greeModel == GREE_YAP)
    {
      switch (swingHCmd)
      {
        case HDIR_AUTO:
          if(greeModel == GREE_YAP) {
            break;
          } 
        case HDIR_SWING:
          swingH = GREE_HDIR_SWING;
          break;
        case HDIR_LEFT:
          swingH = GREE_HDIR_LEFT;
          break;
        case HDIR_MLEFT:
          swingH = GREE_HDIR_MLEFT;
          break;
        case HDIR_MIDDLE:
          swingH = GREE_HDIR_MIDDLE;
          break;
        case HDIR_MRIGHT:
          swingH = GREE_HDIR_MRIGHT;
          break;
        case HDIR_RIGHT:
          swingH = GREE_HDIR_RIGHT;
          break;
      }
    }

  }

  

  sendGree(IR, powerMode, operatingMode, fanSpeed, temperatureCmd, swingV, swingH, turboMode, iFeelMode);
}

// Send the Gree code
void GreeHeatpumpIR::sendGree(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH, bool turboMode, bool iFeelMode)
{
  (void)swingH;

  // 8 - 15 needed for YAP 
  uint8_t GreeTemplate[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  //                            0     1     2     3     4     5     6     7     8     9    10    11     12    13    14   15

  uint8_t i;

  if (greeModel != GREE_YT) {
    GreeTemplate[5] = 0x20;
  }

  // Set the Fan speed, operating mode and power state
  GreeTemplate[0] = fanSpeed | operatingMode | powerMode;
  // Set the temperature

  // YAP1F uses different bytes to display fahrenheit temperatures
  // Probably some bit manipulation here but could not figure out the pattern
  if(greeModel == GREE_YAP && (temperature >= 61 && temperature <= 86)) {
    int baseTemp = 60; // Starting from base temperature
    int tempIncrement = 2; // Initial increment value for lower bits

    // Calculate the difference between the input temperature and the base temperature
    int tempDifference = temperature - baseTemp;

    // Initialize Byte 1 and Byte 3
    GreeTemplate[1] = 0x0; // Initialize to 0

    GreeTemplate[3] = 0x5C; // Set to 0x5C for odd temperatures

    if(tempDifference % 2 == 0 && (temperature < 69 || temperature >= 79)) {
      GreeTemplate[3] = 0x58; 
    } else if(tempDifference % 2 != 0 && (temperature >= 69 && temperature < 79 || temperature == 61)) {
      GreeTemplate[3] = 0x58; 
    } 

    if (temperature >= 69) {
      tempDifference += 1;
    } 

    if (temperature >= 79) {
      tempDifference += 1;
    }

    // Increment Byte 1 based on temperature difference
    while (tempDifference >= tempIncrement && tempDifference > 1) {
        tempDifference -= tempIncrement;
        GreeTemplate[1] += 0x1; 

       /*
        if (GreeTemplate[3] == 0xA) {
            tempDifference -= 1;
        }
*/ 
    }
  } else {
    if (temperature > 15 && temperature < 31)
    {
      temperature = temperature - 16;
    } else {
      temperature = 21;
    }

    if(greeModel == GREE_YAP) {
      // Indicates C for YAP
      GreeTemplate[3] = 0x50;
    }

    GreeTemplate[1] = temperature;
  }

  // Gree YAN-specific
  if (greeModel == GREE_YAN)
  {
    GreeTemplate[2] = turboMode ? 0x70 : 0x60;
    GreeTemplate[3] = 0x50;
    GreeTemplate[4] = swingV;
  }
  if (greeModel == GREE_YAC)
  {
    GreeTemplate[4] |= (swingH << 4); // GREE_YT will ignore packets where this is set
  }
  if (greeModel == GREE_YAC || greeModel == GREE_YT || GREE_YAP)
  {
    if (iFeelMode)
    {
      if(greeModel == GREE_YAP) {
        GreeTemplate[5] |= GREE_YAP_IFEEL_BIT;
      } else {
        GreeTemplate[5] |= GREE_IFEEL_BIT;
      }
    }
  }
  if (greeModel == GREE_YT) {
    GreeTemplate[2] = GREE_LIGHT_BIT | GREE_HEALTH_BIT; // HEALTH is always on for GREE_YT
    GreeTemplate[3] = 0x50; // bits 4..7 always 0101

    if (turboMode)
    {
      GreeTemplate[2] |= GREE_TURBO_BIT;
    }
    if (swingV == GREE_VDIR_SWING)
    {
      GreeTemplate[0] |= GREE_VSWING; // Enable swing by setting bit 6
      GreeTemplate[4] = swingV;
    }
  }
  if (greeModel == GREE_YAA || greeModel == GREE_YAC)
  {
    GreeTemplate[2] = GREE_LIGHT_BIT; // bits 0..3 always 0000, bits 4..7 TURBO,LIGHT,HEALTH,X-FAN
    GreeTemplate[3] = 0x50; // bits 4..7 always 0101
    GreeTemplate[6] = 0x20; // YAA1FB, FAA1FB1, YB1F2 bits 4..7 always 0010

    if (turboMode)
    {
      GreeTemplate[2] |= GREE_TURBO_BIT;
    }
    if (swingV == GREE_VDIR_SWING)
    {
      GreeTemplate[0] |= GREE_VSWING; // Enable swing by setting bit 6
    }
    else if (swingV != GREE_VDIR_AUTO)
    {
      GreeTemplate[5] = swingV;
    }
  }

  if(greeModel == GREE_YAP) {
    GreeTemplate[2] = 0x20;
    GreeTemplate[5] = 0xC0;
    GreeTemplate[11] = 0xA0;
    GreeTemplate[15] = 0xA0;

    if (turboMode)
    {
      GreeTemplate[2] |= GREE_TURBO_BIT;
    }

    if (swingV == GREE_VDIR_SWING || swingH == GREE_HDIR_SWING)
    {
      GreeTemplate[0] |= GREE_VSWING; // Enable swing by setting bit 6
    }

    GreeTemplate[4] = (swingH << 4) | swingV;
  }

  // Calculate the checksum
  if (greeModel == GREE_YAN)
  {
    GreeTemplate[7] = (
      (GreeTemplate[0] << 4) +
      (GreeTemplate[1] << 4) +
      0xC0);
  }
  else
  {
    GreeTemplate[7] = (((
     (GreeTemplate[0] & 0x0F) +
     (GreeTemplate[1] & 0x0F) +
     (GreeTemplate[2] & 0x0F) +
     (GreeTemplate[3] & 0x0F) +
     ((GreeTemplate[5] & 0xF0) >> 4) +
     ((GreeTemplate[6] & 0xF0) >> 4) +
     ((GreeTemplate[7] & 0xF0) >> 4) +
      0x0A) & 0x0F) << 4) | (GreeTemplate[7] & 0x0F);
  }

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Send Header mark
  IR.mark(GREE_AIRCON1_HDR_MARK);
  IR.space(GREE_AIRCON1_HDR_SPACE);

  // Payload part #1
  for (i=0; i<4; i++) {
    IR.sendIRbyte(GreeTemplate[i], GREE_AIRCON1_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);
  }
  // Only three first bits of byte 4 are sent, this is always '010'
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ZERO_SPACE);
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ONE_SPACE);
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ZERO_SPACE);

  // Message space
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_MSG_SPACE);

  // Payload part #2
  for (i=4; i<=7; i++) {
    IR.sendIRbyte(GreeTemplate[i], GREE_AIRCON1_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);
	}

  // Message space
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_MSG_SPACE);

  // Send Header mark
  IR.mark(GREE_AIRCON1_HDR_MARK);
  IR.space(GREE_AIRCON1_HDR_SPACE);

  // Payload part #3
  for (i=8; i<=11; i++) {
    IR.sendIRbyte(GreeTemplate[i], GREE_AIRCON1_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);
	}

  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ZERO_SPACE);
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ONE_SPACE);
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ZERO_SPACE);

  // Message space
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_MSG_SPACE);

  for (i=12; i<=15; i++) {
    IR.sendIRbyte(GreeTemplate[i], GREE_AIRCON1_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);
	}

  // End mark
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(0);
}

// Sends current sensed temperatures, YAC remotes/supporting units only
void GreeiFeelHeatpumpIR::send(IRSender& IR, uint8_t currentTemperature)
{
  uint8_t GreeTemplate[] = { 0x00, 0x00 };

  GreeTemplate[0] = currentTemperature;
  GreeTemplate[1] = 0xA5;

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Send Header mark
  IR.mark(GREE_YAC_HDR_MARK);
  IR.space(GREE_YAC_HDR_SPACE);

  // send payload
  IR.sendIRbyte(GreeTemplate[0], GREE_YAC_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);
  IR.sendIRbyte(GreeTemplate[1], GREE_YAC_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);

  // End mark
  IR.mark(GREE_YAC_BIT_MARK);
  IR.space(0);
}
