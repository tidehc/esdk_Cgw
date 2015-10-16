/*-----------------------------------------------------------------------
//文件名：T28181PTZCmd.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：PTZ指令,返回八个字节的十六进制字符串，详细参考T28181协议
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CgwMonitorManage.Common;

namespace CgwMonitorManage.T28181
{
    /*
    字节1：指令的首字节，本协议中，下列命令都以a5h开始。
    字节2：目的地址码，目的地址码是被控设备的编号，用一个字节表示。在控制过程中，协议中的目的地址必须与被控设备的物理地址一致。
           地址范围00h—ffh(即0—255)，ffh地址作为广播地址。当地址数目大于255时，应使用a6指令格式进行地址映射(详见附录)。
    字节3: 地址的低8位
	字节4: Bit7	Bit6	Bit5	Bit4	Bit3	Bit2	Bit1	Bit0
    	   0	 0	镜头变倍(Zoom)	垂直方向控制(Tilt)	水平方向控制(Pan)
			        缩小(OUT)/放大(IN)	上(Up)/下(Down)	左(Left)/右(Right)
    字节5: 水平控制速度相对值
    字节6: 垂直控制速度相对值
    字节7: 变倍控制速度相对值	地址高4位

    注1：字节4 中的Bit5、Bit4 分别控制镜头变倍的缩小和放大，字节4 中的Bit3、Bit2、Bit1、Bit0位分别控制云台上、下、左、右方向的转动，
    相应Bit 位置1 时，启动云台向相应方向转动，相应Bit位清0 时，停止云台相应方向的转动。云台的转动方向以监视器显示图像的移动方向为准。
    注2：字节5 控制水平方向速度，速度范围由慢到快为00H-FFH；字节6 控制垂直方向速度，速度范围由慢到快为00H-FFH。
    注3：字节7 的高4 位为变焦速度，速度范围由慢到快为0H-FH；低4 位为地址的高4 位
    */
    public class T28181PTZCmd
    {
        //PTZCmd指令字节1
        int byte1;
        public int Byte1
        {
            //指令首字节为0xA5
            get { return 0xA5; }
            set { byte1 = value; }
        }
        //PTZCmd指令字节2,高四位
        int byte2_1;
        public int Byte2_1
        {
            //高 4 位是版本信息，版本信息问0x0
            get { return 0x0; }
            set { byte2_1 = value; }
        }
        //PTZCmd指令字节2,低四位
        int byte2_2;
        public int Byte2_2
        {
            //校验位 =（字节 （字节 1 的高 4 位+字节 1 的低 4 位+字节 2 的高 4 位） % 16 ；
            get { return ((Byte1 & 0xF0) + (Byte1 & 0xF) + (Byte2_1 & 0xF0)) % 16; }
            set { byte2_2 = value; }
        }
        //PTZCmd指令字节2
        int byte2;
        public int Byte2
        {
            get { return Byte2_1 << 4 | Byte2_2; }
            set { byte2 = value; }
        }

        //PTZCmd指令字节3
        int byte3;
        public int Byte3
        {
            //地址的低8
            get { return 0x01; }
            set { byte3 = value; }
        }

        //PTZCmd指令字节4
        int byte4;
        public int Byte4
        {
            get { return byte4; }
            set { byte4 = value; }
        }

        //PTZCmd指令字节5
        int byte5;
        public int Byte5
        {
            get { return byte5; }
            set { byte5 = value; }
        }

        //PTZCmd指令字节6
        int byte6;
        public int Byte6
        {
            get { return byte6; }
            set { byte6 = value; }
        }

        //PTZCmd指令字节7
        int byte7;
        public int Byte7
        {
            get { return byte7; }
            set { byte7 = value; }
        }

        //PTZCmd指令字节8
      //（字节 1+ 字节 2+ 字节 3+ 字节 4+ 字节 5+ 字节 6+ 字节 7）% 256
        int byte8;
        public int Byte8
        {
            get { return (Byte1 + Byte2 + Byte3 + Byte4 + Byte5 + Byte6 + Byte7) % 256; }
            set { byte8 = value; }
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="ptzCommand">命令类型</param>
        /// <param name="param">命令参数（速度、倍数）</param>
        public T28181PTZCmd(PtzCommandType ptzCommand, int param)
        {
            //速度为0，表示停止云台控制
            if (param ==0)
            {
                Byte4 = 0x0;
                return;
            }
            //设置PTZ命令类型和参数,设置字节5、字节6、字节7的十六进制值，把速度参数转化为T28181协议的数值范围 
            switch (ptzCommand)
            {
                case PtzCommandType.PTZ_CMD_DOWN:
                    Byte4 = 0x4;
                    Byte6 = Convert.ToInt32(param.ToString("X2")) * 255 / 10;//字节6控制水平方向速度，速度范围由慢到快为00H-FFH
                    break;
                case PtzCommandType.PTZ_CMD_UP:
                    Byte4 = 0x8;
                    Byte6 = Convert.ToInt32(param.ToString("X2")) * 255 / 10;
                    break;
                case PtzCommandType.PTZ_CMD_FOCUS_IN:
                case PtzCommandType.PTZ_CMD_ZOOM_IN:
                    Byte4 = 0x10;
                    Byte7 = (Convert.ToInt32(param.ToString("X2")) * 15 / 10) << 4;//字节7 的高4 位为变焦速度，速度范围由慢到快为0H-FH
                    break;

                case PtzCommandType.PTZ_CMD_FOCUS_OUT:
                case PtzCommandType.PTZ_CMD_ZOOM_OUT:
                    Byte4 = 0x20;
                    Byte7 = (Convert.ToInt32(param.ToString("X2")) * 15 / 10) << 4;
                    break;
                case PtzCommandType.PTZ_CMD_LEFT:
                    Byte4 = 0x2;
                    Byte5 = Convert.ToInt32(param.ToString("X2")) * 255 / 10;//字节5 控制水平方向速度，速度范围由慢到快为00H-FFH
                    break;
                case PtzCommandType.PTZ_CMD_RIGHT:
                    Byte4 = 0x1;
                    Byte5 = Convert.ToInt32(param.ToString("X2")) * 255 / 10;
                    break;
            }
        }

        /// <summary>
        /// 转为T28181云台控制命令
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return Byte1.ToString("X2") + Byte2.ToString("X2") + Byte3.ToString("X2") + Byte4.ToString("X2") +
                   Byte5.ToString("X2") + Byte6.ToString("X2") + Byte7.ToString("X2") + Byte8.ToString("X2");
        }
    }
}
