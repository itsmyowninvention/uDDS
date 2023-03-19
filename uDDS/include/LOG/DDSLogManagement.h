#ifndef _DDS_LOG_H
#define _DDS_LOG_H
#include <set>
using namespace std;

#include "BasicTypes.h"
#include "uLinxDataType.h"
#include "ULINXAPI/uLinXSemMutex.h"
#include "ULINXAPI/uLinXThread.h"
#define WM_MATCH_LOG			0x1040
#define WM_DOMAINRELEASE_LOG	0x1050
#define WM_RECV_LOG				0x1060
#define WM_WRITE_LOG			0x1070
#define WM_ERROR_LOG			0x1080
#define WM_INFO_LOG				0x1090
#define WM_NEWLINE				0x0002		/* JD 换行 */
#define WM_BINARY				0x0001		/* JD 二进制 */
typedef GUID_t _GUID_T;
typedef struct  DDSLog_DATA_INFO
{
	char* m_pData;
	string m_strTime;
	int m_iLen;
	unsigned int m_iMode;
	_GUID_T m_Guid;
	DDSLog_DATA_INFO()
	{
		m_pData = NULL;
		m_strTime = "";
	}
	~DDSLog_DATA_INFO()
	{
		m_strTime = "";
		if (NULL != m_pData)
		{
			delete m_pData;
			m_pData = NULL;
		}
	}
}_DDSLOG_DATA_INFO_T;

typedef set<string> STRING_VEC;

class  DDSLogManagement
{
public:
	/* 构造函数 */
	DDSLogManagement();

	//DDSLogManagement(const char* pFilePath,const char* pCompName);

	DDSLogManagement(const _DOMAINID_T& uiDomainId, const unsigned int& uiFileSize, const unsigned int& uiTotalSize,
		const char* pCompName, const char* pFilePath);

	/* 设置DDSLogManagement的各种参数 */
	int SetDDSLogManagement(const _DOMAINID_T uiDomainId, const unsigned int uiFileSize, const unsigned int uiTotalSize,
		const char* pCompName, const char* pFilePath);
	int SetUseDDSLogFlag(bool bUseDDSLog);
	//DDSLogManagement(){};
	~DDSLogManagement();

	/* 调用日志文件接口，日志文件*/
	int WriteDDSLog(char* pData, const int len, const int Mod, _GUID_T Guid = GUID_UNKNOWN, int iDomainCount = 0);

	int GetDDSLogDataVec(vector<_DDSLOG_DATA_INFO_T*>& DDSLogDataVec);

protected:
private:
	/* 处理消息队列 */
#ifdef ULINX_WIN
	static DWORD WINAPI DDSLogProcessRun(LPVOID pDDSLogManagement);
	/* 线程句柄 */
	HANDLE m_HTask;

	/* 线程ID */
	DWORD	m_HTaskID;
#elif defined ULINX_LINUX
	static void* DDSLogProcessRun(void* pDDSLogManagement);
	pthread_t m_HTaskID;
#elif defined ULINX_VX
#endif
	/* 写入日志文件 */
	int WriteToLogFile(const string& strLogInfo, int len, unsigned int Mod);

	/* 处理数据 ，将Vec的数据放入 string中 */
	int ProcessData();

	/* 启动日志记录线程 */
	void	StartDDSLogTask();

	/* 关闭日志线程 */
	void	StopDDSLogTask();

	/*  获取系统时间 */
	int GetSysTime(string& strTime);

	/* 獲取文件大小　*/
	long getFileSize(FILE* log);

	int WriteFile(int Mod);
private:
	/* 需要记录日志内容的总大小 */
	unsigned int m_uiStrTotalLen;

	/* 日志文件总大小 */
	unsigned int m_uiTotalSize;

	/* 单个日志文件大小 */
	unsigned int m_uiFileSize;

	/* 单种文件的大小 */
	unsigned int m_uiLogFileNum;

	/* 记录的个数 */
	unsigned int m_uiMsgCount;

	/* 日志文件路径 */
	string m_strFilePath;

	/* 应用的名称 / 组件名 */
	string m_strCompName;

	/* 日志文件所记录的数据域 */
	unsigned int m_uiDomainID;

	/* 是否值录日志的标志位 默认位false */
	bool	m_bUseDDSLog;

	/* 线程开始退出标识 */
	bool	m_bBeginQuit;

	/* 线程完成退出标识 */
	bool    m_bEndQuit;

	/* 判断是否到记录时间*/
	bool	m_bWriteFile;

	/* 上次记录日志的时间 */
	DWORD m_CurrentTick;

	/* 存放MatchLog的信息 */
	string m_strMatchLog;

	/* 存放RecvLog的信息 */
	string m_strRecvLog;

	/* 存放WriteLog的信息 */
	string m_strWriteLog;

	/* 存放InfoLog的信息 */
	string m_strInfoLog;

	/* 存放ErrorLog的信息 */
	string m_strErrorLog;

	/* 存放DomainRelease 的信息 */
	string m_strDomainReleaseLog;
	
	///* Topic 信息记录标志位 */
	/* 保护日志记录的序号 */
	uLinX_SemMutex m_SeqMutex;
	/* 保护写日志的内容 */
	uLinX_SemMutex m_MsgMutex;
	/* 线程 */
	uLinXThread m_Thread;

	vector<_DDSLOG_DATA_INFO_T*>  m_DDSLogDataVec;


}; // class DDSLogManagement

class DDS_DLL DDSLogManagementFactory {
public:
	DDSLogManagementFactory();
	~DDSLogManagementFactory();
	static DDSLogManagementFactory* GetInstance();
	static int EntityToString(EntityId_t EntityId, string& strEntity);
	static int GuidPrefixToString(guidPrefix_t,string& strGuidPrefix);
	/* 创建数据域相关的日志管理对象 */
	DDSLogManagement* CreateDDSManagement(int iDomainCount);

	/* 删除数据域相关的日志管理对象 */
	int RemoveDDSManagement(int iDomainCount);

	bool GetUseDDSLogFlag(int iDomainCount);
	/* 判断是否记录Topic true表示记录*/
	bool IsWriteTopic(int iDomainCount, const string& TopicName);
	/* */
	bool IsReadTopic(int iDomainCount, const string& TopicName);
	/* */
	bool IsWriteALLTopic(int iDomainCount, const string& TopicName);
	/* */
	bool IsReadALLTopic(int iDomainCount, const string& TopicName);
	/* */
	bool IsQueryTopic(int iDomainCount, const string& TopicName);

	static int WriteDDSLog(int iDomainCount, char* pData, const size_t len, const int Mod, _GUID_T Guid = GUID_UNKNOWN);
private:
	/* 获取数据域相关的日志管理对象 */
	/* JD 防止其他地方调用 DDSLogManagement对象的成员 没有进行判空的操作，这边改为私有 */
	DDSLogManagement* GetDDSManagement(int iDomainCount);

	static DDSLogManagementFactory* m_pInstance;

	map<_DOMAINID_T, DDSLogManagement*> m_DDSLogManagementMap;

	DDSLogManagement* m_pDDSLogManagement;

	/* 需要打开日志记录数据域 */
	set<int> m_DomainIdVec;
	/* 日志文件的目录 */
	string	m_strFilePath;
	string  m_strCompName;

	bool	m_bUseDDSLog;
	/* Topic 信息记录标志位 */
	bool	m_bWriteFlag;
	bool	m_bWriteALLFlag;
	bool	m_bReadFlag;
	bool	m_bReadALLFlag;
	bool	m_bQueryALLFlag;
	/* 判断是否记录发送到网络上的数据 */
	bool	m_bSocketFlag;

	int		m_iFileSize;
	int		m_iTotalSize;

	/* 存放WriteTopic的Vec */
	STRING_VEC m_WriteTopicVec;

	/* 存放WriteTopic的Vec */
	STRING_VEC m_ReadTopicVec;

	/* 存放WriteTopic的Vec */
	STRING_VEC m_WriteALLTopicVec;

	/* 存放WriteTopic的Vec */
	STRING_VEC m_ReadTALLopicVec;

	/* 存放QueryTopic的Vec */
	STRING_VEC m_QueryTopicVec;
private:
	/* 读取日志的配置文件配置文件 */
	int LogConfig();
	/* 获取进程名 */
	int GetProcessName(string& ProcessName);
	/* 获取进程Id */
	unsigned long DDSGetProcessId();

	uLinX_SemMutex m_DDSLogManagementMutex;
};


#endif //_DDS_LOG_H
