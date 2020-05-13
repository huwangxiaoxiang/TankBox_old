using System;
using System.IO;
using System.Threading;

namespace TankFlow
{
    static class Log
    {
        private static Semaphore sema = new Semaphore(1, 1);
        private static string path = "log.txt";
        public static void AddLog(string s)
        {
            /**/
#if   DEBUG 
            s = DateTime.Now.ToString() + "   " + s;
            /**/
            sema.WaitOne();
            FileStream fs = new FileStream(path, FileMode.Append);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(s);
            sw.Close();
            fs.Close();
            sema.Release();
            Console.WriteLine(s);
#endif
        }

        public static void Record(string s)
        {
            s = DateTime.Now.ToString() + "   " + s;
            /**/
            sema.WaitOne();
            FileStream fs = new FileStream(path, FileMode.Append);
            StreamWriter sw = new StreamWriter(fs);
            sw.WriteLine(s);
            sw.Close();
            fs.Close();
            sema.Release();
            Console.WriteLine(s);
        }
    }
}