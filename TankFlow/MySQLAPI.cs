using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Data;
using System.Threading;


namespace TankFlow
{
    [Obsolete]
    static class MySqlAPI
    {
        static string IP="47.101.70.188";
        static int port = 3306;
        static string username = "xiaoxiang";
        static string password = "123456";
        static string database = "tank";
        private static MySqlConnection con = null;
        private static Mutex mutex = new Mutex();
  
       
        [Obsolete]
        public static bool ConnectServer()
        {
            string connStr = "server=" + IP + ";port=" + port.ToString() + ";user=" + username + ";password=" + password + ";database=" + database+ ";Charset=utf8";
            try
            {
                if (con == null)
                {
                    con = new MySqlConnection(connStr);
                    con.Open();
                    return true;
                }
                if(con.State == ConnectionState.Open)
                {
                    return true;
                }
                if (con.State == ConnectionState.Broken)
                {
                    con.Close();
                    con.Open();
                    return true;
                }
                if (con.State == ConnectionState.Closed)
                {
                    con.Open();
                    return true;
                }
                return true;
            }
            catch(Exception e)
            {
                Log.AddLog(e.Message);
                return false;
            }
        }

        [Obsolete]
        public static bool CloseServer()
        {
            try
            {
                if (con != null)
                    con.Close();
                con = null;
                return true;
            }
            catch(Exception e)
            {
                Log.AddLog(e.Message);
                con = null;
                return false;
            }
        }

        [Obsolete]
        public static int ExecuteProcedure(string pro_name, Param param)
        {
            int result = -1;
            mutex.WaitOne();
            if (!ConnectServer())
                return -1;
            List<MySqlParameter> parameters = param.getLists();
            MySqlCommand cmd = new MySqlCommand(pro_name, con);
            cmd.CommandTimeout = 0;
            cmd.CommandType = CommandType.StoredProcedure;
            if (parameters == null) parameters = new List<MySqlParameter>();
            
            foreach (MySqlParameter para in parameters)
            {
                cmd.Parameters.Add(para);
            }
            try
            {
                MySqlDataReader reader = cmd.ExecuteReader(CommandBehavior.SingleRow);
                if (reader.HasRows)
                {
                    result = reader.GetInt16(0);
                }
                cmd.Dispose();
                reader.Close();
            }catch(Exception e)
            {
                Log.AddLog(e.Message+e.StackTrace);
                Log.AddLog("捕获到异常:"+e.StackTrace);
            }
            mutex.ReleaseMutex();
            return result;
        }
    }
}
