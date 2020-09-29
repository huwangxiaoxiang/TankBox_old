using System;
using System.Collections.Generic;
using System.Threading;

namespace TH
{
    class ExternTask
    {
        private static Dictionary<string, uint> properties = new Dictionary<string, uint>();
      
        private static bool advance = false;
        private static Semaphore locks=new Semaphore(1,1);

        public static void setAdvanced(bool flag)
        {
            ExternTask.advance = flag;
            readProperties();
        }

        public static void readProperties()
        {
            string line;
            locks.WaitOne();
            properties.Clear();
            try
            {
                System.IO.StreamReader file = new System.IO.StreamReader("C://Users//12703//Documents//tkdz//Tank_Data//Boxconfig");
                while ((line = file.ReadLine()) != null)
                {
                    string[] m = line.Split(' ');
                    if (m.Length != 2) continue;
                    uint k = 0u;
                    if (uint.TryParse(m[1], out k))
                    {
                        properties.Add(m[0], k);
                    }
                }
            }
            catch (Exception e) {
                Console.WriteLine(e.Message);
            }
            if (ExternTask.advance)
            {
                ExternTask.setProperties("lock_tank", 1u);
                ExternTask.setProperties("leave_battle", 1u);
            }
            else
            {
                ExternTask.setProperties("lock_tank", 0u);
                ExternTask.setProperties("leave_battle", 0u);
            }
            locks.Release();
        }

        /// <summary>
        /// 加载已读取的配置信息（性能高）
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public static bool getProperties(string name)
        {
            if (properties.ContainsKey(name))
            {
                if (properties[name] == 0u)
                    return false;
                else return true;
            }
            else return false;
        }

        public static uint getPropertiesInt(string name)
        {
            if (properties.ContainsKey(name))
            {
                return properties[name];
            }
            else
            {
                return 0;
            }
        }

        /// <summary>
        /// 重新加载配置文件后获取配置信息（性能低）
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public static bool getPropertiesImmediately(string name)
        {
            readProperties();
            return getProperties(name);
        }

        public static void setProperties(string name, uint num)
        {
            if (properties.ContainsKey(name))
            {
                properties[name] = num;
            }
            else
            {
                properties.Add(name, num);
            }
        }

        public static void print()
        {
            foreach (string item in properties.Keys)
            {
                Console.WriteLine(item + " : " + properties[item].ToString());
            }
        }
    }
}
