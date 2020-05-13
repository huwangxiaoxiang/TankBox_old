

using System;

namespace TankFlow
{
    class MessageReceiver : Receiver
    {
        private TankFlow form;

        public MessageReceiver(TankFlow form)
        {
            this.form = form;
        }
        public override void OnBreakConnect(string host)
        {

        }

        public override void OnCompleteReceive(string host)
        {

        }

        public override void OnReceiveConnect(string host)
        {

        }

        public override void OnReceiveMessage(string host, string message)
        {
            //Log.AddLog("TankFlow服务端收到消息：" + message);
            try
            {
                string[] m = message.Split('&');
                foreach (string temp in m)
                {
                    if (temp.Length <= 0) continue;
                    //Log.AddLog("TankFlow服务端准备解析字符串：" + temp);
                    ReceivedData data = new ReceivedData(temp);
                    form.HandleCopyData(host, data);
                }
            }catch(Exception e)
            {
                Log.Record("解析数据错误：" + message + " " + e.Message);
            }
           

        }
    }
}
