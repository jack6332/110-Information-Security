﻿#include<iostream>
#include<vector>

using namespace std;

void Caesar(char* plaintext, int shift);
void Playfair(char* plaintext, char* key);
void Vernam(char* plaintext, char* key);
void RailFence(char* plaintext, char* key);
void RowTransition(char* plaintext, char* key);

int main(int argc, char* argv[]) {
    /*std::cout << "We have " << argc << " arguments" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "[" << i << "] " << argv[i] << std::endl;
    }*/
    char test[] = "helloworld";
    char key[] = "3";
    
    RailFence(test, key);
    
    return 0;
}
void Caesar(char* plaintext, int shift) {
    for (int i = 0; i < strlen(plaintext); i++) {
        if ((plaintext[i] + shift) >= 123) {
            plaintext[i] = toupper((plaintext[i] + shift) % 123 + 'a');
        }
        else if ((plaintext[i] + shift) < 97) {
            plaintext[i] = toupper((plaintext[i] + shift) + 26);
        }
        else {
            plaintext[i] = toupper((plaintext[i] + shift) % 123);
        }
    }
    cout << plaintext << endl;
}   
void Playfair(char* plaintext, char* key) {

    vector<char>input;
    vector<char>code;
    char table[5][5] = {' '};
    bool alphabet[26] = {false};
    alphabet[9] = true;

    struct Pair
    {
        int i;
        int j;
    };

    for (int i = 0; i < strlen(plaintext); i++) {
        input.push_back(plaintext[i]);
    }

    // Generate Key Table
    int row=0, col=0;
    for (int i = 0; i < strlen(key); i++) {
        if (alphabet[key[i]-'a'] == false) {
            alphabet[key[i]-'a'] = true;
            table[row][col] = key[i];
            col++;
            if (col == 5) {
                col = 0;
                row++;
            }
        }
    }
    for (int i = 0; i < 26; i++) {
        if (alphabet[i] == false) {
            alphabet[i] = true;
            table[row][col] = 'a' + i;
            col++;
            if (col == 5) {
                col = 0;
                row++;
            }
        }
    }
    
    //// Print Key Table
    //for (int i = 0; i < 5; i++) {
    //    for (int j = 0; j < 5; j++) {
    //        cout << table[i][j] << " ";
    //    }
    //    cout << endl;
    //}

    // Plain Text process
    for (int i = 0; i < input.size()-1; i++) {
        if (input[i] == input[i + 1]) {
            input.insert(input.begin() + i + 1, 'x');
        }
        i++;
    }
    if (input.size() % 2) {
        input.push_back('x');
    }

    //// Print processd plaintext
    //for (int i = 0; i < input.size(); i++) {
    //    cout << input[i];
    //}
    //cout << endl;

    Pair a, b;

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (input[i] == table[j][k]) {
                    a.i = j;
                    a.j = k;
                }
            }
        }
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (input[i+1] == table[j][k]) {
                    b.i = j;
                    b.j = k;
                }
            }
        }
        //cout << "Pair:" << input[i] << " " << input[i + 1] << endl;
        i++;
        // Same Row
        if (a.i == b.i) {
            code.push_back(table[a.i][(a.j + 1) % 5]);
            code.push_back(table[a.i][(b.j + 1) % 5]);
        }
        // Same Col
        else if (a.j == b.j) {
            code.push_back(table[(a.i + 1) % 5][a.j]);
            code.push_back(table[(b.i + 1) % 5][b.j]);
        }
        // Both different
        else {
            code.push_back(table[a.i][b.j]);
            code.push_back(table[b.i][a.j]);
        }
        
    }
    for (int i = 0; i < code.size(); i++) {
        code[i] = toupper(code[i]);
        cout<<code[i];
    }
    cout << endl;
}
void Vernam(char* plaintext, char* key) {
    vector<int>keystream;

    for (int i = 0; i < strlen(key); i++) {
        keystream.push_back(key[i] - 'a');
    }

    for (int i = 0; i < strlen(plaintext)-strlen(key); i++) {
        keystream.push_back(plaintext[i] - 'a');
    }

    for (int i = 0; i < keystream.size(); i++) {
        if ((plaintext[i] + keystream[i]) >= 123) {
            plaintext[i] = toupper((plaintext[i] + keystream[i]) % 123 + 'a');
        }
        else {
            plaintext[i] = toupper((plaintext[i] + keystream[i]) % 123);
        }
    }
    cout << plaintext << endl;
}
void RailFence(char* plaintext, char* key) {

    int row = key[0] - '0';
    int col = strlen(plaintext);
    vector<vector<char> > chipertext(row, vector<char>(col, ' '));
    int current_row = 0;
    bool increase = true;
    for (int i = 0; i < col; i++) {
        chipertext[current_row][i] = plaintext[i];
        if (current_row == row - 1) {
            increase = false;
        }
        else if(current_row ==0){
            increase = true;
        }
        if (increase) {
            current_row++;
        }
        else {
            current_row--;
        }
    }
    for (int j = 0; j < row; j++) {
        for (int k = 0; k < col; k++) {
            if (chipertext[j][k] != ' ') {
                cout << (char)toupper(chipertext[j][k]);
            }     
        }
    }
}
void RowTransition(char* plaintext, char* key) {
    int col = strlen(key);
    int count = 1;
    while (count != col+1) {
        for (int i = 0; i < strlen(key); i++) {
            if (count == key[i] - '0') {
                for (int j = 0; j < 4; j++) {
                    if (j * col + i >= strlen(plaintext)) {
                        cout << "-";
                    }
                    else {
                        cout << (char)toupper(plaintext[j * col + i]);
                    }
                }
                count++;
            }
        }
    }
}