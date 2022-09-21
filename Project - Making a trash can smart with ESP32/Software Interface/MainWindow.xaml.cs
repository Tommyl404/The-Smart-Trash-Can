using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.IO.Ports;
using System.Windows.Shapes;
using System.Windows.Forms;



namespace Hackaton2022
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    
    public partial class MainWindow : Window
    {
        public delegate void d1(string indata); // the info tube
        SolidColorBrush red = new SolidColorBrush();
        SolidColorBrush green = new SolidColorBrush();
        SolidColorBrush lightRed = new SolidColorBrush();
        SerialPort serialPort1 = new SerialPort("COM7");
        
        public MainWindow()
        {
            
            InitializeComponent();
            
            //serialPort1.PortName = "COM7";
            serialPort1.BaudRate = 115200;
            serialPort1.Open();

            red.Color = Color.FromArgb(244, 244, 0, 0);
            green.Color = Color.FromRgb(10, 163, 0);
            lightRed.Color = Color.FromRgb(160, 165, 37);

            setTitleStart();
            lvl1.Fill = green;
            lvl2.Fill = green;
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(serialPort1_DataReceived);





        }

        public void setTitleStart()
        {
            Text1.TextAlignment = TextAlignment.Center;
            Text1.Text = "Hackaton 2022 - Team 2";
        }
        

        public void SetOneLvlGreen(int lvl)
        {
            
            if (lvl == 0) // r g
            {
                lvl2.Fill = red;
                lvl1.Fill = green;
            }
            else if (lvl == 1) // r r
            {
                lvl1.Fill = red;
                lvl2.Fill = red;
            }
            else //g g
            {
                lvl1.Fill = green;
                lvl2.Fill = green;
            }
        }
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string indata = serialPort1.ReadLine();
            d1 writeIt = new d1(Write2Form);
            writeIt.Invoke(indata);
        }
        public void Write2Form(string indata)
        {
            //handle data sent from the arduino.
            char numDataTop = indata[1];
            char numDataBottom = indata[3];



            if (numDataTop =='1' && numDataBottom == '1') //r r
            {
                //this.Dispatcher.BeginInvoke(SetOneLvlGreen(1));
                this.Dispatcher.Invoke(() =>
                {
                    lvl1.Fill = red;
                    lvl2.Fill = red;
                    prog.Value = 70;
                    prog.Foreground = red;
                });
                

            }
            else if (numDataTop == '0' && numDataBottom == '1') // r g
            {
                //SetOneLvlGreen(0);
                this.Dispatcher.Invoke(() =>
                {
                    lvl1.Fill = red;
                    lvl2.Fill = green;
                    prog.Value = 60;
                    prog.Foreground = lightRed;
                });
                
            }
            else //g g
            {
                //SetOneLvlGreen(7);
                this.Dispatcher.Invoke(() =>
                {
                    lvl2.Fill = green;
                    lvl1.Fill = green;
                    prog.Value = 50;
                    prog.Foreground = green;
                });
            }
            





        }
    }
}
