using System;
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Threading;
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
        const int DISABLE_PANEL = 36282;
        const int ENALBE_PANEL = 36283;
        const int STARTRECOGNIZE = 36284;
        const int STOPRECOGNIZE = 36285;


        const string GameClass = "UnityWndClass";
        const string GameName = "Tank Battle";
        bool is_lock = true;
        bool show_damage_panel = true;
        List<Damage> damage_list=new List<Damage>();
        Damage[] temp_damage = new Damage[6];

        private int user_id = -1;

        public SocketClient python_client;
        public SocketClient tank_client;

        private float rec_alpha=0;
        private string rec_text = "正在识别...";

        public TankFlow()
        {
            InitializeComponent();
            initWindow();
            this.postimer.Stop();
            this.StartPosition = FormStartPosition.Manual;
            //followPosition();
            this.user_id = -1;
            
        }

        public void HandleRecognizeResult(string result,int type)
        {
            if (type == 2)
            {
                this.recognizeTimer.Stop();
                this.clearRec();
            }
        }

        private void setUser(int user_id)
        {
            this.user_id = user_id;
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
                    continue;
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

        private void drawRec(int alpha)
        {
            Rectangle rect = new Rectangle(95, 0, 36, 39);
            Graphics g = this.CreateGraphics();
            Brush back_brush = new SolidBrush(this.BackColor);
            g.FillRectangle(back_brush, rect);

            Color border_color = Color.FromArgb(10, 0, 0);
            Color font_color = Color.FromArgb(alpha, 80, 215, 120);
            GDIDraw.Paint_Text("●", rect, font_color, border_color, g, 25f);

            g.Dispose();
            back_brush.Dispose();
            this.drawRecText();

        }

        public void drawRecText()
        {
            Graphics g = this.CreateGraphics();
            Brush back_brush = new SolidBrush(this.BackColor);
            Color border_color = Color.FromArgb(10, 0, 0);
            Rectangle rect2 = new Rectangle(120, 10, 400, 19);
            g.FillRectangle(back_brush, rect2);
            Color font_color2 = Color.FromArgb(80, 215, 120);
            GDIDraw.Paint_Text(this.rec_text, rect2, font_color2, border_color, g, 14f);
            g.Dispose();
            back_brush.Dispose();
        }

        public void clearRec()
        {
            Graphics g = this.CreateGraphics();
            Brush back_brush = new SolidBrush(this.BackColor);
            Rectangle rect2 = new Rectangle(0, 0, 500, 40);
            g.FillRectangle(back_brush, rect2);
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
                    Log.AddLog("收到字符串:"+s1);
                    ReceivedData data = new ReceivedData(s1);
                    this.HandleCopyData(data);
                    
                    break;
                case EVENT:
                    OnHandleEvent((int)m.LParam);
                    break;
                default:
                     base.DefWndProc(ref m);
                    break;
             }
             
        }

        private void HandleCopyData(ReceivedData data)
        {
            switch(data.dataKey)
            {
                case 1://坦克造成伤害
                    Damage dama = new Damage(data.message);
                    if (dama.valid)
                    {
                        damage_list.Add(dama);
                        if (this.show_damage_panel)
                            PushString(dama);
                    }
                    break;
                case 2://战斗结果
                        if (this.user_id != -1)
                        {
                            BattleResult result = new BattleResult(this.user_id.ToString() + "," + data.message);
                            if (result.valid)
                            {
                                Thread th = new Thread(this.uploadBattleResult);
                                th.Start(result);
                            }
                        }
                        else
                        {
                            Log.AddLog("user_id=-1的时候");
                        }
                    break;
                case 3://设置user_id
                    bool resu = int.TryParse(data.message, out this.user_id);
                    if (!resu)
                        this.user_id = -1;
                    else
                    {
                        Log.AddLog("设置ID成功 " + this.user_id.ToString());
                    }
                    break;
            }
        }

        private void uploadBattleResult(object data)
        {
            BattleResult result = (BattleResult)data;
            Log.AddLog("开始上传战斗结果");
            bool up_result = HttpConnect.UploadBattleResult(result);
            if (up_result) Log.AddLog("上传战斗结果成功 " + result.tank);
            else Log.AddLog("上传战斗结果失败 " + result.tank);
        }

        private void uploadData()
        {
            List<Damage> valid_damage = new List<Damage>();
           foreach(Damage data in damage_list)
            {
                if (data.valid&&data.grade>5)
                {
                    valid_damage.Add(data);
                }
            }
            damage_list.Clear();
            UploadManager manager = new UploadManager(valid_damage);
            manager.Upload();//开始上传数据
        }

        private void OnHandleEvent(int flag)
        {
            switch (flag)
            {
                case BATTLE_START:
                    initWindow();
                    this.Show();
                    this.postimer.Start();
                    Log.AddLog("战斗开始");
                    new Thread(() =>
                    {
                        this.python_client = new SocketClient(34567,"127.0.0.1", new ClientReceiver(this));
                        this.tank_client = new SocketClient(3457,"127.0.0.1", new ClientReceiver(this));
                        this.python_client.ConnectServer();
                        this.tank_client.ConnectServer();
                        this.python_client.SendData("start_record");
                    }).Start();
                    break;
                case BATTLE_END:
                    this.postimer.Stop();
                    this.Hide();
                    Log.AddLog("战斗结束");
                    uploadData();
                    initWindow();
                    this.clearRec();
                    this.recognizeTimer.Stop();
                    new Thread(() =>
                    {
                        if (python_client != null)
                        {
                            python_client.SendData("stop_record");
                            this.python_client.DisConnect();
                        }
                        if(this.tank_client!=null)
                            this.tank_client.DisConnect();
                    }).Start();
                    break;
                case SPOTED:
                    this.spot_state.Visible = true;
                    this.Show();
                    this.postimer.Start();
                    break;
                case UNSPOTED:
                    this.spot_state.Visible = false;
                    break;
                case DISABLE_PANEL:
                    this.show_damage_panel = false;
                    break;
                case ENALBE_PANEL:
                    this.show_damage_panel = true;
                    break;
                case STARTRECOGNIZE:
                    Log.AddLog("开始语音识别...");
                    if (python_client != null)
                    {
                        python_client.SendData("start_recognize");
                        this.rec_text = "正在识别...";
                        this.recognizeTimer.Start();
                    }
                    break;
                case STOPRECOGNIZE:
                    Log.AddLog("结束语音识别...");
                    if (python_client != null)
                    {
                        python_client.SendData("stop_recognize");
                    }
                    this.recognizeTimer.Stop();
                    this.clearRec();
                    break;
                default:
                    Log.AddLog("未知消息类型："+flag);
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

        private void RecognizeTimer_Tick(object sender, EventArgs e)
        {
            int alp = 0;
            if (this.rec_alpha > 3.1415f)
            {
                this.rec_alpha -= 3.1415f;
            }
            alp = (int)(255 * Math.Sin(this.rec_alpha));
            this.drawRec(alp);
            this.rec_alpha += 0.3f;
        }
    }
}
