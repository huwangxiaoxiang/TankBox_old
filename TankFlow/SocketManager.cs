using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace TankFlow
{

    public abstract class Receiver
    {
        abstract public void OnReceiveMessage(string host, string message);
        abstract public void OnReceiveConnect(string host);
        abstract public void OnBreakConnect(string host);
        abstract public void OnCompleteReceive(string host);
    }



    class SocketManager
    {
        private static Mutex socket_mutex = new Mutex();

        private static Dictionary<string, Socket> clients = new Dictionary<string, Socket>();

        private static SocketServer server;
        public static void addSocket(string ip, Socket socket)
        {
            socket_mutex.WaitOne();
            clients.Add(ip, socket);
            socket_mutex.ReleaseMutex();
        }

        public static void StartServer(int port, Receiver receiver, bool localIP = true)
        {
            if (SocketManager.server != null) return;
            SocketServer server = new SocketServer(port, receiver, localIP);
            server.StartListen();
            SocketManager.server = server;
        }

        public static void CloseConnect(string host)
        {
            Socket client = GetClientSocket(host);
            client.Close();
            socket_mutex.WaitOne();
            clients.Remove(host);
            socket_mutex.ReleaseMutex();

        }

        public static void StopServer()
        {
            foreach (string item in clients.Keys)
            {
                clients[item].Close();
            }
            if (server != null)
            {
                server.CloseServer();
            }
            socket_mutex.WaitOne();
            clients.Clear();
            socket_mutex.ReleaseMutex();

            SocketManager.server = null;
        }
        public static Socket GetClientSocket(string ip)
        {
            if (clients.ContainsKey(ip)) return clients[ip];
            else return null;
        }

        public static int SendData(string ip, string message)
        {
            Socket socket = GetClientSocket(ip);
            if (socket != null)
                return socket.Send(Encoding.UTF8.GetBytes(message));
            else return 0;
        }

        public static int SendByte(string ip, byte[] buffer, int size)
        {
            Socket socket = GetClientSocket(ip);
            if (socket != null)
                return socket.Send(buffer, size, SocketFlags.None);
            else return 0;
        }


    }

    public class SocketServer
    {
        private string _ip = string.Empty;
        private int _port = 0;
        private Socket _socket = null;
        private byte[] buffer = new byte[1024 * 1024];
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
            catch(Exception e)
            {
                Log.Record("获取本机IP失败:" + e.Message);
                return "";
            }
        }

        public SocketServer(int port, Receiver receiver, bool localIP)
        {
            if (localIP)
                this._ip = GetLocalIP();
            else
                this._ip = "127.0.0.1";
            this._port = port;
            this.mMessageReceiver = receiver;
        }

        public SocketServer(int port, string ip, Receiver receiver)
        {
            this._ip = ip;
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
                Log.Record("服务端开启失败:" + ex.Message);
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
                catch(Exception e)
                {
                    Log.Record("服务器监听连接线程已捕获到异常，服务端监听已停止"+e.Message);
                }
            }
            try
            {
                _socket.Close();
            }
            catch {
            }
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
                    string message = "";
                    int length_sum = 0;
                    do
                    {
                        int length = clientSocket.Receive(buffer);
                        if (length == 0) break;
                        length_sum += length;
                        message += Encoding.UTF8.GetString(buffer, 0, length);
                        if (length < 1024 * 1024)
                        {
                            this.mMessageReceiver.OnCompleteReceive(clientIp);
                            break;
                        }
                    } while (true);

                    if (length_sum == 0) break;
                    //Log.AddLog("服务端收到消息:" + message);
                    mMessageReceiver.OnReceiveMessage(clientIp, message);
                }
                catch (Exception ex)
                {
                    Log.Record("Socket连接已关闭：" + ex.Message);
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
        private bool close = true;
        private uint messageType = 0;//0代表string型的数据，1代表byte型的数据
        private string remoteIp = string.Empty;


        public SocketClient(int port, string ip, uint type = 0)
        {
            this._ip = ip;
            this._port = port;
            this.messageType = type;
        }

        public string GetRemoteIP()
        {
            return this.remoteIp;
        }

        public int GetPort()
        {
            return this._port;
        }

        public bool IsConnect()
        {
            return !this.close;
        }

        protected virtual void OnReceiveMessage(string message)
        {

        }

        protected virtual void OnReceiveByte(byte[] buffer, int length)
        {

        }

        protected virtual void OnCompleteReceive()
        {

        }

        protected virtual void OnConnectBroken()
        {

        }

        protected virtual void OnConnectEstablish()
        {

        }


        public bool ConnectServer()
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
                this.remoteIp = _socket.RemoteEndPoint.ToString();
                this.close = false;
                this.OnConnectEstablish();
                Thread s = new Thread(OnReceive);
                s.Start();

            }
            catch (Exception ex)
            {
                _socket.Close();
                Log.Record("连接服务器失败:" + ex.Message);
                this.close = true;
                return false;
            }
            return true;
        }

        public void DisConnect()
        {
            if (_socket != null)
                _socket.Close();
            this.close = true;
        }

        public void SendData(string s)
        {
            byte[] buffer = Encoding.UTF8.GetBytes(s);
            this.SendByte(buffer, buffer.Length);
        }

        public void SendByte(byte[] data, int size)
        {
            try
            {
                _socket.Send(data, size, SocketFlags.None);
            }
            catch (Exception e)
            {
                Log.Record("byte消息发送失败:" + e.Message);
            }
        }

        private void OnReceive()
        {
            while (!this.close)
            {
                if (this._socket == null) break;
                try
                {
                    string message = "";
                    int length_sum = 0;
                    do
                    {
                        int length = _socket.Receive(buffer);
                        if (length == 0) break;
                        if (this.messageType == 1)
                            this.OnReceiveByte(buffer, length);
                        else if (this.messageType == 0)
                            message += Encoding.UTF8.GetString(buffer, 0, length);
                        length_sum += length;
                        if (length < 1024 * 1024 * 2)
                        {
                            this.OnCompleteReceive();
                            break;
                        }

                    } while (true);

                    if (length_sum == 0) break;
                    if (this.messageType == 0)
                        this.OnReceiveMessage(message);

                    // Log.AddLog("收到服务端消息:" + Encoding.UTF8.GetString(buffer, 0, length));
                }
                catch (Exception e)
                {
                    this.close = true;
                   Log.Record("Socket连接已关闭:"+e.Message);
                    break;
                }
            }
            if (this._socket != null)
                this._socket.Close();
            this.OnConnectBroken();
            this._socket = null;
            this.close = true;
            Log.AddLog("[客户端]与服务端的连接已断开");
        }

    }

}
