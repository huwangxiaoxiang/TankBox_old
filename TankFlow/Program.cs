using System;
using System.Threading;
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
                Log.AddLog("TankFlow启动完成");
                //Test m = new Test();
                Application.Run(f);
            }
            else
            {
                //MessageBox.Show("程序已经在运行！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                Application.Exit();
            }
        }
    }

    public class ReceivedData
    {
        public uint dataKey;

        public string message;
        public ReceivedData(string received_message)
        {
            try
            {
                int pos = received_message.IndexOf(' ');
                string index = received_message.Substring(0, pos);
                this.dataKey = uint.Parse(index);
                this.message = received_message.Substring(pos + 1, received_message.Length - pos - 1);
            }
            catch (Exception e)
            {
                this.dataKey = 0;
                this.message = "";
                Log.AddLog("ReceivedData字符串解析失败!" + e.Message);
            }
        }
    }

    public class BattleResult
    {
        public string user_id;
        public string tank;
        public string victory;
        public string mode;
        public bool valid = false;
        public string honor = "0";
        public string shoot = "0";
        public string hits = "0";
        public string penertration = "0";
        public string damage_caused = "0";
        public string hits_received = "0";
        public string damage_received = "0";
        public string enermy_detected = "0";

        public BattleResult(string data)
        {
            string[] temp = data.Split(',');
            if (temp.Length >= 12)
            {
                user_id = temp[0];
                tank = temp[1];
                mode = temp[3];
                honor = temp[4];
                shoot = temp[5];
                hits = temp[6];
                penertration = temp[7];
                damage_caused = temp[8];
                hits_received = temp[9];
                damage_received = temp[10];
                enermy_detected = temp[11];

                if (temp[2] == "0")
                {
                    victory = "1";
                    valid = true;
                }
                else if (temp[2] == "1" || temp[2] == "2")
                {
                    victory = "0";
                    valid = true;
                }
                else
                {
                    valid = false;
                }
            }
        }
    }
}
