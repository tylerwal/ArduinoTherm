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
			Dictionary<string, BenchmarkData> benchmarkDatas = new Dictionary<string, BenchmarkData>();
			int iterations = 25;

			List<string> methods = new List<string>
			{
				"/Temperature/",
				"/GoalTemperature/",
				"/Status/",
				"Humidity",
				"/FreeMemory/",
				"/ResourceDirectory/",
				"/else/"
			};

			foreach (string method in methods)
			{
				benchmarkDatas.Add(method, prog.TestArduinoGetMethod(@"http://192.168.1.177" + method, iterations));
			}

			Console.WriteLine();

			foreach (KeyValuePair<string, BenchmarkData> benchmarkData in benchmarkDatas)
			{
				Console.WriteLine("Method: {0}\tAverage Test Time: {1}", benchmarkData.Key, benchmarkData.Value.TestAverageTime);
				Console.WriteLine("Pre Memory: {0}\tPost Memory: {1}\n", benchmarkData.Value.BeforeFreeMemory, benchmarkData.Value.AfterFreeMemory);
			}
			
			Console.Read();
		}

		private BenchmarkData TestArduinoGetMethod(string url, int iterations)
		{
			Console.Write("Method: {0}", url);

			Stopwatch stopwatch = new Stopwatch();
			List<long> elapsedTimes = new List<long>();

			string beforeFreeMemory = _webClient.DownloadString(@"http://192.168.1.177/FreeMemory/");

			for (int i = 0; i < iterations; i++)
			{
				stopwatch.Start();

				string returnedValue = _webClient.DownloadString(@"http://192.168.1.177/TempString/");

				long time = stopwatch.ElapsedMilliseconds;
				stopwatch.Reset();

				elapsedTimes.Add(time);

				int currentValue;
				int.TryParse(returnedValue, out currentValue);

				Console.Write(".");
			}
			Console.WriteLine();

			string afterFreeMemory = _webClient.DownloadString(@"http://192.168.1.177/FreeMemory/");

			BenchmarkData benchmarkData = new BenchmarkData
			{
				Url = url,
				TestAverageTime = elapsedTimes.Average(),
				BeforeFreeMemory = int.Parse(beforeFreeMemory),
				AfterFreeMemory = int.Parse(afterFreeMemory)
			};

			return benchmarkData;
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