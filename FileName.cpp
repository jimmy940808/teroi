#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

void woodHurse(string FilePath)
{
	ifstream inputFile(FilePath, ios::binary);

	string outputFilePath(FilePath);

	//得到相對路徑
	size_t num = outputFilePath.find_last_of("\\");
	if (num == string::npos)
	{
		cout << "相對路徑獲取不到";
			return;
	}
	outputFilePath = outputFilePath.erase(num + 1);

	//得到副檔名
	string getPoint(FilePath);
	num = getPoint.find_last_of(".");
	if (num == string::npos)
	{
		cout << "副檔名獲取不到";
		return;
	}
	getPoint = getPoint.substr(num);
	
	//輸出檔名
	outputFilePath = strcat((char*)outputFilePath.c_str(), "temp");
	outputFilePath = strcat((char*)outputFilePath.c_str(), getPoint.c_str());
	ofstream outputFile(outputFilePath, ios::binary);

	if (inputFile.is_open())
	{
		char ch = 0;
		while (inputFile.get(ch))
		{
			ch ^= 5;
			outputFile.put(ch);
		}
	}
    else
    {
        cout << "找不到文件";
        return;
    }

	inputFile.close();
	outputFile.close();

	remove(FilePath.c_str());
	rename(outputFilePath.c_str(), FilePath.c_str());
}

int ListDirectory(const char* Path, int Recursive)
{
    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;
    char FileName[MAX_PATH] = { 0 };
    int Ret = -1;

    strcpy(FileName, Path);
    strcat(FileName, "\\");
    strcat(FileName, "*.*");

    // 查找第一个文件
    hFind = FindFirstFile(FileName, &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error when list %s\n", Path);
        return Ret;
    }
    do
    {
        // 构造文件名
        strcpy(FileName, Path);
        strcat(FileName, "\\");
        strcat(FileName, FindFileData.cFileName);
        printf("%s\n", FileName);
        if(strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0)
            woodHurse(FileName);


        // 如果是递归查找，并且文件名不是.和..，并且文件是一个目录，那么执行递归操作
        if (Recursive != 0
            && strcmp(FindFileData.cFileName, ".") != 0
            && strcmp(FindFileData.cFileName, "..") != 0
            && FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            ListDirectory(FileName, Recursive);
        }
        // 查找下一个文件
        if (FindNextFile(hFind, &FindFileData) == FALSE)
        {
            // ERROR_NO_MORE_FILES 表示已经全部查找完成
            if (GetLastError() != ERROR_NO_MORE_FILES)
            {
                printf("Error when get next file in %s\n", Path);
            }
            else
            {
                Ret = 0;
            }
            break;
        }
    } while (TRUE);

    // 关闭句柄
    FindClose(hFind);
    return Ret;
}

int main()
{
	string path;
    cout << "FilePath:";
	getline(cin, path);

    ListDirectory(path.c_str(), 0);

	//woodHurse(path);
}