namespace TankFlow
{
    public class PythonClient : SocketClient
    {
        private TankFlow form;
        public PythonClient(int port, string ip, TankFlow form) : base(port, ip)
        {
            this.form = form;
        }

        protected override void OnReceiveMessage(string message)
        {
            Log.AddLog("收到服务器" + this.GetRemoteIP() + "的消息:" + message);
            string[] strs = message.Split('_');
            if (strs.Length < 2) return;
            if (strs[0] == "1")
            {
                form.HandleRecognizeResult(strs[1], 1);
            }
            if (strs[0] == "2")
            {
                form.HandleRecognizeResult(strs[1], 2);
            }
        }
    }
}
