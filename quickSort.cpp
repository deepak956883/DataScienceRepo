#include<iostream>
using namespace std;

void interchange(int eleArray[] , int Location1, int Location2){
	int tempVar;
	tempVar = eleArray[Location1];
	eleArray[Location1] = eleArray[Location2];
	eleArray[Location2] = tempVar;
}

int eleArrayPartition (int eleArray[], int min, int max, int targetElement){
	int x = min;
	int y = min;
	while( x <= max){
		if(eleArray[x] > targetElement){
			x++;
		}
		else{
			interchange(eleArray,x,y);
			x++;
			y++;
		}
	}
	return y-1;
}

void AlgoQuickSort(int eleArray[], int min, int max){
	if(min < max){
	int targetElement = eleArray[max];
	int Location = eleArrayPartition(eleArray, min, max, targetElement);
	
	AlgoQuickSort(eleArray, min, Location-1);
	AlgoQuickSort(eleArray, Location+1, max);
	}
}


int main()
{
	int size ;
	cout << "Enter the size of Array : ";
	cin>>size;
    int eleArray[size];
    if(size <= 0){
       cout << "Empty Array !" << endl;
       exit(0);
    }
    else{
    cout << "Enter Array Elements" << endl;
	for( int i = 0 ; i < size; i++){
		cin>> eleArray[i];
	 }
    }
    
    cout<<"Input array is      : ";
    for( int i = 0 ; i < size; i++){
		cout << eleArray[i] << " ";
	 }
    cout << "\n";
	AlgoQuickSort(eleArray, 0 , size-1);
	
	cout<<"QuickSort Result is : ";
	for( int i = 0 ; i < size; i++){
		cout<< eleArray[i]<<" ";
	}
}	


