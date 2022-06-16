#pragma once


int Mul(int a, int b);

template<typename T, int iSize>
void Safe_Delete_Array(T* (&_pArr)[iSize])
{
	for (int i = 0; i < iSize; ++i)
	{
		SAFE_DELETE(_pArr[i]);
	}
}

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	//typename map<T1, T2>::iterator iter = _map.begin();
	//
	//for (; iter != _map.end(); ++iter)
	//{
	//	SAFE_DELETE(iter->second)
	//}
	//_map.clear();

	for (auto& pair : _map)
	{
		SAFE_DELETE(pair.second);
	}
	_map.clear();
}

template<typename T>
void Safe_Delete_Vec(vector<T>& _vec)
{
	for (UINT i = 0; i < _vec.size(); ++i)
	{
		SAFE_DELETE(_vec[i]);
	}
}

void SaveWString(const wchar_t* _pStr, FILE * _pFile);
wchar_t* LoadWString(FILE* _pFile);