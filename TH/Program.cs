using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace TH
{
    class Program
    {
        static ProductBuffer buffer = new ProductBuffer(3);
        static int count = 0;
        static void Main(string[] args)
        {
            ExternTask.setAdvanced(true);
            ExternTask.print();
            Thread.Sleep(20000);
        }

        static void Output()
        {
            while (true)
            {
                object s=buffer.TryGetProduct();
                if (s == null) Console.WriteLine("获取产品失败");
                else
                    Console.WriteLine((string)s);
                Thread.Sleep(500);
            }
            
        }

        static void Input()
        {
            while (true)
            {
                bool result=buffer.TryAddProduct(count.ToString());
                if (result == false)
                {
                    Console.WriteLine("添加产品失败:" + count.ToString());
                }
                else
                {
                    count++;
                }
                Thread.Sleep(100);
            }
        }
    }
}
