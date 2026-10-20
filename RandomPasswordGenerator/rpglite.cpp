#include <iostream>
#include <cstdlib>
#include <sstream>
using namespace std;
int main(){
string a,f;
ostringstream b;
int c,d,e,g;
cin>>f;
if(f=="t")srand(time(0));
else cin>>g;srand(g);
for(c=0;c<62;c++){
if(c<10)b<<char(c+48);
if(c<36&&c>10)b<<char(c+55);
if(c>36)b<<char(c+61);}a=b.str();
for(c=0;c<10;c++){
for(d=0;d<16;d++){
e=rand()%a.length();
cout<<a[e];}cout<<endl;
}cout<<"---"<<time(0)<<"---";
}