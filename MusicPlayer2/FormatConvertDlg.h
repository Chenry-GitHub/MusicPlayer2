#pragma once
#include "ListCtrlEx.h"
#include "BASSEncodeLibrary.h"
#include "AudioTag.h"
#include "MP3EncodeCfgDlg.h"
#include "OggEncodeCfgDlg.h"
#include "BASSWmaLibrary.h"
#include "WmaEncodeCfgDlg.h"

// CFormatConvertDlg 对话框

//单个文件转换进度的消息
//wParam: 文件在m_file_list列表中的索引
//lParam: 转换的进度。0~100: 已完成的百分比; 101: 已完成; <0:出错 (-1: 源文件无法打开; -2: 编码通道创建失败; -3:找不到编码器或编码器参数错误)
#define WM_CONVERT_PROGRESS (WM_USER+111)
//全部文件转换已完成的消息
#define WM_CONVERT_COMPLETE (WM_USER+112)

#define ALBUM_COVER_NAME_ENCODE L"TempAlbumCover-MusicPlayer2-CkV8E0YI"

class CFormatConvertDlg : public CDialog
{
	DECLARE_DYNAMIC(CFormatConvertDlg)

public:
	enum class EncodeFormat { WAV, MP3, WMA, OGG };

	CFormatConvertDlg(CWnd* pParent = nullptr);   // 标准构造函数
	CFormatConvertDlg(const vector<int>& items_selected, CWnd* pParent = nullptr);
	virtual ~CFormatConvertDlg();

	//工作线程函数
	static UINT ThreadFunc(LPVOID lpParam);


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMAT_CONVERT_DIALOG };
#endif

protected:
	//控制变量
	CListCtrlEx m_file_list_ctrl;
	CComboBox m_encode_format_combo;

	vector<wstring> m_file_list;		//要转换格式的文件列表
	wstring m_out_dir;		//输出目录
	EncodeFormat m_encode_format{ EncodeFormat::MP3 };
	CWinThread* m_pThread{};		//格式转换的线程
	wstring m_encode_dir;
	bool m_encoder_successed;

	MP3EncodePara m_mp3_encode_para;		//MP3编码参数
	int m_ogg_encode_quality{ 3 };				//OGG编码质量
	int m_wma_encode_bitrate{ 80 };			//wma编码比特率（单位kbps）

	static CBASSEncodeLibrary m_bass_encode_lib;
	static CBASSWmaLibrary m_bass_wma_lib;

	CSize m_min_size;

	//选项数据
	bool m_write_tag;			//向目标文件写入标签信息
	bool m_write_album_cover;	//向目标文件写入专辑封面
	int m_file_exist_action;	//当目标文件存在时的动作（0: 自动重命名; 1: 忽略; 2: 覆盖）
	bool m_add_file_serial_num;	//为目标文件添加数字编号

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void LoadConfig();
	void SaveConfig() const;
	void LoadEncoderConfig();
	void SaveEncoderConfig() const;

	void EnableControls(bool enable);
	void ShowFileList();
	bool InitEncoder();
	//编码单个文件（在线程函数中调用）
	//pthis: 当前对话框的指针
	//file_index: 编码的文件在m_file_list中的索引
	//out_file_path: 接收输出文件的路径
	//song_info: 包含歌曲的标签信息，用于写入转换后的文件
	static bool EncodeSingleFile(CFormatConvertDlg* pthis, int file_index, SongInfo& song_info);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeOutFormatCombo();
	afx_msg void OnBnClickedStartConvertButton();
	afx_msg void OnBnClickedBrowseButton();
protected:
	afx_msg LRESULT OnConvertProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnConvertComplete(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedEncoderConfigButton();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedCopyTagCheck();
	afx_msg void OnBnClickedCopyAlbumCoverCheck();
	afx_msg void OnCbnSelchangeTargetFileExistCombo();
	afx_msg void OnBnClickedAddNumberCheck();
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedClearButton();
	afx_msg void OnBnClickedMoveUpButton();
	afx_msg void OnBnClickedMoveDownButton();
};
