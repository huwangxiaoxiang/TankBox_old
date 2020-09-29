using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;

using System.Net;
using System.Text;

namespace TankFlow
{
    class HttpConnect
    {
        public static readonly string mBaseURL = "http://www.bestxiaoxiang.top/Tank_Service_SSM/";
        private static int mMaxRetryTimes = 5;

        public static bool UploadBattleResult(BattleResult result)
        {
            string request = "add_battle_result2";
            //Dictionary<string, object> dic = new Dictionary<string, object>();
            JObject dic = new JObject();
            dic.Add("user_id", result.user_id);
            dic.Add("tank", result.tank);
            dic.Add("modes", result.mode);
            dic.Add("victory", result.victory);
            dic.Add("honor", result.honor);
            dic.Add("shoot", result.shoot);
            dic.Add("hits", result.hits);
            dic.Add("penertration", result.penertration);
            dic.Add("damage_caused", result.damage_caused);
            dic.Add("hits_received", result.hits_received);
            dic.Add("damage_received", result.damage_received);
            dic.Add("enermy_detected", result.enermy_detected);

            string param = "param=" + dic.ToString();
            //Console.WriteLine(param);
            for(int i = 1; i <= mMaxRetryTimes; i++)
            {
                string response = HttpConnect.HttpPostSafe(mBaseURL + request, param);
                if (response != "")
                {
                    Log.AddLog("上传战斗结果成功。");
                    return true;
                }
                Log.Record("上传战斗结果异常，正在重试第" + i.ToString() + "次");
            }
            Log.Record("上传战斗结果失败，重试了" + mMaxRetryTimes.ToString() + "次仍然失败，已丢弃数据");
            return false;
        }

        public static string BattleStart(string uid)
        {
            JObject param = new JObject();
            param.Add("id", uid);
            string response = HttpConnect.HttpPostSafe(mBaseURL + "mBattleStart", "param=" + param.ToString());
            if (response == "") return "0";
            param = JObject.Parse(response);
            return (string)param.GetValue("result");
        }

        public static bool BattleEnd(string uid)
        {
            JObject param = new JObject();
            param.Add("id", uid);
            string response = HttpConnect.HttpPostSafe(mBaseURL + "mBattleEnd", "param=" + param.ToString());
            if (response == "") return false;
            param = JObject.Parse(response);
            return (bool)param.GetValue("result");
        }

        public static string HttpPostSafe(string url,string param)
        {
            string result = "";
            try
            {
                result = HttpPost(url, param);
            }catch(Exception e)
            {
                Log.AddLog(e.Message);
            }
            return result;
        }

        public static string HttpGet(string URL)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(URL);
            request.Method = "GET";
            request.ContentType = "application/json;charset=UTF-8";
            request.UserAgent = "tanbox";
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream myResponseStream = response.GetResponseStream();
            StreamReader myStreamReader = new StreamReader(myResponseStream, Encoding.GetEncoding("utf-8"));
            string retString = myStreamReader.ReadToEnd();
            myStreamReader.Close();
            myResponseStream.Close();
            response.Close();
            
            return retString;
        }

        public static string HttpPost(string url, string param)
        {
            string result = "";
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
            req.Method = "POST";
            req.ContentType = "application/x-www-form-urlencoded";
            req.UserAgent = "tanbox";

            byte[] data = Encoding.UTF8.GetBytes(param);//把字符串转换为字节

            req.ContentLength = data.Length; //请求长度

            using (Stream reqStream = req.GetRequestStream()) //获取
            {
                reqStream.Write(data, 0, data.Length);//向当前流中写入字节
                reqStream.Close(); //关闭当前流
            }

            HttpWebResponse resp = (HttpWebResponse)req.GetResponse(); //响应结果
            Stream stream = resp.GetResponseStream();
            //获取响应内容
            using (StreamReader reader = new StreamReader(stream, Encoding.UTF8))
            {
                result = reader.ReadToEnd();
                reader.Close();
            }
            stream.Close();
            resp.Close();
            
            return result;
        }
    }
}
