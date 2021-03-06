﻿/* 
 * Copyright (c) 2016-2017 Akitsugu Komiyama
 * under the Apache License Version 2.0
 */

using System;
using System.Runtime.InteropServices;


// ★秀丸クラス
public sealed partial class hmPSDynamicLib
{
    public sealed partial class Hidemaru
    {

        // 秀丸本体から出ている関数群
        delegate IntPtr TGetTotalTextUnicode();
        delegate IntPtr TGetLineTextUnicode(int nLineNo);
        delegate IntPtr TGetSelectedTextUnicode();
        delegate int TGetCursorPosUnicode(ref int pnLineNo, ref int pnColumn);

        /*
        [StructLayout(LayoutKind.Sequential)]
        struct POINT
        {
            public int x;
            public int y;
        }
        delegate int TGetCursorPosUnicodeFromMousePos(out POINT lpPoint, ref int pnLineNo, ref int pnColumn);
        */
        delegate int TGetCursorPosUnicodeFromMousePos(IntPtr lpPoint, ref int pnLineNo, ref int pnColumn);

        delegate int TEvalMacro([MarshalAs(UnmanagedType.LPWStr)] String pwsz);
        delegate int TCheckQueueStatus();

        // 秀丸本体から出ている関数群
        static TGetTotalTextUnicode pGetTotalTextUnicode;
        static TGetLineTextUnicode pGetLineTextUnicode;
        static TGetSelectedTextUnicode pGetSelectedTextUnicode;
        static TGetCursorPosUnicodeFromMousePos pGetCursorPosUnicodeFromMousePos;
        static TGetCursorPosUnicode pGetCursorPosUnicode;
        static TEvalMacro pEvalMacro;
        static TCheckQueueStatus pCheckQueueStatus;

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GlobalLock(IntPtr hMem);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern int GlobalUnlock(IntPtr hMem);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GlobalFree(IntPtr hMem);

        // 秀丸本体のexeを指すモジュールハンドル
        static UnManagedDll hmExeHandle;

        // 秀丸本体のExport関数を使えるようにポインタ設定。
        static void SetUnManagedDll()
        {
            // OutputDebugStream(version.ToString());
            if (version >= 866)
            {
                // 初めての代入のみ
                if (hmExeHandle == null)
                {
                    hmExeHandle = new UnManagedDll(strExecuteFullpath);

                    pGetTotalTextUnicode = hmExeHandle.GetProcDelegate<TGetTotalTextUnicode>("Hidemaru_GetTotalTextUnicode");
                    pGetLineTextUnicode = hmExeHandle.GetProcDelegate<TGetLineTextUnicode>("Hidemaru_GetLineTextUnicode");
                    pGetSelectedTextUnicode = hmExeHandle.GetProcDelegate<TGetSelectedTextUnicode>("Hidemaru_GetSelectedTextUnicode");
                    pGetCursorPosUnicode = hmExeHandle.GetProcDelegate<TGetCursorPosUnicode>("Hidemaru_GetCursorPosUnicode");
                    pEvalMacro = hmExeHandle.GetProcDelegate<TEvalMacro>("Hidemaru_EvalMacro");
                    pCheckQueueStatus = hmExeHandle.GetProcDelegate<TCheckQueueStatus>("Hidemaru_CheckQueueStatus");

                    if (_ver >= 873)
                    {
                        pGetCursorPosUnicodeFromMousePos = hmExeHandle.GetProcDelegate<TGetCursorPosUnicodeFromMousePos>("Hidemaru_GetCursorPosUnicodeFromMousePos");
                    }
                }
            }
        }

    }
}