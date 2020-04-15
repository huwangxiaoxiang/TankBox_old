﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TankFlow
{
    class uploadPack
    {
        private List<Damage> damages = new List<Damage>();

        public void AddDamage(Damage d)
        {
            this.damages.Add(d);
        }

        private string getJSON()
        {
            string result = "[";
            foreach(Damage damage in damages)
            {
                string json = "{";
                json = json + "'attacker':'" + damage.source + "',";
                json = json + "'victim':'" + damage.victim + "',";
                json = json + "'damage':" + damage.damage.ToString() + ",";
                json = json + "'damagetype':" + damage.type.ToString() + ",";
                json = json + "'battletype':" + damage.battleType.ToString() + ",";
                json = json + "'battleid':" + damage.battleId.ToString();
                json = json + "}";
                result = result + json + ",";
            }
            result += "]";
            return result;
        }

        public void uploadDamage()
        {
            if (this.damages.Count > 0)
            {
                try
                {
                    string param = "param=" + this.getJSON();
                    string result = HttpConnect.HttpPost(HttpConnect.mBaseURL+ "add_damage", param);
                    Console.WriteLine("上传" + this.damages.Count.ToString() + "条数据，" + (this.damages.Count - int.Parse(result)).ToString() + "条失败");
                }
                catch (Exception e) { }
                
            }
        }

     
    }
    class UploadManager
    {
        private int perMagine = 20;
        private List<uploadPack> packs = new List<uploadPack>();
        public UploadManager(List<Damage> datas)
        {
            if (datas.Count <= 0) return;
            int m = (int)(datas.Count / this.perMagine) + 1;//将数据分组
            for(int i = 0; i < m - 1; i++)
            {
                uploadPack pack = new uploadPack();
                for (int j = 0; j < perMagine; j++)
                    pack.AddDamage(datas[i * perMagine + j]);
                packs.Add(pack);
            }
            uploadPack pack1 = new uploadPack();
            for (int i = (m - 1) * perMagine; i < datas.Count; i++)
            {
                pack1.AddDamage(datas[i]);
            }
            packs.Add(pack1);
            Console.WriteLine("将" + datas.Count.ToString() + "条数据分成了" + packs.Count.ToString() + "组");
        }

        public void Upload()
        {
            if (this.packs.Count <= 0) return;
            foreach(uploadPack pack in packs)
            {
                Thread th = new Thread(() =>
                {
                    pack.uploadDamage();
                });
                th.Start();
            }
        }

    }
}