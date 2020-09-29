using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Threading;


namespace TankFlow
{
    class uploadPack
    {
        private List<Damage> damages = new List<Damage>();
        private int mMaxRetryTimes = 5;
        private int pack_part = 0;

        public void AddDamage(Damage d)
        {
            this.damages.Add(d);
        }

        public void setPart(int i)
        {
            this.pack_part = i;
        }

        private string getJSON()
        {
            JArray array = new JArray();
            foreach (Damage damage in damages)
            {
                JObject temp = new JObject();
                temp.Add("attacker", damage.source);
                temp.Add("victim", damage.victim);
                temp.Add("damage", damage.damage);
                temp.Add("damagetype", damage.type);
                temp.Add("battletype", damage.battleType);
                temp.Add("battleid", damage.battleId.ToString() + "_" + pack_part.ToString());
                temp.Add("part", damage.hitpart);
                array.Add(temp);
            }
            //Console.WriteLine(array.ToString());
            return array.ToString();
        }

        public void uploadDamage()
        {
            if (this.damages.Count > 0)
            {
                string param = "param=" + this.getJSON();
                for (int i = 0; i < this.mMaxRetryTimes; i++)
                {
                    try
                    {
                        string result = HttpConnect.HttpPost(HttpConnect.mBaseURL + "add_damage", param);
                        Log.AddLog("上传" + this.damages.Count.ToString() + "条数据，" + (this.damages.Count - int.Parse(result)).ToString() + "条失败");
                        break;
                    }
                    catch (Exception e)
                    {
                        Log.Record("第" +(i+1).ToString()+"次数据上传失败，正在第"+(i+2).ToString()+"次尝试。");
                    }
                }
            }
        }


    }
    class UploadManager
    {
        private int perMagine = 200;
        private List<uploadPack> packs = new List<uploadPack>();
        public UploadManager(List<Damage> datas)
        {
            if (datas.Count <= 0) return;
            int m = (int)(datas.Count / this.perMagine) + 1;//将数据分组
            for (int i = 0; i < m - 1; i++)
            {
                uploadPack pack = new uploadPack();
                pack.setPart(i);
                for (int j = 0; j < perMagine; j++)
                    pack.AddDamage(datas[i * perMagine + j]);
                packs.Add(pack);
            }
            uploadPack pack1 = new uploadPack();
            pack1.setPart(m - 1);
            for (int i = (m - 1) * perMagine; i < datas.Count; i++)
            {
                pack1.AddDamage(datas[i]);
            }
            packs.Add(pack1);
            Log.AddLog("将" + datas.Count.ToString() + "条数据分成了" + packs.Count.ToString() + "组");
        }

        public void Upload()
        {
            if (this.packs.Count <= 0) return;
            foreach (uploadPack pack in packs)
            {
                pack.uploadDamage();
            }
        }

    }
}
