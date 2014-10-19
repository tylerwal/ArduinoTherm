using System;
using System.IO.Ports;
using System.Threading;

namespace ArduinoFun
{
	class Program
	{
		private static SerialPort _serialPort;

		private static bool _continue;

		public static void Main()
		{
			StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;
			//Thread readThread = new Thread(Read);

			// Create a new SerialPort object with default settings.
			_serialPort = new SerialPort
			{
				BaudRate = 9600, // 9600 default
//				DataBits = 8, // 8 default
//				DiscardNull = false, // false default
				DtrEnable = true, // false default
//				Handshake = Handshake.None, // None default
//				Parity = Parity.None, // None default
//				ParityReplace = 63,
				PortName = "COM3",  // COM1 default
//				ReadBufferSize = 4096, // 4096 default
//				ReadTimeout = -1, 
//				ReceivedBytesThreshold = 1,
				RtsEnable = true,
//				StopBits = StopBits.One,
//				WriteBufferSize = 2048,
//				WriteTimeout = -1
			};

			_serialPort.DataReceived += _serialPort_DataReceived;

			// Allow the user to set the appropriate properties.
			// Set the read/write timeouts

			_serialPort.Open();
			_continue = true;
			//readThread.Start();

			//Console.Write("Name: ");
			//string name = Console.ReadLine();

			//Console.WriteLine("Type QUIT to exit");

			//_serialPort.Write(Environment.NewLine);

			while (_continue)
			{
//				string message = Console.ReadLine();
//
//				if (stringComparer.Equals("quit", message))
//				{
//					_continue = false;
//				}
//				else
//				{
//					_serialPort.WriteLine(
//					    String.Format("<{0}>: {1}", name, message)
//					    );
//				}
			}

//			readThread.Join();
			_serialPort.Close();
		}

		static void _serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			SerialPort sp = (SerialPort)sender;
			string indata = sp.ReadExisting();
			//Console.WriteLine("Data Received:");
			Console.WriteLine(indata);
		}

		public static void Read()
		{
			while (_continue)
			{
				try
				{
					string message = _serialPort.ReadLine();
					Console.WriteLine(message);
				}
				catch (TimeoutException) { }
			}
		}
	}
}
