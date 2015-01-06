#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  turnSel,        sensorDigitalIn)
#pragma config(Sensor, I2C_1,  ime_lift,       sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ime_right,      sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ime_left,       sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           claw,          tmotorVex269, openLoop)
#pragma config(Motor,  port2,           left1,         tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port3,           left2,         tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_3, 1000)
#pragma config(Motor,  port4,           right1,        tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port5,           right2,        tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port6,           lift1,         tmotorVex393, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port7,           lift2,         tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port8,           lift3,         tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port9,           lift4,         tmotorVex393, openLoop)
#pragma config(Motor,  port10,           ,             tmotorVex269, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
#include "lcdAPI.h"

const int CLAW_OPEN = 140;
const int LIFT_HIGH = 3050;
const int LIFT_LOW = 5;
const int LIFT_SPEED = 96;
const bool UP = 1;
const bool DOWN = 0;
const double IME_S_TICKS = 392;
const double IME_T_TICKS = 627.2;
void setR(int speed)
{
	motor[right1] = motor[right2] = speed;
}

void setL(int speed)
{
	motor[left1] = motor[left2] = speed;
}

void lift(int speed, bool up = true)
{
	motor[lift1] = motor[lift2] = motor[lift3] = motor[lift4] = speed * (up?1:-1);
}

void lift(bool dir)
{
	if (dir)
	{
		while (SensorValue[ime_lift] <= LIFT_HIGH)
		{
			motor[lift1] = motor[lift2] = motor[lift3] = motor[lift4] = LIFT_SPEED;
		}
		motor[lift1] = motor[lift2] = motor[lift3] = motor[lift4] = 3;
		return;
		//SensorValue[ime_lift] = LIFT_HIGH;
	}
	else
	{
		while (SensorValue[ime_lift] >= LIFT_LOW)
		{
			motor[lift1] = motor[lift2] = motor[lift3] = motor[lift4] = -LIFT_SPEED;
		}
		motor[lift1] = motor[lift2] = motor[lift3] = motor[lift4] = 3;
		SensorValue[ime_lift] = 0;
		return;
	}

}

void clawControl(int speed)
{
	motor[claw] = speed;
}

int getEncoder()
{
	return (nMotorEncoder[right2]+nMotorEncoder[left2])/2;
}

void fwd(int inches)
{
	nMotorEncoder[right2] = nMotorEncoder[left2] = 0;
	setL(127);
	setR(127);
	while(getEncoder()<(inches / (4*3.14))*IME_S_TICKS){}
	setL(0);
	setR(0);
}

void diag()
{
string mainBattery, backupBattery;
while(nLCDButtons!=0){}
while(nLCDButtons!=2)                                       // An infinite loop to keep the program running until you terminate it
{
clearLCDLine(0);                                            // Clear line 1 (0) of the LCD
clearLCDLine(1);                                            // Clear line 2 (1) of the LCD

//Display the Primary Robot battery voltage
displayLCDString(0, 0, "Primary: ");
sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V'); //Build the value to be displayed
displayNextLCDString(mainBattery);

//Display the Backup battery voltage
displayLCDString(1, 0, "Backup: ");
sprintf(backupBattery, "%1.2f%c", BackupBatteryLevel/1000.0, 'V');    //Build the value to be displayed
displayNextLCDString(backupBattery);

//Short delay for the LCD refresh rate
wait1Msec(100);
}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////
int Program;
void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...

  short leftButton = 1;
	short centerButton = 2;
	short rightButton = 4;
  int choice1;
  int choice2;
  bLCDBacklight = true;
  bool inMenu = true;
 	while(inMenu)
  {
  	lcdClear();
  	lcdWaitForBtnUp();
  	lcd_printf("\tChoice 1\nBLU\tDiag\tRED");
		lcdWaitForBtnDown();
  	if (lcdIsBtnDown(leftButton))
  	{
  		choice1=0;
  	}
  	if (lcdIsBtnDown(rightButton))
  	{
  		choice1=1;
  	}
  	if (lcdIsBtnDown(centerButton))
  	{
  		diag();
  		continue;
  	}

  	lcdClear();
  	lcdWaitForBtnUp();
		lcd_printf("\tChoice 2\nAuto\tBack\tPole");
		lcdWaitForBtnDown();
    // Display menu 2
  	if (lcdIsBtnDown(leftButton))
  	{
  		choice2=0;
  	}
  	if (lcdIsBtnDown(rightButton))
  	{
  		choice2=2;
  	}
  	if (lcdIsBtnDown(centerButton))
  	{
  		continue;
  	}
	int temp = choice1 + choice2;
	lcdClear();
	char* disp = (temp==0?"BLU Auto":temp==1?"RED Auto":temp==2?"BLU Pole":"RED Pole");
	lcd_printf("\nBack\tOK\tBack");
	clearLCDLine(0);
	displayLCDCenteredString(0,disp);
	lcdWaitForBtnUp();
	lcdWaitForBtnDown();
	if(lcdIsBtnDown(rightButton)||lcdIsBtnDown(leftButton))
		continue;
	Program = temp;
	inMenu=false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous()
{
  // .....................................................................................
  // Insert user code here.
  // .....................................................................................
lcdClear();
char* disp = "\t";
disp += (Program==0?"BLU Auto":Program==1?"RED Auto":Program==2?"BLU Pole":"RED Pole");
disp += "Running";
lcd_printf(disp);
setL(-127);
	setR(-127);
	wait1Msec(750);
	setL(0);
	setR(0);
clawControl(80);
	wait1Msec(500);
	clawControl(-40);

	wait1Msec(1000);
	clawControl(0);
	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	// User control code here, inside the loop
	bool tension = false;
	bool down_7u = false;
	SensorValue[ime_lift] = 0;
	bool lift_running = false;
	while(1)
	{
		nMotorEncoder[claw] = 0;
	SensorValue[ime_lift] = 0;
	bool lift_running = false;
	bool claw_running = false;
	while(1)
	{
		setR(vexRT[Ch2]);
		setL(vexRT[Ch3]);
		if(vexRT[Btn6U] && !vexRT[Btn6D])
		{
			lift(LIFT_SPEED, true);
		}
		else if(vexRT[Btn6D] && !vexRT[Btn6U])
		{
			lift(LIFT_SPEED, false);
		}
		else
		{
			lift(3);
		}

		/*if(vexRT[Btn5U] && !vexRT[Btn5D])
		{
			lift(UP);
		}
		else if(vexRT[Btn5D] && !vexRT[Btn5U])
		{
			lift(DOWN);
		}*/

		if (vexRT[Btn5U] && !vexRT[Btn5D])
		{
			clawControl(-80);
		}
		else if (vexRT[Btn5D] && !vexRT[Btn5U])
		{
			clawControl(80);
		}
		else
		{
			clawControl(-15);
		}

		if (vexRT[Btn7U] && !down_7u)
		{
			clawControl(-15*(tension?1:0));
			tension = !tension;
			down_7u = true;
		}
		else
		{
			down_7u = false;
		}

	}

	}
}
