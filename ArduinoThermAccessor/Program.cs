using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Linq;
using System.Net;

namespace ArduinoThermAccessor
{
	public class Program
	{
		private readonly WebClient _webClient = new WebClient();

		public static void Main(string[] args)
		{
			Program prog = new Program();
			Dictionary<string, double> testTimes = new Dictionary<string, double>();
			int iterations = 150;

			List<string> methods = new List<string>
			{
				"/TempString/",
				"/Temp/",
				"/FreeMemory/"
			};

			foreach (var method in methods)
			{
				double time = prog.TestArduinoGetMethod(@"http://192.168.1.177" + method, iterations);
				testTimes.Add(method, time);
			}

			Console.WriteLine();
			foreach (var testTime in testTimes)
			{
				Console.WriteLine("Method: {0}\nAverage time: {1}\n", testTime.Key, testTime.Value);
			}
			
			Console.Read();
		}

		private void ReadServer()
		{
			string allTemp = _webClient.DownloadString(@"http://192.168.1.177/getTemp/42");

			Console.WriteLine(allTemp);
			Console.Read();
		}

		private double TestArduinoGetMethod(string url, int iterations)
		{
			Console.WriteLine("Method: {0}", url);

			int initialFreeMemory = 0;

			Stopwatch stopwatch = new Stopwatch();
			List<long> elapsedTimes = new List<long>();
		
			for (int i = 0; i < iterations; i++)
			{
				stopwatch.Start();

				//string returnedValue = _webClient.UploadString(@"http://192.168.1.177/DesiredTemp/70", "PUT", string.Empty);

				string returnedValue = _webClient.DownloadString(@"http://192.168.1.177/TempString/");
				//string returnedFreeMemory = _webClient.DownloadString(@"http://192.168.1.177/FreeMemory/");

				long time = stopwatch.ElapsedMilliseconds;
				stopwatch.Reset();

				elapsedTimes.Add(time);

				int currentValue;
				int.TryParse(returnedValue, out currentValue);

				Console.Write(".");

//				if (i == 0)
//				{
//					initialFreeMemory = currentValue;
//				}
//
//				if (currentValue < initialFreeMemory)
//				{
//					Console.WriteLine("!!! Memory Leak Detected !!!");
//					Console.WriteLine("Initial memory: {0}", initialFreeMemory);
//					Console.WriteLine("Current memory: {0}", currentValue);
//				}
			}
			Console.WriteLine();

			double averageTime = elapsedTimes.Average();
			return averageTime;
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