#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "îrslinger/irslinger.h"

// ----------------------------------------- ENUMS -----------------------------------------

enum onOff {Off = 0, On = 1};
enum mode {Auto = 0, Cool = 1, Dry = 2, Fan = 3, Heat = 4};

// ----------------------------------------- UTIL -----------------------------------------

void toBinary(unsigned int in, int count, int* out)
{
    unsigned int mask = 1U << (count-1);
    int i;
    for (i = 0; i < count; i++) {
        out[i] = (in & mask) ? 1 : 0;
        in <<= 1;
    }
}

// ----------------------------------------- MAIN -----------------------------------------

int main(int argc, char *argv[])
{
	// ----------------------------------------- PREP -----------------------------------------

	uint32_t outPin = 17;
	int frequency = 38000;
	double dutyCycle = 0.5;

	int startSignal = 9000;
	int startPause = 4500;

	int signal = 610;
	int signalZero = 610;
	int signalOne = 1645;

	int signalSeparator = 20000;

	int endSignal = 610;
	int endPause = 40000;

	int message[] = 
	{
		// start 0 | 0-1
		startSignal, startPause,
		// mode 1-3 | 2-6
		signal, signalZero, signal, signalZero, signal, signalZero,
		// power 4 | 7-8
		signal, signalZero,
		// fan 5-6 | 9-12
		signal, signalZero, signal, signalZero,
		// swing 7 | 13-14
		signal, signalZero,
		// sleep 8 | 15-16
		signal, signalZero,
		// temp 9-12 | 17-24
		signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero,
		// timing one and two, 13-20 | 25-40
		signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero,
		// fan4 21 | 41-42
		signal, signalZero,
		// unknown 1 | 22 | 43-44
		signal, signalOne,
		// power 23 | 45-46
		signal, signalZero,
		// unknown 0 24 | 47-48
		signal, signalZero,
		// ventilation 25 | 49-50
		signal, signalZero,
		// unknown 0001010010 26-35 | 51-70
		signal, signalZero, signal, signalZero, signal, signalZero, signal, signalOne, signal, signalZero, signal, signalOne, signal, signalZero, signal, signalZero, signal, signalOne, signal, signalZero,
		// separator 36 | 71-72
		signal, signalSeparator,
		// swing UD 37 | 73-74
		signal, signalZero,
		// unknown 000 38-40 | 75-80
		signal, signalZero, signal, signalZero, signal, signalZero,
		// swing LR 41 | 81-82
		signal, signalZero,
		// unknown 000 42-44 | 83-88
		signal, signalZero, signal, signalZero, signal, signalZero,
		// temp display 45 | 89-90
		signal, signalZero,
		// unknown 00001000000000000 46-62 | 91-124
		signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalOne, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero,
		// energy saving 63 | 125-126
		signal, signalZero,
		// unknown 0 64 | 127-128
		signal, signalZero,
		// checksum 65-68 | 129-136
		signal, signalZero, signal, signalZero, signal, signalZero, signal, signalZero,
		// end 69 | 137-138
		endSignal, endPause
	};

	// ----------------------------------------- CURRENT INPUT -----------------------------------------

	enum onOff power = On;
	enum onOff swing = Off;
	enum mode acMode = Cool;
	unsigned int fan = 3;
	unsigned int temp = 26;

	// 1: on/off | 2: mode | 3: temp | 4: fan | 5: swing

	power = strcmp(argv[1], "on") == 0 ? On : Off;
	
	if (strcmp(argv[2], "auto") == 0) acMode = Auto;
	if (strcmp(argv[2], "cool") == 0) acMode = Cool;
	if (strcmp(argv[2], "dry") == 0) acMode = Dry;
	if (strcmp(argv[2], "fan") == 0) acMode = Fan;
	if (strcmp(argv[2], "heat") == 0) acMode = Heat;

	temp = atoi(argv[3]);

	fan = atoi(argv[4]);

	swing = strcmp(argv[5], "on") == 0 ? On : Off;

	//printf("power: %d; mode %d; temp: %d; fan: %d; swing: %d\n", power, acMode, temp, fan, swing);

	// ----------------------------------------- MESSAGE MODIFY -----------------------------------------

	int binary[8]; // shouldn't get larger results

	// power	
	message[4*2+1] = power == Off ? signalZero : signalOne;
	message[23*2+1] = power == Off ? signalZero : signalOne;

	// mode
	toBinary(acMode, 8, binary);
	message[1*2+1] = binary[7] == 1 ? signalOne : signalZero;
	message[2*2+1] = binary[6] == 1 ? signalOne : signalZero;
	message[3*2+1] = binary[5] == 1 ? signalOne : signalZero;

	//swing
	message[7*2+1] = swing == On ? signalOne : signalZero;
	message[37*2+1] = swing == On ? signalOne : signalZero;
	message[41*2+1] = swing == On ? signalOne : signalZero;

	// fan
	if (fan < 4) 
	{
		toBinary(fan, 8, binary);
		message[5*2+1] = binary[7] == 1 ? signalOne : signalZero;
		message[6*2+1] = binary[6] == 1 ? signalOne : signalZero;
		message[21*2+1] = signalZero;// binary[5] == 1 ? signalOne : signalZero;
	} 
	if (fan == 4) 
	{
		message[5*2+1] = signalOne;
		message[6*2+1] = signalOne;
		message[21*2+1] = signalOne;
	}
	

	// temperature
	toBinary(temp - 16, 8, binary);
	message[9*2+1] = binary[7] == 1 ? signalOne : signalZero;
	message[10*2+1] = binary[6] == 1 ? signalOne : signalZero;
	message[11*2+1] = binary[5] == 1 ? signalOne : signalZero;
	message[12*2+1] = binary[4] == 1 ? signalOne : signalZero;

	// ----------------------------------------- CALCULATE CHECKSUM -----------------------------------------

	int checksum = (acMode - 1) + (temp - 16) + 5 + swing + 0 + 0 - ((1-power) * 8); // last two are bits 25 and 63, always 0
	toBinary(checksum, 8, binary);
	message[65*2+1] = binary[7] == 1 ? signalOne : signalZero;
	message[66*2+1] = binary[6] == 1 ? signalOne : signalZero;
	message[67*2+1] = binary[5] == 1 ? signalOne : signalZero;
	message[68*2+1] = binary[4] == 1 ? signalOne : signalZero;

	// ----------------------------------------- MESSAGE SEND -----------------------------------------

	for(int i = 0; i < sizeof(message) / sizeof(int); i++)
	{
		printf("%d:%d, ", i, message[i]);
	}

	int result = 0;
	
	result = irSlingRaw(
		outPin,
		frequency,
		dutyCycle,
		message,
		sizeof(message) / sizeof(int));
	
	return result;
}
