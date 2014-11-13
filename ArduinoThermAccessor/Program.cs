﻿using System;
using System.IO.Ports;
using System.Net;

namespace ArduinoThermAccessor
{
	public class Program
	{
		private readonly WebClient _webClient = new WebClient();

		public static void Main(string[] args)
		{
			Program prog = new Program();
			prog.TestArduinoGetMethod();
		}

		private void ReadServer()
		{
			string allTemp = _webClient.DownloadString(@"http://192.168.1.177/getTemp/42");

			Console.WriteLine(allTemp);
			Console.Read();
		}

		private void TestArduinoGetMethod()
		{
			int initialFreeMemory = 0;

			for (int i = 0; i < 100; i++)
			{
				string returnedValue = _webClient.UploadString(@"http://192.168.1.177/getTemp/42", "PUT", string.Empty);

				int currentValue;
				int.TryParse(returnedValue, out currentValue);

				if (i == 0)
				{
					initialFreeMemory = currentValue;
				}

				if (currentValue < initialFreeMemory)
				{
					Console.WriteLine("!!! Memory Leak Detected !!!");
					Console.WriteLine("Initial memory: {0}", initialFreeMemory);
					Console.WriteLine("Current memory: {0}", currentValue);
				}
			}

			Console.WriteLine("Test Complete");
			Console.Read();
		}

		private void ReadArduino()
		{
			SerialPort serialPort = new SerialPort("COM3", 9600)
			{
				DtrEnable = true
			};

			serialPort.Open();

			serialPort.DataReceived += SerialPortDataReceived;

			while (true)
			{
			}
		}

		private void SerialPortDataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			string line = ((SerialPort)sender).ReadLine();
			Console.WriteLine(line);
		}
	}
}