
using System.Runtime.CompilerServices;

namespace Luna
{
    public class Log
    {
        /// <summary>
        /// app trace log to cli and logfile
        /// 
        /// <example>
        /// <code>
        /// string name = "John Doe";
        /// int age = 26;
        /// Log.Trace("Hi, my name is {0}. I am {1} years old", name, age);
        /// </code>
        /// </example>
        /// </summary>
        /// <param name="text"></param>
        /// <param name="args"></param>
        public static void Trace(string text, params dynamic[] args) { TraceInternal(string.Format(text, args)); }

        /// <summary>
        /// app Info log to cli and logfile
        /// 
        /// <example>
        /// <code>
        /// string name = "John Doe";
        /// int age = 26;
        /// Log.Info("Hi, my name is {0}. I am {1} years old", name, age);
        /// </code>
        /// </example>
        /// </summary>
        /// <param name="text"></param>
        /// <param name="args"></param>
        public static void Info(string text, params dynamic[] args) { InfoInternal(string.Format(text, args)); }

        /// <summary>
        /// app Warn log to cli and logfile
        /// 
        /// <example>
        /// <code>
        /// string name = "John Doe";
        /// int age = 26;
        /// Log.Warn("Hi, my name is {0}. I am {1} years old", name, age);
        /// </code>
        /// </example>
        /// </summary>
        /// <param name="text"></param>
        /// <param name="args"></param>
        public static void Warn(string text, params dynamic[] args) { WarnInternal(string.Format(text, args)); }

        /// <summary>
        /// app Error log to cli and logfile
        /// 
        /// <example>
        /// <code>
        /// string name = "John Doe";
        /// int age = 26;
        /// Log.Error("Hi, my name is {0}. I am {1} years old", name, age);
        /// </code>
        /// </example>
        /// </summary>
        /// <param name="text"></param>
        /// <param name="args"></param>
        public static void Error(string text, params dynamic[] args) { ErrorInternal(string.Format(text, args)); }

        /// <summary>
        /// app Critical log to cli and logfile
        /// 
        /// <example>
        /// <code>
        /// string name = "John Doe";
        /// int age = 26;
        /// Log.Critical("Hi, my name is {0}. I am {1} years old", name, age);
        /// </code>
        /// </example>
        /// </summary>
        /// <param name="text"></param>
        /// <param name="args"></param>
        public static void Critical(string text, params dynamic[] args) { CriticalInternal(string.Format(text, args)); }

        /*internal calls*/
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void TraceInternal(string text);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void InfoInternal(string text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void WarnInternal(string text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void ErrorInternal(string text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void CriticalInternal(string text);

    }
}
