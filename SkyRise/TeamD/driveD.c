#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           left1,         tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_1, 1000)
#pragma config(Motor,  port2,           right1,        tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port3,           right2,        tmotorVex393HighSpeed, openLoop, encoder, encoderPort, I2C_2, 1000)
#pragma config(Motor,  port4,           lift1,         tmotorVex393, openLoop, encoder, encoderPort, I2C_3, 1000)
#pragma config(Motor,  port5,           lift2,         tmotorVex393, openLoop)
#pragma config(Motor,  port6,           lift3,         tmotorVex393, openLoop)
#pragma config(Motor,  port7,           lift4,         tmotorVex393, openLoop)
#pragma config(Motor,  port8,           lift5,         tmotorVex393, openLoop)
#pragma config(Motor,  port10,          left2,         tmotorVex393HighSpeed, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define LIFT_SPEED 98
//Set the right motors to speed speed
void setR(int speed)
{
	//Set the right motors to speed speed
	motor[right1] = motor[right2] = speed;
}

//Set the left motors to speed speed
void setL(int speed)
{
	//Set the left motors to speed speed
	motor[left1] = motor[left2] = speed;
}

void lift(int speed, bool up = true)
{
		motor[lift1] = motor[lift2] = motor[lift3] = motor[lift4] = motor[lift5] = speed * (up?1:-1);
}

task main()
{

	while(1)
	{
		//Set the right motor to the speed of the channel 2 input
		setR(vexRT[Ch2]);
		//Set the left motor to the speed of the channel 3 input
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

	}

}
