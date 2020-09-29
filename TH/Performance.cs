using System;
using System.Collections.Generic;
using System.Text;

namespace TH
{
   

    class Performance
    {
        private static Dictionary<string, Recorder> record = new Dictionary<string, Recorder>();

        public static void Start(string name)
        {
            if (record.ContainsKey(name))
            {
                record[name].StartOnce();
            }
            else
            {
                Recorder temp = new Recorder(name);
                temp.StartOnce();
                record.Add(name,temp);
            }
        }

        public static void EndOnce(string name)
        {
            if (record.ContainsKey(name))
            {
                record[name].EndOnce();
            }
        }

        public static void Init()
        {
            record.Clear();
        }

        public static void LogRecord()
        {
            foreach(KeyValuePair<string,Recorder> m in record)
            {
                m.Value.LogRecord();
            }
        }

        class Recorder
        {
            private string name;
            private long count=0;
            private long times = 0;
            private long start_time = 0;
            public Recorder(string name)
            {
                this.name = name;
                this.count = 0;
                this.times = 0;
            }

            public void StartOnce()
            {
                this.start_time = DateTime.Now.Ticks;
            }

            public void EndOnce()
            {
                this.count++;
                this.times = this.times + (DateTime.Now.Ticks - this.start_time) / 10000;
            }

            public void LogRecord()
            {
                Log.Record(name + " 运行" + count.ToString() + "次，总用时:" + this.times.ToString() + "毫秒");
            }
        }
    }
}
