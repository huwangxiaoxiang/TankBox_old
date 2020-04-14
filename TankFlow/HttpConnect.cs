
using System;
using System.Collections.Generic;
using System.IO;

using System.Net;
using System.Text;
using System.Web;

namespace TankFlow
{
    class HttpConnect
    {
        public static readonly string mBaseURL = "http://localhost:8080/Tank_Service_SSM/";

        public static bool UploadBattleResult(BattleResult result)
        {
            string request = "add_battle_result";
            Dictionary<string, object> dic = new Dictionary<string, object>();
            dic.Add("user_id", result.user_id);
            dic.Add("tank", result.tank);
            dic.Add("modes", result.mode);
            dic.Add("victory", result.victory); 
            string param = GetJSONParam(dic);
            //Console.WriteLine(param);
            string response = HttpConnect.HttpPost(mBaseURL + request, param);
            if (response == "0") return false;
            else return true;

        }
        public static string getParam(Damage damage)
        {
            string tempURL = mBaseURL;
            string json = "{";
            json = json + "'attacke':'" + damage.source + "',";
            json=  json + "'victim':'" + damage.victim +"',";
            json = json+ "'damage':" + damage.damage.ToString() + ",";
            json = json + "'damagetype':" + damage.type.ToString() + ",";
            json = json + "'battletype':" + damage.battleType.ToString() + ",";
            json = json + "'battleid':" + damage.battleId.ToString() ;
            json = json + "}";
            return "param="+json;
        }

        public static string GetJSONParam(Dictionary<string, object> param)
        {
            string json = "{";
            foreach (KeyValuePair<string, object> item in param)
            {
                json = json + "'" + item.Key + "':'" + item.Value + "',";
            }
            json = json + "}";
            return "param=" + json;
        }

        public static string getParam(string user_id, string tank, string victory, int modes)
        {
            string json = "{";
            json = json + "'user_id':'" + user_id + "',";
            json = json + "'tank':'" + tank + "',";
            json = json + "'victory':" + victory + ",";
            json = json + "'modes':" + modes.ToString();
            json = json + "}";
            return "param=" + json;
        }

        public static string HttpGet(string URL)
        {
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(URL);
            request.Method = "GET";
            request.ContentType = "application/json;charset=UTF-8";

            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream myResponseStream = response.GetResponseStream();
            StreamReader myStreamReader = new StreamReader(myResponseStream, Encoding.GetEncoding("utf-8"));
            string retString = myStreamReader.ReadToEnd();
            myStreamReader.Close();
            myResponseStream.Close();

            return retString;
        }

        public static string HttpPost(string url, string param)
        {
            string result = "";
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
            req.Method = "POST";
            req.ContentType = "application/x-www-form-urlencoded";

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
            }
            return result;
        }
    }
}
