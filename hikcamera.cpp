#include "hikcamera.h"
#include <QDebug>
#include <vector>
#include <chrono>

hikcamera::hikcamera()
{

}

hikcamera::~hikcamera()
{

}

hikcamera *hikcamera::getInstance()
{
    static hikcamera* instance = new hikcamera();
    return instance;
}

int hikcamera::initialize()
{
    if (isInit) // 如果已经初始化，直接返回
    {
        //qDebug() << u8"相机已初始化，直接返回";
        return -1;
    }

    // 初始化海康相机SDK
    if(!NET_DVR_Init())
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        qDebug() << u8"相机初始化错误:" << errorMessage;
    }

    //设置连接时间与重连时间
    if(!NET_DVR_SetReconnect(10000, true))
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        qDebug() << u8"相机初始化错误:" << errorMessage;
    }

    qDebug() << u8"相机初始化成功";
    isInit = true;

    return 0;
}

int hikcamera::uninitialize()
{
    if (!isInit) // 如果未初始化，直接返回
    {
        qDebug() << u8"相机未初始化，直接返回";
        return 1;
    }

    //相机反初始化
    if(!NET_DVR_Cleanup())
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        qDebug() << u8"相机反初始化错误:" << errorMessage;
    }

    qDebug() << u8"相机反初始化";
    isInit = false;

    return 0;
}

bool hikcamera::isInitialized()
{
    return isInit;
}

int hikcamera::connect()
{
    //qDebug()<<QSslSocket::sslLibraryBuildVersionString();
    if (!isInit)
    {
        qDebug() << u8"相机未初始化";
        return -1;
    }

    if (!(lUserID < 0)) //保证目前没有设备被打开
    {
        qDebug() << u8"已有设备被打开";
        return -1;
    }

    // 设置连接相机的信息
    struLoginInfo.bUseAsynLogin = 0; //同步登录方式
    strcpy_s(struLoginInfo.sDeviceAddress, "192.168.1.13"); //设备IP地址
    struLoginInfo.wPort = 8000; //设备服务端口
    strcpy_s(struLoginInfo.sUserName, "admin"); //设备登录用户名
    strcpy_s(struLoginInfo.sPassword, "zhy19991117"); //设备登录密码

    //设备信息, 输出参数
    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    qDebug()<<"lUSerID "<< lUserID;
    if (lUserID < 0)
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        qDebug()<< "NET_DVR_LOgin_V40 ErrorNo :"<<ErrorNo;
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        qDebug() << u8"连接相机失败:" << errorMessage;
        NET_DVR_Cleanup();
        return -1;
    }

    isConn = true;
    qDebug() << u8"相机连接成功";

    if (!NET_DVR_SetCapturePictureMode(BMP_MODE))
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        qDebug() << u8"设置抓图模式失败:" << errorMessage;
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return -1;
    }

    return 0;
}

int hikcamera::disconnect()
{
    if (!isInit)
    {
        //qDebug() << u8"相机未初始化";
        return -1;
    }

    //相机断开连接
    if(!NET_DVR_Logout(lUserID))
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        //qDebug() << u8"相机反初始化错误:" << errorMessage;
    }
    lUserID = -1;
    isConn = false;
    //qDebug() << u8"相机断开成功";
    return 0;
}

bool hikcamera::isConnected()
{
    return isConn;
}

int hikcamera::startStreaming(HWND hPlayWnd)
{
    if (!isConn)
    {
        qDebug() << u8"相机未连接";
        return -1;
    }

    struPlayInfo.hPlayWnd = hPlayWnd; //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
    struPlayInfo.lChannel = 1; //预览通道号
    struPlayInfo.dwStreamType = 0; //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode = 0; //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked = 1; //0- 非阻塞取流，1- 阻塞取流

    lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL); //海康摄像头实时回放设置
    if (lRealPlayHandle < 0)
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        qDebug() << u8"相机开始取流失败:" << errorMessage;
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return -1;
    }

    qDebug() << u8"相机开始拉流";
    isRunning = true;

    return 0;
}

int hikcamera::stopStreaming()
{
    if (!isConn)
    {
        //qDebug() << u8"相机未连接";
        return -1;
    }
    int nRet = 0;

    if (NET_DVR_StopRealPlay(lRealPlayHandle) < 0)
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        //qDebug() << u8"相机结束取流失败:" << errorMessage;
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return -1;
    }

    if (0 != nRet)
    {
        //qDebug() << u8"视频流停止拉流失败";
        return -1;
    }

    isRunning = false;

    return 0;
}

int hikcamera::startGrabbing(cv::Mat &srcImg)
{
    std::vector<char> inputData(2560*1440*4 + 54);
    DWORD size= 0;
    DWORD* psize = &size;

    if (!NET_DVR_CapturePictureBlock_New(lRealPlayHandle, inputData.data(), (DWORD)inputData.size(), psize))
    {
        LONG ErrorNo = (LONG) NET_DVR_GetLastError();
        char* errorMessage = NET_DVR_GetErrorMsg(&ErrorNo);
        qDebug() << u8"抓取一张图像失败:" << errorMessage;
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();
        return -1;
    }

    //qDebug() << size;
    cv::_InputArray pic_arr(inputData);
    srcImg = cv::imdecode(pic_arr, cv::IMREAD_COLOR);
    return 0;
}


