#ifndef __QMEM_H_
#define __QMEM_H_




template <typename T>
static void QMem_SafeDelete( T*& obj )
{
	if(obj)
	{
		delete obj;
		obj = 0;
	}
}


template <typename T>
static void QMem_SafeDeleteArray( T*& obj )
{
	if(obj)
	{
		delete[] obj;
		obj = 0;
	}
}



#endif