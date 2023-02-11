using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace NfqApp
{
    public class Logger
    {
        public Logger()
        {
        }

        public void Write(string folderPath, string logMessage)
        {
            try
            {
                using (StreamWriter w = File.AppendText($"{folderPath}\\performanceLog.txt"))
                {
                    WriteToTextWritter(logMessage, w);
                }
            }
            catch (Exception)
            {
            }
        }

        private void WriteToTextWritter(string logMessage, TextWriter txtWriter)
        {
            try
            {
                txtWriter.Write("\r\nLog Entry : ");
                txtWriter.WriteLine("{0} {1}", DateTime.Now.ToLongTimeString(),
                    DateTime.Now.ToLongDateString());
                txtWriter.WriteLine("  :");
                txtWriter.WriteLine("  :{0}", logMessage);
                txtWriter.WriteLine("-------------------------------");
            }
            catch (Exception ex)
            {
            }
        }

    }
}
