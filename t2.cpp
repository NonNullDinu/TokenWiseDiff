#include<bits/stdc++.h>
using namespace std;

ifstream in("base_converter.in");
ofstream out("base_converter.out");

int mod(string num, int a)
{
    int res = 0;
    for (int i = 0; i < num.length(); i++)
         res = (res*10 + (int)num[i] - '0') %a;
    return res;
}

int mic(string str1,string str2)
{
    int n1=str1.length(),n2=str2.length();
    if(n1<n2)
        return true;
    if(n2>n1)
        return false;

    for(int i=0;i<n1;i++)
    {
        if(str1[i]<str2[i])
            return true;
        else
        if(str1[i]>str2[i])
            return false;
    }
    return false;
}


string dx(string str1,string str2)
{
    if(mic(str1,str2))
        swap(str1, str2);
    string str = "";
    int n1=str1.length(),n2=str2.length();
    int dif=n1-n2;
    int t=0;
    for(int i=n2-1;i>=0;i--)
    {
        int sub=((str1[i+dif]-'0')-(str2[i]-'0')-t);
        if(sub<0)
            sub+=10,t=1;
        else
            t=0;
        str.push_back(sub+'0');
    }
    for(int i=n1-n2-1;i>=0;i--)
    {
        if(str1[i]=='0'&&t)
        {
            str.push_back('9');
            continue;
        }
        int sub=((str1[i]-'0')-t);
        if (i>0||sub>0)
            str.push_back(sub+'0');
        t=0;
    }
    reverse(str.begin(),str.end());
    return str;
}

string sx(string str1,string str2)
{
    int t=0;
    if (str1.length()>str2.length())
        swap(str1,str2);
    string str="";
    int n1=str1.length(),n2=str2.length();
    int dif=n2-n1;
    for(int i=n1-1;i>=0;i--)
    {
        int sum=((str1[i]-'0')+(str2[i+dif]-'0')+t);
        str.push_back(sum%10+'0');
        t=sum/10;
    }
    for(int i=n2-n1-1;i>=0;i--)
    {
        int sum=((str2[i]-'0')+t);
        str.push_back(sum%10+'0');
        t=sum/10;
    }
    if(t)
        str.push_back(t+'0');
    reverse(str.begin(),str.end());
    return str;
}

string px(string num1, string num2)
{
    int n1=num1.size();
    int n2=num2.size();
    if(n1==0||n2==0)
       return "0";
    vector<int> rez(n1+n2,0);
    int i_n1=0;
    int i_n2=0;
    for(int i=n1-1;i>=0;i--)
    {
        int t=0;
        int n1=num1[i]-'0';
        i_n2=0;
        for(int j=n2-1;j>=0;j--)
        {
            int n2=num2[j]-'0';
            int sum=n1*n2+rez[i_n1+i_n2]+t;
            t=sum/10;
            rez[i_n1+i_n2]=sum%10;
            i_n2++;
        }
        if(t>0)
            rez[i_n1+i_n2]+=t;
        i_n1++;
    }
    int i=rez.size()-1;
    while(i>=0 && rez[i]==0)
       i--;
    if(i==-1)
       return "0";
    string s="";
    while(i>=0)
        s+=to_string(rez[i--]);
    return s;
}

string ddx(string number, int divisor)
{
    string ans;
    int idx = 0;
    int temp = number[idx] - '0';
    while (temp < divisor)
       temp = temp * 10 + (number[++idx] - '0');
    while (number.size() > idx)
    {
        ans += (temp / divisor) + '0';
        temp = (temp % divisor) * 10 + number[++idx] - '0';
    }
    if (ans.length() == 0)
        return "0";
    return ans;
}

char reVal(int n)
{
    if (n >= 0 && n <= 9)
        return (char)(n + '0');
    else
        return (char)(n - 10 + 'A');
}

int val(char c)
{
	if(c>='0' && c<='9')
		return (int)c-'0';
	else
		return (int)c-'A'+10;
}

string fromdec(string n, int b)
{
    string s;
    while (n != "0")
    {
        s+= reVal(mod(n,b));
        n=ddx(n,b);
    }
    reverse(s.begin(),s.end());
    return s;
}

string dec(string s, int b)
{
    string n="0",p="1",bb=to_string(b);
	for(int i=s.size()-1;i>=0;i--)
	{
		if(val(s[i])>=b)
		{
            printf("Invalid number");
            return "-1";
		}
		string sss=to_string(val(s[i])),ss;
		ss=px(p,sss);
		n=sx(ss,n);
		p=px(p,bb);
	}
	return n;
}

int main()
{
    string s;
    int b,c;
    in>>s>>b>>c;
    s=dec(s,b);
    out<<fromdec(s,c);
	return 0;
}
