#include<iostream>
using namespace std;

int func(int before, int tmp, int i){ //a[i-1], a[i], iを引数にして、次の状態でのa[i]の値を返す関数(a[i+1] == 0の場合の処理はmain関数の中で)
    if (tmp == 0 && before != i) {
        return before;
    }
    else {
        return tmp;
    }
}
int fit(int i){　//使ってないので読み飛ばしていい関数です
    if (i > 0){
        return i - 1;
    }
    else {
        return 5;
    }
}

int main() {
    bool flag = false; #a[i] == 0ならtrue, それ以外はfalse
    bool flag2 = false;#2番目の駅に今車両があればfalse,なければ(a[1] == 0)ならtrue

    int a[7] = {1, 2, 0, 3, 4, 0, 1};#初期条件なのでいじって良い。a[6] == a[0]となるようにして円形を再現
    for (int j = 0; j < n; j++){#nに適当な値を入れるとn回目の動作後の状態が配列で返ってくる
        if (a[1] == 0) {
            flag2 = true;
        }
    for (int i = 1; i < 7; i++) {
        //cout << a[i] << endl;
        if (a[i] == 0){
            flag = true;
        }
        a[i] = func(a[i-1], a[i], i); #a[i]の値を更新
        //cout << a[i] << endl;
        //cout << endl;
        if (flag) {
            a[i-1] = 0;#操作前にa[i] == 0だった時a[i-1] == 0になるはずなので
        }
        flag = false;
    }
    
    a[0] = a[6];　#a[0] == a[6]にする
    if (flag2){#これをやらないとa[0] == a[6]にならない
        a[0] = 0;
        a[6] = 0;
        flag2 = false;
    }
    for (int i = 0; i < 7; i++){ #操作後のaを表示
         cout << a[i] << " ";
    }
    cout << endl;
    }
    /*for (int i = 0; i < 6; i++){
         cout << a[i] << " ";
    }
    cout << endl;*/
}
