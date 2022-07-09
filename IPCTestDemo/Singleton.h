#pragma once

template <class T>
class CSingleton
{
public:
	CSingleton() {}
	~CSingleton() {}

	static T &GetInstance()
	{
		static T instance;
		return instance;
	}
};

