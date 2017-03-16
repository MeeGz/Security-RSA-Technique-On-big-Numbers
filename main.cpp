#include <iostream>
#include <vector>
#include "math.h"
#include <chrono>

using namespace std;
using namespace chrono;

class bigInt {
private:
    vector<unsigned long long> numVec;
    string numStr;
public:
    bigInt() {
        numStr = "0";
    }
    bigInt(string num) {
        numStr = num;
        numVec = toVec(num);
    }
    static void deleteLeftZeros(vector <unsigned long long> &v) {
        while (v.size() > 1 && v[v.size()-1] == 0)
            v.pop_back();
    }
    void setNumStr(string num) {
        numStr = num;
        numVec = toVec(num);
    }
    void setNumVec(vector<unsigned long long> num) {
        deleteLeftZeros(num);
        numVec = num;
        numStr = toStr(numVec);
    }
    string getNumStr() {
        return numStr;
    }
    vector<unsigned long long> getNumVec() {
        return numVec;
    }
    static vector<unsigned long long> toVec(string &str) {
        string tmp;
        int len = str.length(), i;
        vector<unsigned long long> v;
        for (i = 0; i < len; i += 9) {
            if(len >= i+9)
                tmp = str.substr(len-i-9,9);
            else
                tmp = str.substr(0,len-i);
            v.push_back(stoull(tmp));
        }
        deleteLeftZeros(v);
        return v;
    }
    static string toStr (vector<unsigned long long> &v) {
        string vec = "", tmp;
        for (int i = v.size()-1; i >= 0; --i) {
            tmp = to_string(v[i]);
            while(tmp.length() < 9 && i != v.size()-1)
                tmp = "0" + tmp;
            vec += tmp;
        }
        while(vec[0] == '0' && vec.length() != 1 )
            vec.erase(0,1);
        return vec;
    }
    static vector<unsigned long long> addNum(vector<unsigned long long> &num1, vector<unsigned long long> &num2) {
        int carry = 0, num1l = num1.size(), num2l = num2.size();
        unsigned long long segment;
        vector<unsigned long long> sum;
        for (int i = 0; i < max(num1l, num2l); ++i) {
            if (i<num1l && i<num2l)
                segment = num1[i]+ num2[i]+ carry;
            else if (i < num1l || i < num2l)
                if (i < num1l)
                    segment = num1[i]+ carry;
                else
                    segment = num2[i]+ carry;
            sum.push_back(segment % 1000000000);
            if (segment / 1000000000)
                carry = 1;
            else
                carry = 0;
        }
        if(carry)
            sum.push_back(1);
        return sum;
    }
    void subNum(bigInt &num1, bigInt &num2) {
        vector<unsigned long long> sum, num1v = num1.getNumVec() ,num2v = num2.getNumVec();
        sum = subNum(num1v, num2v);
        setNumVec(sum);
    }
    static vector<unsigned long long> subNum(vector<unsigned long long> &num1, vector<unsigned long long> &num2) {
        int carry = 0, num1l = num1.size(), num2l = num2.size(), endFlag = 0, i;
        unsigned long long segment;
        vector<unsigned long long> sum;
        for (i = 0; i < num1l; ++i) {
            if (i < num1l && i < num2l)
                if(num1[i] >= num2[i]+ carry ) {
                    segment = num1[i]- num2[i]- carry;
                    carry = 0;
                }
                else {
                    segment = num1[i]- num2[i]- carry+ 1000000000;
                    carry = 1;
                }
            else {
                segment = num1[i]- carry;
                endFlag = 1;
                break;
            }
            sum.push_back(segment);
        }
        if(endFlag)
            sum.push_back(segment);
        for (++i; i < num1l; ++i)
            sum.push_back(num1[i]);
        deleteLeftZeros(sum);
        return sum;
    }
    void mulNum(bigInt &num1, bigInt &num2) {
        vector<unsigned long long> sum, num1v = num1.getNumVec(), num2v = num2.getNumVec();
        sum = mulNum(num1v,num2v);
        setNumVec(sum);
    }
    static vector<unsigned long long> mulNum(vector<unsigned long long> num1, vector<unsigned long long> num2) {
        if (num1.size() < num2.size())
            swap(num1,num2);
        int num1l = num1.size(), num2l = num2.size();
        unsigned long long carry = 0, segment;
        vector<unsigned long long> sum, tmpSum;
        for (int i = 0; i < num2l; ++i) {
            for (int j = 0; j < num1l; ++j) {
                segment = num1[j] * num2[i] + carry;
                carry = segment / 1000000000;
                tmpSum.push_back(segment % 1000000000);
                if(j == num1l-1)
                    if(carry) {
                        tmpSum.push_back(carry);
                        carry = 0;
                    }
            }
            sum = addNum(sum,tmpSum);
            tmpSum.clear();
            for (int k = 0; k <= i; ++k)
                tmpSum.push_back(0);
        }
        return sum;
    }
    void divNum(bigInt &num1, bigInt &num2, bigInt &rem) {
        setNumVec(divNumLD(num1,num2,rem));
    }
    static void mod(bigInt &num1, bigInt &num2, bigInt &rem) {
        divNumLD(num1,num2,rem);
        num1.setNumVec(rem.getNumVec());
    }
    static vector<unsigned long long> divNumLD(bigInt &num1, bigInt &num2, bigInt &rem) {
        vector<unsigned long long> result = {0};
        if(num2.getNumStr() == "2")
            result = divNumBy2(num1,rem);
        else if (num2.getNumStr() == "1") {
            rem.setNumStr("0");
            result = num1.getNumVec();
        }
        else {
            int num2sL = num2.getNumStr().length();
            string num1s = num1.getNumStr(), num1part = num1s.substr(0,num2sL);
            int  num1sL = num1s.length(), count = 0;
            vector<unsigned long long> num1partVec = toVec(num1part), num2v = num2.getNumVec(), tmpResult, ten = {10};
            rem.setNumStr("0");
            while(num2sL <= num1sL) {
                while(isLargerOrEqual(num1partVec, num2v)) {
                    num1partVec = subNum(num1partVec,num2v);
                    count++;
                }
                tmpResult = {(unsigned long long)count};
                result = mulNum(result,ten);
                result = addNum(result,tmpResult);
                if(num2sL == num1sL)
                    break;
                num1part = num1s.substr(num2sL++,1);
                num1partVec = mulNum(num1partVec,ten);
                tmpResult = toVec(num1part); // to not declare another vector
                num1partVec = addNum(tmpResult, num1partVec);
                count = 0;
            }
            rem.setNumVec(num1partVec);
        }
        return result;
    }
    static vector<unsigned long long> divNumBy2(bigInt &num1, bigInt &rem) {
        vector<unsigned long long> num1v = num1.getNumVec();
        int carry = 0, num1vSize = num1v.size();
        vector<unsigned long long> result(num1vSize);
        while(num1vSize--) {
            result[num1vSize] = ((num1v[num1vSize] + carry) / 2);
            if((num1v[num1vSize] + carry) % 2)
                carry = 1000000000;
            else
                carry = 0;
        }
        if(carry)
            rem.setNumStr("1");
        else
            rem.setNumStr("0");
        return result;
    }
    static bool isLargerOrEqual(vector<unsigned long long> &num1, vector<unsigned long long> &num2) {
        int num1size = num1.size(), num2size = num2.size();
        if(num1size > num2size)
            return true;
        else if(num1size < num2size)
            return false;
        else {
            for (int i = num1size-1; i >= 0; --i) {
                if (num1[i] > num2[i])
                    return true;
                else if (num1[i] < num2[i])
                    return false;
            }
            return true;
        }
    }
    static bigInt powNumEx(bigInt num1, bigInt num2, bigInt &num3) {
        vector<unsigned long long> num2v = num2.getNumVec();
        bigInt two = {"2"}, result{"1"}, rem;
        mod(num1,num3,rem);
        while(true) {
            num2.divNum(num2,two,rem);
            num2v = num2.getNumVec();
            if(num2v[0] != 0 || num2v.size() > 1) {
                if(rem.getNumVec()[0] == 1) {
                    result.mulNum(result,num1);
                    mod(result,num3,rem);
                }
                num1.mulNum(num1,num1);
                mod(num1,num3,rem);
            }
            else
                break;
        }
        result.mulNum(result,num1);
        mod(result,num3,rem);
        return result;
    }
    void extEuclid(bigInt &a3, bigInt &b3) {
        setNumVec(extendEuclid(a3,b3));
    }
    static vector<unsigned long long> extendEuclid(bigInt b, bigInt m) {
        bigInt rem;
        mod(b,m,rem);
        vector<unsigned long long> a3 = m.getNumVec(), Mod = a3, b3= b.getNumVec(), a2 = {0}, b2 = {1}, q, tmp2, tmp3, qb, noInverse ={0};
        while(true) {
            if(b3.size() == 1) {
                if(b3[0] == 0)
                    return noInverse; // no inverse !!
                if(b3[0] == 1)
                    return b2;
            }
            q = divNumLD(m,b,rem);
            qb = mulNum(q,b2);
            while(!isLargerOrEqual(a2,qb))
                a2 = addNum(a2,Mod);
            tmp2 = subNum(a2, qb);
            qb = mulNum(q,b3);
            if(!isLargerOrEqual(a3,qb))
                a3 = addNum(a3,Mod);
            tmp3 = subNum(a3, qb);
            a2 = b2;
            a3 = b3;
            b2 = tmp2;
            b3 = tmp3;
            m.setNumVec(a3);
            b.setNumVec(b3);
        }
    }
    static bool isPrime (bigInt &num) {
        vector<unsigned long long> numv = num.getNumVec();
        if(numv.size() == 1 && numv[0] == 1)
            return false;
        if(numv.size() == 1 && (numv[0] == 2 || numv[0] == 3))
            return true;
        int k = 0;
        bigInt numMinus1 , q, rem, base1("2"), base2("3"), twoPowerJQ;
        vector<unsigned long long> tmpNumv, one = {1};
        vector<bigInt> base = {base1,base2};
        numMinus1.setNumVec(subNum(numv, one));
        q = numMinus1;
        while(true)
        {
            numMinus1.setNumVec(divNumBy2(numMinus1,rem));
            if(rem.getNumVec()[0] != 0)
                break;
            q = numMinus1;
            k++;
        }
        for (int i = 0; i < 2; ++i)
        {
            tmpNumv = powNumEx(base[i],q,num).getNumVec();
            if(tmpNumv[0] == 1 && tmpNumv.size() == 1)
                return true;
            for (int j = 0; j < k; ++j) {
                tmpNumv.clear();
                tmpNumv.push_back(pow(2,j));
                twoPowerJQ.setNumVec(tmpNumv);
                twoPowerJQ.mulNum(twoPowerJQ,q);
                if(powNumEx(base[i], twoPowerJQ, num).getNumVec() == subNum(numv,one))
                    return true;
            }
        }
        return false;
    }
    void encrypt (bigInt &m, bigInt &key, bigInt &n){
        *this = powNumEx(m,key,n);
    }
};

int main() {
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;

    bool quit = true;
    string p, q, e, m, c, input;
    bigInt P, Q, N, Phi, E, P1, Q1, D, C, M, one("1"), resIsPrime;

    while(quit)
    {
        cin >> input;
        t1 = high_resolution_clock::now();
        if ((input[0] == 'p' || input[0] == 'P') && input[1] == '=')
            P.setNumStr(input.substr(2, input.length()-2));
        else if ((input[0] == 'q' || input[0] == 'Q') && input[1] == '=')
            Q.setNumStr(input.substr(2, input.length()-2));
        else if ((input[0] == 'e' || input[0] == 'E') && input[1] == '=')
            E.setNumStr(input.substr(2, input.length()-2));
        else if (input == "IsPPrime")
            if(resIsPrime.isPrime(P))
                cout << "Yes" << endl;
            else
                cout << "No" << endl;
        else if (input == "IsQPrime")
            if(resIsPrime.isPrime(Q))
                cout << "Yes" << endl;
            else
                cout << "No" << endl;
        else if (input == "PrintN")
        {
            if(N.getNumVec().size() == 0)
                N.mulNum(P,Q);
            cout << N.getNumStr() << endl;
        }
        else if (input == "PrintPhi")
        {
            if(Phi.getNumVec().size() == 0)
            {
                P1.subNum(P,one);
                Q1.subNum(Q,one);
                Phi.mulNum(P1,Q1);
            }
            cout << Phi.getNumStr() << endl;
        }
        else if (input == "PrintD")
        {
            if(Phi.getNumVec().size() == 0)
            {
                P1.subNum(P,one);
                Q1.subNum(Q,one);
                Phi.mulNum(P1,Q1);
            }
            D.extEuclid(E,Phi);
            cout << D.getNumStr() << endl;
        }
        else if (input.substr(0,13) == "EncryptPublic")
        {
            m = input.substr(15,input.length()-16);
            M.setNumStr(m);
            if(N.getNumVec().size() == 0)
                N.mulNum(P,Q);
            C.encrypt(M,E,N);
            cout << C.getNumStr() << endl;
        }
        else if (input.substr(0,14) == "EncryptPrivate")
        {
            m = input.substr(16,input.length()-17);
            M.setNumStr(m);
            if(N.getNumVec().size() == 0)
                N.mulNum(P,Q);
            if(D.getNumVec().size() == 0)
            {
                if(Phi.getNumVec().size() == 0)
                {
                    P1.subNum(P,one);
                    Q1.subNum(Q,one);
                    Phi.mulNum(P1,Q1);
                }
                D.extEuclid(E,Phi);
            }
            C.encrypt(M,D,N);
            cout << C.getNumStr() << endl;
        }
        else if (input == "Quit" || input == "quit")
            quit = false;
        t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(t2 - t1).count();
        cout << "Time : " << duration <<"ms .... as: " << duration/1000000 << " sec"<< endl;
    }
    return 0;
}