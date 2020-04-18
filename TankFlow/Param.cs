using MySql.Data.MySqlClient;

using System.Collections.Generic;
using System.Data;


namespace TankFlow
{
    class Param
    {
        List<MySqlParameter> lists = new List<MySqlParameter>();
        MySqlParameter output = new MySqlParameter();
        bool is_added = false;
        public const  int STR=1;
        public const int INT = 2;
        public const int DOUBLE = 3;
        public const int INPUT = 4;
        public const int OUTPUT = 5;

        public Param()
        {
            lists.Clear();
            SetResult("result", INT);
        }
        public List<MySqlParameter> getLists()
        {
            if (!is_added)
            {
                lists.Add(output);
                is_added = true;
            }
            return lists;
        }

        public void Add(string paranam,object value,int type)
        {
            Add(paranam, value, type,INPUT);
        }

        public void Add(string paranam, object value,int type,int direction)
        {
            MySqlParameter m = new MySqlParameter("@" + paranam, getParaType(type));
            m.Value = value;
            m.Direction = getDirectioin(direction);
            lists.Add(m);
        }

        public void SetResult(string name,int type)
        {
            output.ParameterName = "@" + name;
            output.DbType = (DbType)getParaType(type);
            output.Value = null;
            output.Direction = getDirectioin(OUTPUT);
        }

        private static System.Data.ParameterDirection getDirectioin(int flag)
        {
            switch (flag)
            {
                case INPUT:
                    return System.Data.ParameterDirection.Input;
                case OUTPUT:
                    return System.Data.ParameterDirection.Output;
                default:
                    return System.Data.ParameterDirection.Input;
            }
        }

        private static MySqlDbType getParaType(int flag)
        {
            switch (flag)
            {
                case STR:
                    return MySqlDbType.VarChar;
                case INT:
                    return MySqlDbType.Int32;
                case DOUBLE:
                    return MySqlDbType.Double;
                default:
                    return MySqlDbType.Text;
            }
        }
    }
}
