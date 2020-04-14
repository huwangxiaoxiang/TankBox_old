using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace TankFlow
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        private static Mutex mutex;
        [STAThread]
        static void Main()
        {
            mutex = new Mutex(true, "TankFlow.exe");
            if (mutex.WaitOne(0, false))
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                TankFlow f = new TankFlow(); //创建一个新窗体
                f.FormBorderStyle = FormBorderStyle.None; //设置窗体无边框
                f.ShowInTaskbar = false;
                f.BackColor = System.Drawing.Color.Black;
                f.TransparencyKey = f.BackColor; //让窗体透明 

                Application.Run(f);
            }
            else
            {
                //MessageBox.Show("程序已经在运行！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                Application.Exit();
            }
        }
    }

    class ReceivedData
    {
        public uint dataKey;

        public string message;
        public ReceivedData(string received_message)
        {
            try
            {
                string[] temp = received_message.Split(' ');
                this.dataKey = uint.Parse(temp[0]);
                this.message = temp[1];
            }catch(Exception e)
            {
                this.dataKey = 0;
                this.message = "";
            }
        }
    }

    class BattleResult
    {
        public string user_id;
        public string tank;
        public string victory;
        public string mode;
        public bool valid = false;
       
        public BattleResult(string data)
        {
            string[] temp = data.Split(',');
            if (temp.Length >=4)
            {
                user_id = temp[0];
                tank = temp[1];
                mode = temp[3];
                if (temp[2] == "0")
                {
                    victory = "true";
                    valid = true;
                }
                else if (temp[2] == "1" || temp[2] == "2")
                {
                    victory = "false";
                    valid = true;
                }else
                {
                    valid = false;
                }
                
            }
        }
    }
}
