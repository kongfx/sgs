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
struct BigInt{
    int num[MAX];
    int minusflag;
    BigInt&operator=(const char*);
    BigInt&operator=(int);
    BigInt();
    BigInt(int);

    bool operator>(const BigInt&)const;
    bool operator<(const BigInt&)const;
    bool operator<=(const BigInt&)const;
    bool operator>=(const BigInt&)const;
    bool operator!=(const BigInt&)const;
    bool operator==(const BigInt&)const;

    BigInt operator+(const BigInt&)const;
    BigInt operator-(const BigInt&)const;
    BigInt operator*(const BigInt&)const;
    BigInt operator/(const BigInt&)const;
    BigInt operator%(const BigInt&)const;

    BigInt&operator+=(const BigInt&);
    BigInt&operator-=(const BigInt&);
    BigInt&operator*=(const BigInt&);
    BigInt&operator/=(const BigInt&);
    BigInt&operator%=(const BigInt&);
    BigInt operator-() {
        BigInt val=*this;
        val.minusflag=!val.minusflag;
        return val;
    }
};
// num[0]用来保存数字位数。利用10000进制可以节省空间和时间。
BigInt&BigInt::operator=(const char* c){
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

BigInt&BigInt::operator=(int a){
    char s[MAX];
    sprintf(s,"%d",a);
    return *this=s;
}
BigInt::BigInt(){
	memset(num,0,sizeof(num));
	num[0]=1;
    minusflag=0;
}
BigInt::BigInt(int n){
	*this=n;
}// 目的：支持“hp a=1;”之类的代码。

// 如果位数不等，大小是可以明显看出来的。如果位数相等，就需要逐位比较。
bool BigInt::operator > (const BigInt &b) const{
    if (!minusflag && b.minusflag)
        return true;
    if (minusflag && b.minusflag) {
        BigInt x=*this,y=b;
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
bool BigInt::operator<(const BigInt &b)const{return b>*this;}
bool BigInt::operator<=(const BigInt &b)const{return !(*this>b);}
bool BigInt::operator>=(const BigInt &b)const{return !(b>*this);}
bool BigInt::operator!=(const BigInt &b)const{return (b>*this)||(*this>b);}
bool BigInt::operator==(const BigInt &b)const{return !(b>*this)&&!(*this>b);}
// 注意：最高位的位置和位数要匹配。
//加法
BigInt BigInt::operator+(const BigInt&b)const{
    BigInt c;
    if (minusflag && b.minusflag) {
        c.minusflag = 1;
    } else if (minusflag && !b.minusflag) {
        BigInt tmp = *this;
        tmp.minusflag=0;
        return b-tmp;


    } else if (!minusflag && b.minusflag) {
        BigInt tmp=b;
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
BigInt BigInt::operator-(const BigInt&b)const{
   BigInt c;
    if (minusflag && !b.minusflag) {
        BigInt tmp=*this;
        tmp.minusflag=0;
        c=tmp+b;
        c.minusflag =1;
        return c;
    }
    if (!minusflag && b.minusflag) {
        BigInt tmp=b;
        tmp.minusflag=0;
        return *this+tmp;
    }
    if (minusflag && b.minusflag) {
        BigInt t1=*this, t2=b;
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
BigInt&BigInt::operator+=(const BigInt &b){return *this=*this+b;}
BigInt&BigInt::operator-=(const BigInt &b){return *this=*this-b;}
//乘法
BigInt BigInt::operator*(const BigInt&b)const{
    BigInt c;
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
BigInt&BigInt::operator*=(const BigInt &b){return *this=*this*b;}
BigInt BigInt::operator/(const BigInt&b)const{
    BigInt c, d;
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
            if(b*BigInt(mid)<=d) left=mid+1;
            else right=mid;
        }
        c.num[i]=right-1;
        d=d-b*BigInt(right-1);
    }
    while(c.num[c.num[0]]==0&&c.num[0]>1) c.num[0]--;    // 99999999/99999999
    return c;
}
BigInt abs(BigInt x) {
    x.minusflag=0;
    return x;
}
BigInt BigInt::operator%(const BigInt&b)const{
    BigInt c, d;
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
            if(b*BigInt(mid)<=d) left=mid+1;
            else right=mid;
        }
        c.num[i]=right-1;
        d=d-b*BigInt(right-1);
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
BigInt&BigInt::operator/=(const BigInt &b){return *this=*this/b;}
BigInt&BigInt::operator%=(const BigInt &b){return *this=*this%b;}
ostream&operator<<(ostream &o,BigInt &n){
    if (n.minusflag) {
        o<<'-';
    }
    o<<n.num[n.num[0]];
    for(int i=n.num[0]-1;i>=1;i--){
        o.width(4);
        o.fill('0');
        o<<n.num[i];
    }
    return o;
}
istream & operator>>(istream &in, BigInt &n){
    char s[MAX];
    in>>s;
    n=s;
    return in;
}
// FastRead Part
// 快读
template<typename T>
 T fastread(T){
	T ans=0;
    int flag=1;
	char ch=getchar();
	while((ch>'9'||ch<'0')&&ch!='-') ch=getchar();
	if(ch=='-') flag=-1,ch=getchar();
	while(ch>='0'&&ch<='9'){
		ans=ans*10+ch-'0';
		ch=getchar();
	}
	return ans*flag;
}

BigInt mexp(const BigInt &x) {
    return x*x;
}

BigInt msum(const BigInt &x) {
    return x*(x-1)/2;
}
namespace Lexer {
    enum class TokenType {
        BadToken=1,//1
        Number,//2
        Add,//3
        Sub,//4
        Mul,//5
        Div,//6
        Mod,//7
        Func,//8
        LP,//9
        RP,//10
        EOL//11

    };
    enum class FuncType {
        Sqrt=1,
        Exp,
        Msum,

    };
    struct Token {
        TokenType type;
        BigInt val;
        FuncType ftype;
    };
    static char *lineBuffer;
    static size_t pos=0;
    inline char nextChar() {
        //cout << "GetChar:" << lineBuffer[pos] << endl;
        return lineBuffer[pos++];
    }
    inline void backChar() {
        //cout << "BackChar:"<<lineBuffer[pos-1] << endl;
        pos--;
    }

    void getToken(Token* token) {
        while(true) {
            token->type = TokenType::BadToken;
            char c = nextChar();
            if (c=='\0')
                return;

            if (isdigit(c)) {
                token->type = TokenType::Number;
                BigInt val;
                while (isdigit(c)) {
                    val = val*10 + (c^48);
                    c=nextChar();
                }
                token->val = val;
                backChar();
                //cout << "ReturnTok: " << (int) token->type << endl;
                return;
            }
            if (isspace(c)) {
                if(c=='\n') {
                    token->type = TokenType::EOL;
                    //cout << "ReturnTok: " << (int) token->type << endl;
                    return;
                }
                continue;

            }
            switch (c) {
                case '+':
                    token->type = TokenType::Add;
                break;
                case '-':
                    token->type = TokenType::Sub;
                break;
                case '*':
                    token->type = TokenType::Mul;
                break;
                case '/':
                    nextChar();
                token->type=TokenType::Div;
                break;
                case '%':
                    token->type=TokenType::Mod;
                break;
                case '(':
                    token->type=TokenType::LP;
                break;
                case ')':
                    token->type=TokenType::RP;
                break;
                case 's':
                    if (nextChar() == 'q')
                        if (nextChar() == 'r')
                            if (nextChar() == 't') {
                                token->type = TokenType::Func;
                                token->ftype = FuncType::Sqrt;
                                break;
                            }
                case 'm':
                    if (nextChar() =='s' && nextChar() == 'u' && nextChar() == 'm') {
                        token->type=TokenType::Func;
                        token->ftype=FuncType::Msum;
                        break;
                    }
                case 'e':
                    if(nextChar() == 'x' && nextChar() == 'p') {
                        token->type = TokenType::Func;
                        token->ftype = FuncType::Exp;
                    }
                break;
                default:
                    fprintf(stderr, "WhatChar??? `%c' ", c);


            }
            //cout << "ReturnTok: " << (int) token->type << endl;
            return;
        }
    }

    void setLine(char *line) {
        lineBuffer=line;
        pos=0;

    }
};
BigInt sqrt(BigInt n) {
    BigInt l=1,r=(n+50)/2, mid=0, maxcnt = n*n*n;

    while((maxcnt-=1)+1>0) {
        mid = l+(r-l)/2;
        BigInt res=mid*mid;
        //cout << "sqrt(" << n << "): l="<<l<<" r="<<r<<" mid=" << mid << " res=" << res << endl;
        if (res == n || (res < n && (mid+1)*(mid+1) > n)) {
            return mid;
        } else if (res < n) {
            l=mid+1;
        } else {
            assert(res>n);
            r=mid-1;
        }

    }
    printf("detected dead loop... \nAborting...\n");

    abort();
    return 0;
}
namespace Parser {
    Lexer::Token tokenRingBuffer;
    bool hasTokenRing;

    void inputToken(Lexer::Token *tok) {
        if (hasTokenRing) {
            hasTokenRing=false;
            *tok=tokenRingBuffer;
            return;
        }
        Lexer::getToken(tok);
    }
    void unputToken(Lexer::Token *tok) {

        hasTokenRing=true;
        tokenRingBuffer=*tok;
        return;
    }

    BigInt parseExpr();
    BigInt parseTerm() {
        BigInt val;
        Lexer::Token tok;
        int minusflag=0;
        inputToken(&tok);
        if (tok.type == Lexer::TokenType::Sub) {
            minusflag=1;
        } else {
            unputToken(&tok);
        }
        inputToken(&tok);
        if (tok.type == Lexer::TokenType::Number) {
            val=tok.val;
            return minusflag ? -val : val;
        } else if (tok.type == Lexer::TokenType::Func) {
            switch (tok.ftype)
            {
            case Lexer::FuncType::Exp:
                inputToken(&tok);
                if (tok.type!=Lexer::TokenType::LP) {
                    fprintf(stderr, "Bad Token...\nAborting\n");


                }
                val=::mexp(parseExpr());
                inputToken(&tok);
                if (tok.type != Lexer::TokenType::RP) {
                    fprintf(stderr, "Missing `)'...\nAborting\n");

                }
            return minusflag ? -val : val;
                break;
            case Lexer::FuncType::Msum:
            inputToken(&tok);
                if (tok.type!=Lexer::TokenType::LP) {
                    fprintf(stderr, "Bad Token...\nAborting\n");


                }
                val=::msum(parseExpr());
                inputToken(&tok);
                if (tok.type != Lexer::TokenType::RP) {
                    fprintf(stderr, "Missing `)'...\nAborting\n");

                }
            return minusflag ? -val : val;
            case Lexer::FuncType::Sqrt:
            inputToken(&tok);
                if (tok.type!=Lexer::TokenType::LP) {
                    fprintf(stderr, "Bad Token...\nAborting\n");


                }
                val=::sqrt(parseExpr());
                inputToken(&tok);
                if (tok.type != Lexer::TokenType::RP) {
                    fprintf(stderr, "Missing `)'...\nAborting\n");

                }
            return minusflag ? -val : val;
            default:
                break;
            }
        } else if (tok.type==Lexer::TokenType::LP) {
            val=parseExpr();
            inputToken(&tok);
            if (tok.type != Lexer::TokenType::RP) {
                    fprintf(stderr, "Missing `)'...\nAborting\n");

            }
                        return minusflag ? -val : val;

        } else if(tok.type==Lexer::TokenType::EOL) {
            return parseTerm();
        }
        fprintf(stderr, "Syntax error..., token%d", (int)tok.type);

        return 0;

    }

    BigInt parseFactor() {
        Lexer::Token tok;
        BigInt v1,v2;
        v1=parseTerm();
        while(1) {
            inputToken(&tok);
            if (tok.type != Lexer::TokenType::Mul &&
                tok.type != Lexer::TokenType::Div &&
                tok.type != Lexer::TokenType::Mod
            ) {
                unputToken(&tok);
                return v1;

            }
            v2=parseTerm();
            if (tok.type == Lexer::TokenType::Mul)
                v1 *= v2;
            else if (tok.type == Lexer::TokenType::Div)
                v1/=v2;
            else v1%=v2;

        }

    }

    BigInt parseExpr() {
        Lexer::Token tok;
        BigInt v1,v2;
        v1=parseFactor();
        while(1) {
            inputToken(&tok);
            if (tok.type != Lexer::TokenType::Add &&
                tok.type != Lexer::TokenType::Sub
            ) {
                unputToken(&tok);
                return v1;

            }
            v2=parseFactor();
            if (tok.type == Lexer::TokenType::Add)
                v1 += v2;
            else if (tok.type == Lexer::TokenType::Sub)
                v1-=v2;

        }
    }

    BigInt parseLine(char *line) {
        Lexer::setLine(line);
        BigInt val=parseExpr();
        return val;
    }

}

int main() {
    char *buf= (char*)malloc(10000);
    int count = fastread(0);
    BigInt v;
    for (int i=0;i<count;i++) {
        fgets(buf, 9997, stdin);
        v=Parser::parseLine(buf);
        cout << v << endl;

    }


    free(buf);
}
