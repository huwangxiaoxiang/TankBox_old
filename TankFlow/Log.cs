using System;
using System.IO;


namespace TankFlow
{
    static class Log
    {
        private static string path = "log.txt";
        public static void AddLog(string s)
        {
            /**/
#if   DEBUG 
            s = DateTime.Now.ToString() + "   " + s;
            FileStream fs = new FileStream(path, FileMode.Append);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(s);
            sw.Close();
            fs.Close();
            Console.WriteLine(s);
#endif
        }
    }
}
