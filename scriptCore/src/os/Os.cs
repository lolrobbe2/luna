using System.Runtime.CompilerServices;

namespace Luna
{
    public class Os
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
        /// <returns>string: open file path</returns>
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
        /// <returns>string: save file path</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string SaveFileDialog(string filter);

        /// <summary>
        /// returns the current working directory
        /// </summary>
        /// <returns>string: absolute path of current working directory </returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string GetCurrentWorkingDirectory();
        /// <summary>
        /// returns the current osName.
        /// </summary>
        /// <returns>string os name</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string GetName();

        /// <summary>
        /// returns the os version.
        /// </summary>
        /// <returns>MajorVersion.MinorVersion.Patch</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string GetVersion();

        /// <summary>
        /// returns the users locale 
        /// 
        /// </summary>
        /// <returns>string of format en-EN</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string GetLocale();
        /// <summary>
        /// returns the users current language.
        /// </summary>
        /// <returns>string of format en</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern string GetLocaleLanguage();
    }
}
