#include <string.h>
#include <iostream>
using namespace std;

int main()
{
	int l1 = 10;
	int l2 = 5;
	for(int i = 0; i < l1; i++)
	{
		cout << "l1:" << i << endl;
		for(int i = 0; i < l2; i++)
		{
			cout << "l2:" << i << endl;
		}
	}
}
