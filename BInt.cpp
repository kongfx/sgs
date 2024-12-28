/*
 * Copyright (C) 2024 ko114
 * Permission to use, copy, modify and distribute this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies.
 * ko114 makes no representations about the suitability of this software for any
 * purpose.  It is provided "as is" WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * 版权所有 (C) 2024 ko114
 * 兹允许免费使用、复制、修改和分发本软件及其文档，无需支付费用，但须保证所有副本中包含上述版权声明。
 * ko114 不对本软件适用于任何目的作任何陈述。该软件按原样提供，不附带任何保修或保证；
 *
 */

#include <bits/stdc++.h>
using namespace std;

// BigInt Part
// 高精度部分
const int MAX=1001;
struct BInt{
    int num[MAX];
    int minusflag;
    BInt&operator=(const char*);
    BInt&operator=(int);
    BInt();
    BInt(int);

    bool operator>(const BInt&)const;
    bool operator<(const BInt&)const;
    bool operator<=(const BInt&)const;
    bool operator>=(const BInt&)const;
    bool operator!=(const BInt&)const;
    bool operator==(const BInt&)const;

    BInt operator+(const BInt&)const;
    BInt operator-(const BInt&)const;
    BInt operator*(const BInt&)const;
    BInt operator/(const BInt&)const;
    BInt operator%(const BInt&)const;

    BInt&operator+=(const BInt&);
    BInt&operator-=(const BInt&);
    BInt&operator*=(const BInt&);
    BInt&operator/=(const BInt&);
    BInt&operator%=(const BInt&);
    BInt operator-() {
        BInt val=*this;
        val.minusflag=!val.minusflag;
        return val;
    }
};
// num[0]用来保存数字位数。利用10000进制可以节省空间和时间。
BInt&BInt::operator=(const char* c){
    memset(num,0,sizeof(num));
    int n=strlen(c),j=1,k=1;
    int mflag=0;
    if (c[0] == '-') mflag=1;

    for(int i=1;i<=(n-mflag);i++){

        if(k==10000) j++,k=1;// 10000进制，4个数字才算1位。
        num[j]+=k*(c[n-i]-'0');
        k*=10;
    }
    num[0]=j;
    minusflag = mflag;
    return *this;
}

BInt&BInt::operator=(int a){
    char s[MAX];
    sprintf(s,"%d",a);
    return *this=s;
}
BInt::BInt(){
	memset(num,0,sizeof(num));
	num[0]=1;
    minusflag=0;
}
BInt::BInt(int n){
	*this=n;
}// 目的：支持“hp a=1;”之类的代码。

// 如果位数不等，大小是可以明显看出来的。如果位数相等，就需要逐位比较。
bool BInt::operator > (const BInt &b) const{
    if (!minusflag && b.minusflag)
        return true;
    if (minusflag && b.minusflag) {
        BInt x=*this,y=b;
        x.minusflag=0;
        y.minusflag=0;
        return x<y;

    }
    if (minusflag && !b.minusflag) {
        return false;
    }
    if(num[0]!=b.num[0]) return num[0]>b.num[0];
    for(int i=num[0];i>=1;i--)
        if(num[i]!=b.num[i])
            return (num[i]>b.num[i]);
    return false;
}
bool BInt::operator<(const BInt &b)const{return b>*this;}
bool BInt::operator<=(const BInt &b)const{return !(*this>b);}
bool BInt::operator>=(const BInt &b)const{return !(b>*this);}
bool BInt::operator!=(const BInt &b)const{return (b>*this)||(*this>b);}
bool BInt::operator==(const BInt &b)const{return !(b>*this)&&!(*this>b);}
// 注意：最高位的位置和位数要匹配。
//加法
BInt BInt::operator+(const BInt&b)const{
    BInt c;
    if (minusflag && b.minusflag) {
        c.minusflag = 1;
    } else if (minusflag && !b.minusflag) {
        BInt tmp = *this;
        tmp.minusflag=0;
        return b-tmp;


    } else if (!minusflag && b.minusflag) {
        BInt tmp=b;
        tmp.minusflag=0;
        return tmp-b;
    }
    c.num[0]=std::max(num[0],b.num[0]);
    for(int i=1;i<=c.num[0];i++){
        c.num[i]+=num[i]+b.num[i];
        if(c.num[i]>=10000){// 进位
            c.num[i]-=10000;
            c.num[i+1]++;
        }
    }
    if(c.num[c.num[0]+1]>0) c.num[0]++;// 9999+1，计算完成后多了一位
    return c;
}
//减法
BInt BInt::operator-(const BInt&b)const{
   BInt c;
    if (minusflag && !b.minusflag) {
        BInt tmp=*this;
        tmp.minusflag=0;
        c=tmp+b;
        c.minusflag =1;
        return c;
    }
    if (!minusflag && b.minusflag) {
        BInt tmp=b;
        tmp.minusflag=0;
        return *this+tmp;
    }
    if (minusflag && b.minusflag) {
        BInt t1=*this, t2=b;
        t1.minusflag=0;t2.minusflag=0;
        return t1-t2;

    }
    if (*this < b) {

        c= b-*this;
        c.minusflag =1;
        return c;

    }
   c.num[0]=num[0];
   for (int i=1;i<=c.num[0];i++){
       c.num[i]+=num[i]-b.num[i];
       if(c.num[i]<0){  // 退位
            c.num[i]+=10000;
            c.num[i+1]--;
        }
    }
    while(c.num[c.num[0]]==0&&c.num[0]>1) c.num[0]--;// 100000000-99999999
    return c;
}
//顺便声明
BInt&BInt::operator+=(const BInt &b){return *this=*this+b;}
BInt&BInt::operator-=(const BInt &b){return *this=*this-b;}
//乘法
BInt BInt::operator*(const BInt&b)const{
    BInt c;
    c.minusflag = minusflag ^ b.minusflag;
    c.num[0]=num[0]+b.num[0]+1;
    for(int i=1;i<=num[0];i++){
        for(int j=1;j<=b.num[0];j++){
            c.num[i+j-1]+=num[i]*b.num[j];            // 和小学竖式的算法一模一样
            c.num[i+j]+=c.num[i+j-1]/10000;            // 进位
            c.num[i+j-1]%=10000;
        }
    }
    while(c.num[c.num[0]]==0&&c.num[0]>1) c.num[0]--;    // 99999999*0
    return c;
}
//同上
BInt&BInt::operator*=(const BInt &b){return *this=*this*b;}
BInt BInt::operator/(const BInt&b)const{
    BInt c, d;
    c.minusflag=minusflag^b.minusflag;

    c.num[0]=num[0]+b.num[0]+1;
    d.num[0]=0;
    for(int i=num[0];i>=1;i--){
        // 以下三行的含义是：d=d*10000+num[i];
        memmove(d.num+2, d.num+1, sizeof(d.num)-sizeof(int)*2);
        d.num[0]++;
        d.num[1]=num[i];

        // 以下循环的含义是：c.num[i]=d/b; d%=b; 利用二分查找求c.num[i]的上界。
        // 注意，这里是二分优化后除法和朴素除法的区别！
        int left=0,right=9999,mid;
        while(left<right){
            mid = (left+right)/2;
            if(b*BInt(mid)<=d) left=mid+1;
            else right=mid;
        }
        c.num[i]=right-1;
        d=d-b*BInt(right-1);
    }
    while(c.num[c.num[0]]==0&&c.num[0]>1) c.num[0]--;    // 99999999/99999999
    return c;
}
BInt abs(BInt x) {
    x.minusflag=0;
    return x;
}
BInt BInt::operator%(const BInt&b)const{
    BInt c, d;
    c.num[0]=num[0]+b.num[0]+1;
    d.num[0]=0;
    for(int i=num[0];i>=1;i--){
        // 以下三行的含义是：d=d*10000+num[i];
        memmove(d.num+2, d.num+1, sizeof(d.num)-sizeof(int)*2);
        d.num[0]++;
        d.num[1]=num[i];

        // 以下循环的含义是：c.num[i]=d/b; d%=b; 利用二分查找求c.num[i]的上界。
        // 注意，这里是二分优化后除法和朴素除法的区别！
        int left=0,right=9999,mid;
        while(left<right){
            mid = (left+right)/2;
            if(b*BInt(mid)<=d) left=mid+1;
            else right=mid;
        }
        c.num[i]=right-1;
        d=d-b*BInt(right-1);
    }
    while(c.num[c.num[0]]==0&&c.num[0]>1) c.num[0]--;    // 99999999/99999999
    if (!minusflag && !b.minusflag)
        return d;
    if(minusflag && !b.minusflag) {
        return b-d;

    }
    if (minusflag && b.minusflag) {
        d.minusflag=1;
        return d;

    }
    if (!minusflag && b.minusflag) {
        return -(b-d);

    }
    return d;
}
BInt&BInt::operator/=(const BInt &b){return *this=*this/b;}
BInt&BInt::operator%=(const BInt &b){return *this=*this%b;}

int test(int count) {
    mt19937 mt(time(0));

}