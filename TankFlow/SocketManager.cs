using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TankFlow
{

    public abstract class  Receiver
    {
        abstract public void OnReceiveMessage(string host,string message);
        abstract public void OnReceiveConnect(string host);
        abstract public void OnBreakConnect(string host);
    }


    class ServerReceiver : Receiver
    {
        public override void OnReceiveMessage(string host, string message)
        {
            Log.AddLog("收到来自" + host + "的消息(ReceiveMessage)：" + message);
            /*
            string[] strs = message.Split('_');
            if (strs.Length != 2)
                return;
            if (strs[0] == "1"&&strs[1]!="")
            {
                GUIEntityBattleChat entityone = GUISystem.Instance.GetEntity<GUIEntityBattleChat>();
                entityone.SendBattleChatMessage(strs[1], BattleChatChannel.BCC_TEAM);
            }
            */

        }
        public override void OnReceiveConnect(string host)
        {
            Log.AddLog("收到来自" + host + "的连接(ReceiveConnect)" );
        }

        public override void OnBreakConnect(string host)
        {
            Log.AddLog(host + "连接已断开(BreakConnect)");
        }
    }

    class ClientReceiver : Receiver
    {
        TankFlow window;
        /**/
        public ClientReceiver(TankFlow window)
        {
           
            this.window = window;
        }
        
        public override void OnBreakConnect(string host)
        {
            Log.AddLog("与服务器:" + host + "的连接断开");
        }

        public override void OnReceiveConnect(string host)
        {
            Log.AddLog("与服务器:" + host + "连接");
        }

        public override void OnReceiveMessage(string host, string message)
        {
            /**/
            Log.AddLog("收到服务器"+host+"的消息:" + message);
            string[] strs = message.Split('_');
            if (strs.Length < 2) return;
            if (strs[0] == "1")
            {
                window.HandleRecognizeResult(strs[1],1);
            }
            if (strs[0] == "2")
            {
                Log.AddLog("准备向游戏客户端发送数据:" + "1_" + strs[1]);
                this.window.tank_client.SendData("1_" + strs[1]);
                window.HandleRecognizeResult(strs[1],2);
            }
            
        }
    }

    class SocketManager
    {
        private static Dictionary<string, long> status = new Dictionary<string, long>();
        private static Dictionary<string, Socket> clients = new Dictionary<string, Socket>();
        private static Thread protect_thread;
        
        private static SocketServer server;
        private static bool protect_exit = false;
        public static void addSocket(string ip,Socket socket)
        {
            status.Add(ip, DateTime.Now.Ticks);
            clients.Add(ip,socket);
        }
        private static void StartKeepAlive()
        {
            if (protect_thread != null)
            {
                return;
            }
            protect_thread = new Thread(KeepAlive);
            protect_exit = false;
            protect_thread.Start();
        }
        private static void StopKeepAlive()
        {
            protect_exit = true;
            if (protect_thread != null)
            {
                Log.AddLog("等待服务端守护线程关闭...");
                protect_thread.Join();
            }
            protect_thread = null;
            string message = "0 close";
            foreach(KeyValuePair<string,Socket> item in clients)
            {
                item.Value.Send(Encoding.UTF8.GetBytes(message));
                item.Value.Close();
            }
            clients.Clear();
            status.Clear();
            Log.AddLog("服务端守护线程已关闭");
        }
        private static void KeepAlive()
        {
            Log.AddLog("服务端Socket守护线程已开启");
            while (!protect_exit)
            {
                long now = DateTime.Now.Ticks;
                foreach(KeyValuePair<string,long> item in status)
                {
                    long delta = now - item.Value;
                    if ( delta> 10* 10000000)//大于10秒没有数据通信则关闭该Socket
                    {
                        clients[item.Key].Close();
                        clients.Remove(item.Key);
                        status.Remove(item.Key);
                        break;
                    }
                }
                Thread.Sleep(2000);
            }
        }
        public static void ReportMessage(string ip)
        {
            if (status.ContainsKey(ip))
            {
                status[ip] = DateTime.Now.Ticks;
            }
        }

        public static void StartServer(int port,Receiver receiver)
        {
            if (SocketManager.server != null) return;
            SocketManager.StartKeepAlive();
            SocketServer server = new SocketServer(port, receiver);
            server.StartListen();
            SocketManager.server = server;
            
        }

        public static void StopServer()
        {
            StopKeepAlive();
            if (server != null)
            {
                server.CloseServer();
            }
            SocketManager.server = null;
        }
        public static Socket GetClientSocket(string ip)
        {
            if (clients.ContainsKey(ip)) return clients[ip];
            else return null;
        }
        
        public static void SendData(string ip,string message)
        {
            Socket socket = GetClientSocket(ip);
            if (socket != null)
                socket.Send(Encoding.UTF8.GetBytes(message));
        }
    }

    public class SocketServer
    {
        private string _ip = string.Empty;
        private int _port = 0;
        private Socket _socket = null;
        private byte[] buffer = new byte[1024 * 1024 * 2];
        private bool close = false;
        private Receiver mMessageReceiver = null;

        public static string GetLocalIP()
        {
            try
            {
                IPHostEntry IpEntry = Dns.GetHostEntry(Dns.GetHostName());
                foreach (IPAddress item in IpEntry.AddressList)
                {
                    if (item.AddressFamily == AddressFamily.InterNetwork)
                    {
                        return item.ToString();
                    }
                }
                return "";
            }
            catch { return ""; }
        }

        public SocketServer(int port,Receiver receiver)
        {
            this._ip = "127.0.0.1";
            this._port = port;
            this.mMessageReceiver = receiver;
        }

        public void CloseServer()
        {
            this.close = true;
            this._socket.Close();
        }

        public void StartListen()
        {
            try
            {
                //1.0 实例化套接字(IP4寻找协议,流式协议,TCP协议)
                _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                //2.0 创建IP对象
                IPAddress address = IPAddress.Parse(_ip);
                //3.0 创建网络端口,包括ip和端口
                IPEndPoint endPoint = new IPEndPoint(address, _port);
                //4.0 绑定套接字
                _socket.Bind(endPoint);
                //5.0 设置最大连接数
                _socket.Listen(int.MaxValue);
                Console.WriteLine("服务端{0}开启成功", _socket.LocalEndPoint.ToString());
                //6.0 开始监听
                Thread thread = new Thread(ListenClientConnect);
                thread.Start();
            }
            catch (Exception ex)
            {
                Log.AddLog("服务端开启失败");
            }
        }
   
        private void ListenClientConnect()
        {
            while (!this.close)
             {
                try
                {
                    //Socket创建的新连接
                    Socket clientSocket = _socket.Accept();
                    Log.AddLog("接收到一个来自" + clientSocket.RemoteEndPoint.ToString() + "的连接");
                    SocketManager.addSocket(clientSocket.RemoteEndPoint.ToString(), clientSocket);
                    mMessageReceiver.OnReceiveConnect(clientSocket.RemoteEndPoint.ToString());
                    Thread thread = new Thread(ReceiveMessage);
                    thread.Start(clientSocket);
                }
                catch
                {
                    Log.AddLog("服务器监听连接线程已捕获到异常，服务端监听已停止");
                }
            }
            try
            {
                _socket.Close();
            }
            catch { }
            
            Log.AddLog("服务端监听已关闭");
        }

        private void ReceiveMessage(object socket)
        {
            Socket clientSocket = (Socket)socket;
            string clientIp = clientSocket.RemoteEndPoint.ToString();
            while (true)
            {
                try
                {
                    int length = clientSocket.Receive(buffer);
                    string message = Encoding.UTF8.GetString(buffer, 0, length);
                    if (message == "0 alive")
                    {
                        SocketManager.ReportMessage(clientIp);
                        continue;
                    }
                    if (message=="") break;
                    int index = message.IndexOf(' ');
                    if(this.mMessageReceiver!=null&&message.Length>2)
                        mMessageReceiver.OnReceiveMessage(clientIp, message.Substring(index+1,message.Length-index-1));
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                    break;
                }
            }
            clientSocket.Close();
            mMessageReceiver.OnBreakConnect(clientIp);
            Log.AddLog("客户端" + clientIp + "的连接已断开");
        }
    }

    public class SocketClient
    {
        private string _ip = string.Empty;
        private int _port = 0;
        private Socket _socket = null;
        private byte[] buffer = new byte[1024 * 1024 * 2];
        private bool close = false;
        private Receiver mMessageReceiver;

        public SocketClient(int port,Receiver receiver)
        {
            this._ip = SocketServer.GetLocalIP();
            this._port = port;
            this.mMessageReceiver = receiver;
        }

        public SocketClient(int port,string ip, Receiver receiver)
        {
            this._ip = ip;
            this._port = port;
            this.mMessageReceiver = receiver;
        }

        public void ConnectServer()
        {
            try
            {
                if (this._socket == null)
                {
                    //1.0 实例化套接字(IP4寻址地址,流式传输,TCP协议)
                    _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                }
                //2.0 创建IP对象
                IPAddress address = IPAddress.Parse(_ip);
                //3.0 创建网络端口包括ip和端口
                IPEndPoint endPoint = new IPEndPoint(address, _port);
                //4.0 建立连接
                _socket.Connect(endPoint);
                this.mMessageReceiver.OnReceiveConnect(_socket.RemoteEndPoint.ToString());
                this.KeepAlive();//保持连接
                Thread s = new Thread(OnReceive);
                s.Start();
               
            }
            catch (Exception ex)
            {
                _socket.Close();
                Console.WriteLine(ex.Message);
            }
            
        }

        public void DisConnect()
        {
            if(_socket!=null)
                _socket.Close();
            this.close = true;
        }

        public void SendData(string s)
        {
            try
            {
                _socket.Send(Encoding.UTF8.GetBytes("1 "+s));
            }
            catch (Exception ex)
            {
                Log.AddLog("消息发送失败:" + s);
            }
        }

        public void OnReceive()
        {
            string remoteIp = _socket.RemoteEndPoint.ToString();
            while (!this.close)
            {
                if (this._socket == null) break;
                try
                {
                    int length = _socket.Receive(buffer);
                    if (length == 0) break;
                    string message = Encoding.UTF8.GetString(buffer, 0, length);
                    if (message == "0 close")
                    {
                        this.close = true;
                    }
                    else{
                        mMessageReceiver.OnReceiveMessage(remoteIp,message);
                    }
                   // Log.AddLog("收到服务端消息:" + Encoding.UTF8.GetString(buffer, 0, length));
                }catch(Exception e)
                {
                    this.close = true;
                    break;
                }
            }
            if(this._socket!=null)
                   this._socket.Close();
            mMessageReceiver.OnBreakConnect(remoteIp);
            Log.AddLog("[客户端]与服务端的连接已断开");
        }

        private void KeepAlive()
        {
            Thread th = new Thread(() =>
            {
                String ip = _socket.RemoteEndPoint.ToString();
                while (!close)
                {
                    if (_socket == null)
                    {
                        this.close = true;
                        break;
                    }
                    try
                    {
                        _socket.Send(Encoding.UTF8.GetBytes("0 alive"));
                    }catch(Exception e)
                    {
                        this.close = true;
                        this._socket = null;
                        break;
                    }
                    Thread.Sleep(4000);
                }
               
            });
            th.Start();
        }
    }

}
