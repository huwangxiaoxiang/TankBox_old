using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace TankFlow
{
    public partial class TankFlow : Form
    {
        const int WM_COPYDATA = 0x004A;
        const int EVENT = 30000;
        const int BATTLE_START = 36278;
        const int BATTLE_END = 36279;
        const int SPOTED = 36281;
        const int UNSPOTED = 36280;
        const string GameClass = "UnityWndClass";
        const string GameName = "Tank Battle";
        PCManager manager;
        bool is_lock = true;
        List<Damage> damage_list=new List<Damage>();
        Damage[] temp_damage = new Damage[6];

        public TankFlow()
        {
            InitializeComponent();
            initWindow();
            this.postimer.Stop();
            this.StartPosition = FormStartPosition.Manual;
            followPosition();
            manager = new PCManager(100);
          
        }

        //初始化窗口数据
        private void initWindow()
        {
            label1.Text = "";
            label2.Text = "";
            label3.Text = "";
            label4.Text = "";
            label5.Text = "";
            label6.Text = "";
            for (int i = 0; i < 6; i++)
                temp_damage[i] = new Damage("");
            drawLabels();
            this.spot_state.Visible = false;
        }

        //添加字符串至窗口
        public void PushString(Damage s)
        {
            for (int i = 5; i > 0; i--)
                temp_damage[i] = temp_damage[i - 1];
            temp_damage[0] = s;
            drawLabels();
        }

        /// <summary>
        /// 绘制伤害面板上的所有伤害数据
        /// </summary>
        /// <param name="m"></param>
        void drawLabels()
        {
            Point init = new Point(1, 36);
            int dy = 30;
            int width = 350, height = 25;
            Graphics g = this.CreateGraphics();
            Brush back_brush = new SolidBrush(this.BackColor);
            for (int i = 0; i < 6; i++)
            {
                Damage temp = temp_damage[i];
                if (!temp.valid)
                    return;
                Rectangle rect = new Rectangle(init.X + 1, init.Y + i * dy, width, height);
                g.FillRectangle(back_brush, rect);
                Color font_color,border_color;
                border_color = Color.FromArgb(10, 0, 0);
                if (temp_damage[i].friend)
                    font_color = Color.FromArgb(80, 215, 120);
                else
                    font_color = Color.FromArgb(239, 32, 0);
                GDIDraw.Paint_Text(temp.fillspace(10,temp.source),rect,font_color,border_color,g, 14f);
                rect.Offset(115, 0);
                GDIDraw.Paint_Text("->", rect, font_color, border_color, g, 14f);
                rect.Offset(30, 0);
                GDIDraw.Paint_Text(temp.fillspace(10, temp.victim), rect, font_color, border_color, g, 14f);
                rect.Offset(115, 0);
                GDIDraw.Paint_Text(temp.GetDamageType(), rect, font_color, border_color, g, 14f);
               
            }
            g.Dispose();
            back_brush.Dispose();
        }

        //消息处理函数
        /**/
        protected override void DefWndProc(ref System.Windows.Forms.Message m)
        {
            is_lock = false;
            if (m.Msg == 32770)
                is_lock = false;
            if (is_lock)
            {
                base.DefWndProc(ref m);
                return;
            }
            switch (m.Msg)
             {
                 case WM_COPYDATA:
                    string s1=getCopyMessage(ref m);
                    Console.WriteLine("收到字符串：" + s1);
                    Damage dama = new Damage(s1);
                    if (dama.valid)
                    {
                        damage_list.Add(dama);
                        PushString(dama);
                    }
                    this.postimer.Start();
                    break;
                case EVENT:
                    OnHandleEvent((int)m.LParam);
                    break;
                default:
                     base.DefWndProc(ref m);
                    break;
             }
             
        }
        
        private void uploadData()
        {
           foreach(Damage data in damage_list)
            {
                if (data.valid)
                {
                    manager.AddProductPlus(data);
                }
            }
            damage_list.Clear();
            Thread consumer = new Thread(() =>
            {
                int times = 0;
                int fail = 0;
                Log.AddLog("消费者线程开始");
                Console.WriteLine("消费者线程开始：" +DateTime.Now.ToString());
                while (true)
                {
                    Damage m = (Damage)manager.GetProduct(10000);
                    if (m != null)
                    {
                        if (!Connector.add_damage(m))
                            fail++;
                        times++;
                        m = null;
                    }
                    else
                        break;
                }
                Console.WriteLine("消费者线程结束，上传数据：" + times.ToString()+" 结束时间:"+DateTime.Now.ToString());
                Log.AddLog("消费者线程结束，上传数据：" + times.ToString()+" 失败数："+fail.ToString());
            });
            consumer.Start();
        }

        private void OnHandleEvent(int flag)
        {
            switch (flag)
            {
                case BATTLE_START:
                    initWindow();
                    this.Show();
                    this.postimer.Start();
                    Console.WriteLine("战斗开始");
                    break;
                case BATTLE_END:
                    this.postimer.Stop();
                    this.Hide();
                    Console.WriteLine("战斗结束");
                    uploadData();
                    break;
                case SPOTED:
                    this.spot_state.Visible = true;
                    this.Show();
                    this.postimer.Start();
                    break;
                case UNSPOTED:
                    this.spot_state.Visible = false;
                    break;
                default:
                    Console.WriteLine("未知消息类型："+flag);
                    break;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct COPYDATASTRUCT
        {
            public IntPtr dwData;
            public int cbData;
            public IntPtr lpData;
        }

        private void Button1_Click(object sender, EventArgs e)
        {

        }

        //获取传递过来的字符串
        private string getCopyMessage(ref System.Windows.Forms.Message m)
        {
            COPYDATASTRUCT mystr = new COPYDATASTRUCT();
            Type mytype = mystr.GetType();
            mystr = (COPYDATASTRUCT)m.GetLParam(mytype);
            int flag = (int)mystr.dwData;
            byte[] bt = new byte[mystr.cbData];
            Marshal.Copy(mystr.lpData, bt, 0, bt.Length);
            string m2 = System.Text.Encoding.Unicode.GetString(bt);
            Console.WriteLine(m2);
            return m2;
        }

        //定时器执行动作
        private void Postimer_Tick(object sender, EventArgs e)
        {
            followPosition();
        }

        //设置窗口跟随游戏窗口移动
        private void followPosition()
        {
            IntPtr hwnd = BaseAPI.FindWindow(GameClass, GameName);
            IntPtr forgeWindow = BaseAPI.GetForegroundWindow();
            if (hwnd != forgeWindow)
            {
                this.Location = new System.Drawing.Point(-300,-300);
                return;
            }
          this.TopMost = true;
          BaseAPI.RECT client;
          BaseAPI.GetWindowRect(hwnd, out client);
          this.Location = new System.Drawing.Point((int)(client.Left + 165), (int)(client.Bottom - 220));
        }

        private void TankFlow_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }
    }
}
