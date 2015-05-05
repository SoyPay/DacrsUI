#pragma once
using namespace std;
class CPostMsg
{
	int UI_TYPE;
    int DATA_TYPE;
	CString strType;
	CString strData;
public:

	CPostMsg(int uitype,int datatype);
	CPostMsg(void);
	~CPostMsg(void);
	void SetStrType(CString const &strTpye);
	void SetType(int uitype,int datatype);
	int GetUItype() const;
	int GetDatatype() const;
	void SetData(CString const & data){
		strData = data;
	}
	CString GetData(){
		return strData;
	}
	CString GetStrType() const;
public:
	 bool operator==(CPostMsg const&meg)const;
};

