/*-----------------------------------------------------------------------
//文件名：eLTEDataStruct.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：eLTE数据类型 
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CgwMonitorManage.Common
{
    public struct eLTEVideoParameter
    {
        public string fmtvalue;       //视频格式，取值为“D1”、“CIF”、“QCIF”、“1080P”、“720P”
        public string cameratype;     //“0”表示前置； “1”表示后置
        public string user_confirm_type; //“0”表示不需要用户确认；“1”表示需要用户确认
        public string mutetype;          //“0”表示需要伴音；“1”表示不需要伴音
    }

    public class eLTEUserInfo
    {
        public int userid;
        public eLTEUser_Category_t usercategory; //包括PTTuser， Camera, FixedLine, etc
        public int userpriority;
        public string username;
        public int vpnid;
        public int vpnout;
        public int vpnin;
        public eLTEUser_Location_t location;
    }
    public enum eLTEGroup_Category_t
    {
        GRP_ALLBROADCAST = 0,      //全网广播组
        GRP_GENERAL = 1,        //普通组；
        GRP_AREABROADCAST = 2,       //  区域广播组；
        GRP_EXTERNAL = 8,         //外部组，用在分级场景
        GRP_DYNAMICGRP = 9,      //动态组
        GRP_ALLTYPE = 10
    };

    public enum eLTEUser_Category_t
    {
        DISPATCHER = 0,      //Dispatcher 调度台用户
        FIXEDCAMERA = 1,    //固定摄像头；
        PSTNUSER = 2,       //  PSTN用户；
        TETRAUSER = 3,      // TETRA用户；
        PLMNUSER = 4,       //  PLMN移动用户；
        EXTERNALPTT = 5,    // 外部PTT用户
        DECUSER = 6,         //DEC用户
        PTTUSER = 9,         //PTT用户
        ALLTYPEUSER = 10,     // 所有用户，不分类型
        OTHERUSER = 255       // 其它未分类用户"
    };
    public enum eLTEUser_Location_t
    {
        LOCAL_USER = 0,      //本调度机用户
        EXTERNAL_USER = 1   //外部调度机用户；
    };
    public class eLTEGroupInfo
    {
        public int grpid;
        public eLTEGroup_Category_t grpcategory; //全网， 区域， 静态， 动态， etc
        public int grppriority;
        public string grpname;
        public int grpstate;    //0=disable, 1=enable
        public int vpnid;
        public int setupDcId;    //creator, valid if it's dynamic or temp group
    };

    public enum eLTEGroup_Member_t
    {
        MEMBER_GROUP = 0,
        MEMBER_USER = 1

    };
    public class eLTEGrpUserInfo
    {
        public int userid;                      //根据membertype，可能为静态用户或组 ID
        public int grpid;                       //可能为静态组或动态组
        public int userpriorityInGrp;
        public eLTEGroup_Member_t memberType;         //1=普通用户成员， 0=动态重组中的组成员

    };
}
