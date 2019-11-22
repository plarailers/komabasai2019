#include<iostream>
using namespace std;

int func(int before, int tmp){ //a[i-1], a[i]を引数にして、次の状態でのa[i]の値を返す関数(a[i+1] == 0の場合の処理はmain関数の中で)
    if (tmp == 0) {
        return before;
    }
    else {
        return tmp;
    }
}
int fit(int i){
    if (i > 0){
        return i - 1;
    }
    else {
        return 5;
    }
}

int main() {
    bool flag = false;
    bool flag2 = false;
    int a[7] = {1, 2, 0, 3, 4, 0, 1};
    for (int j = 0; j < n; j++){ //nに適当な数字を入れる
        if (a[1] == 0) {
            flag2 = true;
        }
    for (int i = 1; i < 7; i++) {
        //cout << a[i] << endl;
        if (a[i] == 0){
            flag = true;
        }
        a[i] = func(a[i-1], a[i]);
        //cout << a[i] << endl;
        //cout << endl;
        if (flag) {
            a[i-1] = 0;
        }
        flag = false;
    }
    
    a[0] = a[6];
    if (flag2){
        a[0] = 0;
        a[6] = 0;
        flag2 = false;
    }
    /*for (int i = 0; i < 7; i++){
         cout << a[i] << " ";
    }
    cout << endl;
    }*/
    for (int i = 0; i < 6; i++){
         cout << a[i] << " ";
    }
    cout << endl;
}
