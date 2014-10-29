using System;
using System.IO.Ports;

namespace ArduinoFun
{
	public class Program
	{
		public static void Main(string[] args)
		{
			Program prog = new Program();
			prog.ReadArduino();
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