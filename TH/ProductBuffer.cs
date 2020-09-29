using System;
using System.Threading;

namespace TH
{
    class ProductBuffer
    {
        private int buffer_length;
        private int in_index = 0;
        private int out_index = 0;
        private Mutex mutex_in;
        private Mutex mutex_out;
        private Semaphore rest;
        private Semaphore used;
        private Object[] buffer;

        public ProductBuffer(int buffer_length)
        {
            this.buffer_length = buffer_length;
            this.buffer = new object[buffer_length];
            this.rest = new Semaphore(buffer_length, buffer_length);
            this.used = new Semaphore(0, buffer_length);
            this.mutex_in = new Mutex();
            this.mutex_out = new Mutex();
        }

        /// <summary>
        /// 添加一个产品，若缓冲区满，则无限等待至添加成功
        /// </summary>
        /// <param name="obj">要添加的产品</param>
        public void AddProduct(object obj)
        {
            this.mutex_in.WaitOne();
            this.rest.WaitOne();
            this.buffer[in_index] = obj;
            this.used.Release();
            in_index = (in_index + 1) % this.buffer_length;
            this.mutex_in.ReleaseMutex();
        }


        /// <summary>
        /// 尝试添加一个产品，在out_time时间后仍未添加成功则会失败返回
        /// </summary>
        /// <param name="obj">要添加的产品</param>
        /// <param name="out_time">超时时间，默认100毫秒</param>
        /// <returns>添加操作是否成功</returns>
        public bool TryAddProduct(object obj,int out_time=100)
        {
            bool result = false;
            if (this.mutex_in.WaitOne(out_time))
            {
                if (this.rest.WaitOne(out_time))
                {
                    this.buffer[in_index] = obj;
                    in_index = (in_index + 1) % this.buffer_length;
                    this.used.Release();
                    result = true;
                }
                this.mutex_in.ReleaseMutex();
            }
            return result;
        }


        /// <summary>
        /// 获取一个产品，若缓冲区无产品，则无线等待至获取到一个产品
        /// </summary>
        /// <returns>获取到的产品</returns>
        public object GetProduct()
        {
            Object result = null;
            this.mutex_out.WaitOne();
            this.used.WaitOne();
            
            result = this.buffer[out_index];
            this.rest.Release();
            this.out_index = (this.out_index + 1) % this.buffer_length;
            this.mutex_out.ReleaseMutex();
            return result;
        }


        /// <summary>
        /// 尝试获取一个产品，若在times毫秒后仍未获取到则返回null
        /// </summary>
        /// <param name="times">超时时间，默认100毫秒</param>
        /// <returns>获取到的产品，可能为空</returns>
        public object TryGetProduct(int times=100)
        {
            Object result = null;
            if (this.mutex_out.WaitOne(times))
            {
                if (this.used.WaitOne(times))
                {
                    result = this.buffer[out_index];
                    this.out_index = (this.out_index + 1) % this.buffer_length;
                    this.rest.Release();
                }
                this.mutex_out.ReleaseMutex();
            }
            return result;
        }
    }
}
