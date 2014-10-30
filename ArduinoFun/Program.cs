using System;
using System.IO.Ports;
using System.Net;

namespace ArduinoFun
{
	public class Program
	{
		public static void Main(string[] args)
		{
			Program prog = new Program();
			//prog.ReadArduino();
			prog.ReadServer();
		}

		private void ReadServer()
		{
			System.Net.WebClient client = new WebClient();
			string allTemp = client.DownloadString(@"http://192.168.1.177/getTemp/");

			Console.WriteLine(allTemp);
			Console.ReadKey();
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

		private void SerialPortDataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
		{
			string line = ((SerialPort)sender).ReadLine();
			Console.WriteLine(line);
		}
	}
}