using System;
using System.Runtime.CompilerServices;

namespace Luna
{
    static public class Os
    {
        /// <summary>
        /// opens a platform specific openFIleDialog.
        /// <example>
        /// <code>
        /// string openFilePath = OpenFileDialog("image\0*.png;*.jpeg;*.jpg\0");
        /// </code>
        /// </example>
        /// </summary>
        /// <param name="filter"></param>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string OpenFileDialog(string filter);

        /// <summary>
        /// opens a platform specific saveFIleDialog.
        /// <example>
        /// <code>
        /// string saveFilePath = SaveFileDialog("image\0*.png;*.jpeg;*.jpg\0");
        /// </code>
        /// </example>
        /// </summary>
        /// <param name="filter"></param>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string SaveFileDialog(string filter);
        /// <summary>
        /// returns the current osName.
        /// </summary>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string GetName();
    }
}
