int batstats()
{
string mainBattery, backupBattery;
lcdWaitForBtnUp();
while(nLCDButtons==0)                                       // An infinite loop to keep the program running until you terminate it
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
return nLCDButtons;
}

void diag()
{
while(1)
{
	int screen=0;
	int returnCode = 0;
	switch (screen)
	{
		case 0:
			returnCode=batstats();
	}
	lcdClear();
	if(returnCode==2)
		return;
}
}
