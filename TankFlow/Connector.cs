using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankFlow
{
    [Obsolete]
    static class Connector
    {
        public static bool add_damage(Damage damage)
        {
            Param para = new Param();
            para.Add("attackers", damage.source, Param.STR);
            para.Add("victims", damage.victim, Param.STR);
            para.Add("damagess", damage.damage, Param.INT);
            para.Add("damagetypes", damage.type, Param.INT);
            para.Add("battletypes", damage.battleType, Param.INT);
            para.Add("battleids", damage.battleId, Param.INT);
            if (MySqlAPI.ExecuteProcedure("add_damage", para) == 0)
                return true;
            else
                return false;
        }
    }
}
