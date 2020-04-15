using System;
using System.IO;


namespace TankFlow
{
    static class Log
    {
        private static string path = "log.txt";
        public static void AddLog(string s)
        {
            /*
            FileStream fs = new FileStream(path, FileMode.Append);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(DateTime.Now.ToString()+"   "+s);
            sw.Close();
            fs.Close();*/
        }
    }
}
