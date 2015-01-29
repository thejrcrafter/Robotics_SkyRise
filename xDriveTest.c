#pragma config(Sensor, in1,    kGyroPort,    sensorGyro)
#pragma config(Sensor, dgtl1,  piston,         sensorDigitalOut)
#pragma config(Motor,  port6,           leftGear,      tmotorVex393, openLoop)
#pragma config(Motor,  port7,           rightGear,     tmotorVex393, openLoop)
#pragma config(Motor,  port8,           eleLeft,       tmotorVex393, openLoop)
#pragma config(Motor,  port9,           eleRight,      tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


const bool doUseGyro = false; // enable gyroscopic sensor
bool isGyroCalibrated = false;
float gyroOffset = 90.0; // degrees CCW from +x axis robot faces at gyro=0

//DPI
bool isSlowActive = false; //Is slow mode active
float multiplierSlow = 0.3;
byte debounceSlow = 0; //Debouncer for it (If 0 can switch)
bool isAutoRunning = false;

//XDrive Settings
const TVexJoysticks kChTranslate = Ch1; // Translation Control
const TVexJoysticks kChYaw = Ch4; //Rotating control
const TVexJoysticks kChFrwrd = Ch2; //Forward Movement
const short kDelay = 25; // milliseconds for loop delay
const ubyte kNumWheels = 4; // number of drive wheels
const ubyte kNumMotorsPerWheel = 2; // max number of motors per wheel
const tMotor kNone = -1; // used for indicating the lack of an additional motor
const tMotor kMotorPort[kNumWheels][kNumMotorsPerWheel] = { // drive motor ports/names
    {port2, kNone}, // front-left
    {port3, kNone}, // front-right
    {port4, kNone}, // back-left
    {port5, kNone}  // back-right
};

void calibrateGyro()
{
	if (doUseGyro && !isGyroCalibrated) {
    writeDebugStreamLine("Gyro Calibrated");
    isGyroCalibrated = true;

    // completely clear out any previous sensor readings by setting the port to "sensorNone"
    SensorType[kGyroPort] = sensorNone;
    wait1Msec(1000);
    SensorType[kGyroPort] = sensorGyro; // reconfigure as sensorGyro
    wait1Msec(2000); // wait for calibration: ROBOT MUST STAY STILL

    SensorScale[kGyroPort] = 138; // adjust SensorScale to correct the scaling for your gyro
    SensorFullCount[kGyroPort] = 3599; // fix rollover to be "...3598, 3599, 0, 1..."
	}
}

void setXDrive(word sideways, word rotate, word forward) {
	//sideways = Left Joystick 4
	//rotate = Right Joystick 1
	//forward = Right Joystick 2
	//127^2 = 16129
  float gyro,radius,theta,wheelSpeed[kNumWheels],topSpeed;

  	//gyro = gyroOffset + (doUseGyro ? SensorValue[kGyroPort]/10.0 : 0.0); // if using gyro, scale its value to degrees

    // ==== convert joystick values to polar ====
    radius = sqrt(pow(sideways,2) + pow(forward,2)); // forward = sqrt(sideways^2 + rotate^2)
    theta = atan2(forward,sideways)*180.0/PI; // t = arctan(rotate/sideways) [converted from radians to degrees]

    //theta -= gyro; // adjust for gyro angle

    //start.getX() + len * Math.cos(Math.toRadians(dir))
    //start.getY() + len * Math.sin(Math.toRadians(dir))

    // ==== calculate opposite-side speeds ====
    //float a,b;
    //a = (cosDegrees(theta + 90.0) + sinDegrees(theta + 90.0))*radius; // front-left and back-right
    //b = (cosDegrees(theta) + sinDegrees(theta))*radius; // front-right and back-left

    // ==== set speeds, including rotation ====
    //wheelSpeed[0] = a + forward; // front-left
    //wheelSpeed[1] = b - forward; // front-right
    //wheelSpeed[2] = b + forward; // back-left
    //wheelSpeed[3] = a - forward; // back-right


    float x,y;
    x = radius * cosDegrees(theta);
    y = radius * sinDegrees(theta);

    // ==== set speeds, including rotation ====
    wheelSpeed[0] = x + y + rotate; // front-left
    wheelSpeed[1] = x - y - rotate; // front-right
    wheelSpeed[2] = x - y + rotate; // back-left
    wheelSpeed[3] = x + y - rotate; // back-right


    // ==== normalize speeds ====
    topSpeed = 0.0;
    for(ubyte i=0; i<kNumWheels; i++)
      if(abs(wheelSpeed[i]) > topSpeed)
        topSpeed = abs(wheelSpeed[i]); // find highest desired speed
    if(topSpeed > 127.0)
      for(ubyte i=0; i<kNumWheels; i++)
        wheelSpeed[i] /= topSpeed/127.0; // downscale all speeds so none are above 127

    // ==== update motor powers ====
    for(ubyte i=0; i<kNumWheels; i++) // cycle through all wheels
      for(ubyte j=0; j<kNumMotorsPerWheel; j++) // cycle through all motors for each wheel
        if(kMotorPort[i][j] != kNone) // check existence of motor
          motor[kMotorPort[i][j]] = (word)wheelSpeed[i]; // update motor power
}


task userDriveHolo() {
  word x,y,r;
	int btnDown = 5;
  while(true) {
    // ==== collect joystick & sensor values ====
    x = vexRT[kChYaw]* (isSlowActive ? multiplierSlow : 1); // x component
    y = vexRT[kChFrwrd]* (isSlowActive ? multiplierSlow : 1); // y component
    r = -vexRT[kChTranslate]* (isSlowActive ? multiplierSlow : 1); //Rotation componenet

    setXDrive(x, y, r);

    if (!isAutoRunning && vexRT(Btn7U)) {
			isAutoRunning = true;
			StartTask(autonomous);
    }

    //Main Lift
    if(vexRT[Btn6U] && !vexRT[Btn6D])
    {
    	motor[eleLeft] = 96 * (isSlowActive ? multiplierSlow : 1);
    	motor[eleRight] = 96 * (isSlowActive ? multiplierSlow : 1);
    }
    else if(!vexRT[Btn6U] && vexRT[Btn6D])
    {
    	motor[eleLeft] = -96 *(isSlowActive ? multiplierSlow : 1);
    	motor[eleRight] = -96 *(isSlowActive ? multiplierSlow : 1);
    }
    else
    {
    	motor[eleLeft] = 15;
    	motor[eleRight] = 15;
    }

    //Arm Lift
    if(vexRT[Btn5D] && !vexRT[Btn5U])
    {
    	motor[rightGear] = 96*(isSlowActive ? multiplierSlow : 1);
    	motor[leftGear] = 96*(isSlowActive ? multiplierSlow : 1);
    }
    else if(!vexRT[Btn5D] && vexRT[Btn5U])
    {
    	motor[rightGear] = -96 *(isSlowActive ? multiplierSlow : 1);
    	motor[leftGear] = -96 *(isSlowActive ? multiplierSlow : 1);
    }
    else
    {
    	//Default Pressure
    	motor[rightGear] = -15;
    	motor[leftGear] = -15;
    }

    //Slow mode
    if (vexRT[Btn8L] && debounceSlow == 0) {
    	debounceSlow = 5;
    	isSlowActive = !isSlowActive;
    }
    else if (debounceSlow > 0) {
  			debounceSlow--;
  	}


    //Piston Claw
    if(!vexRT[Btn8D] && btnDown==0)
    {
    	btnDown=5;
    	SensorValue[piston]=!SensorValue[piston];
    }
    else if(vexRT[Btn8D] && btnDown>0)
    {
    	btnDown--;
    }

    wait1Msec(kDelay);
  }
}


void pre_auton()
{
  calibrateGyro();
  bStopTasksBetweenModes = true;
}

task autonomous()
{
    	motor[rightGear] = -96;
    	motor[leftGear] = -96;
    	wait10Msec(100);
    	motor[rightGear] = -15;
    	motor[leftGear] = -15;
    	wait10Msec(10);
    	//P1 = Rotate
	//P2 = Forward Motion
  //P3 = Sideways Motion
   setXDrive(0, -127, 0);
   wait10Msec(100);
   setXDrive(0, 127, 0);
   wait10Msec(100);
   setXDrive(0, 0, 0);
   isAutoRunning = false;
}



task usercontrol()
{
  calibrateGyro();
  StartTask(userDriveHolo);

  while(true)
    wait1Msec(100);
}
/*
task usercontrol()
{
	clearTimer(T1);
	while (true)
	{
		rotValueSquares = SensorValue[rotSensor]/rotToSquaresRatio;

		//pneumatic claw
		if(vexRT[Btn8D] == 1 && time1[T1] > 500)
		{
			if(SensorValue[claw] == 1)
				SensorValue[claw] = 0;
			else
				SensorValue[claw] = 1;
			clearTimer(T1);
		}
		else {}
		//not claw pneumatic
		//not claw is actually the claw, claw is the not claw
		//changed the sensors to the correct slots - jason
		//pneumatic for not claw just releases the gas or closes the gas, no lift action

		if(vexRT[Btn7D] == 1 && time1[T1] > 500)
		{
			if(SensorValue[notClaw] == 1)
				SensorValue[notClaw] = 0;
			else
				SensorValue[notClaw] = 1;
			clearTimer(T1);
		}
		else {}
		//-----------------
		if(vexRT[Btn7R] != 1 && vexRT[Btn7L] != 1)
		{
			motor[frontRight] = -vexRT[Ch2];
			motor[backRight] =  -vexRT[Ch2];
			motor[frontLeft] = vexRT[Ch3];
			motor[backLeft] =  vexRT[Ch3];
		}
		else if(vexRT[Btn7R] == 1)
		{
			strafeRight(90);
		}
		else if(vexRT[Btn7L] == 1)
		{
			strafeLeft(90);
		}
		//	if(vexRT[Btn8U] == 1)
		//	{
		//		drive(90);
		//	}
		//		else if(vexRT[Btn8D] == 1)
		//		{
		//			drive(-90);
		//		}
		//		if(vexRT[Btn8R] == 1)
		//		{
		//	driveLeftSide(110);
		/*		driveRightSide(110);
		}
		else if(vexRT[Btn8L] == 1)
		{
		driveLeftSide(-110);
		driveRightSide(-110);
		}
		else
		{}
		*/	//----------------------------------------
/*
		if(vexRT[Btn6D] == 1)
		{
			motor[liftLeftBottom] = -127;
			motor[liftRightMid] = 127;
			motor[liftRightTop] = 127;
			motor[liftRightBottom] = 127;
			motor[liftLeftTop] = -127;
		}
		else if(vexRT[Btn6U] == 1)
		{
			motor[liftLeftBottom] = 127;
			motor[liftRightMid] = -127;
			motor[liftRightTop] = -127;
			motor[liftRightBottom] = -127;
			motor[liftLeftTop] = 127;
		}
		else
		{
			motor[liftLeftBottom] = 0;
			motor[liftRightMid] = 0;
			motor[liftRightTop] = 0;
			motor[liftRightBottom] = 0;
			motor[liftLeftTop] = 0;
		}
		//Convayer control ---------------------
		if(vexRT[Btn5U] == 1)
		{
			motor[con] = 127;
		}
		else if(vexRT[Btn5D] == 1)
		{
			motor[con] = -127;
		}
		else
		{
			motor[con] = 0;

		}

	}
}*/
