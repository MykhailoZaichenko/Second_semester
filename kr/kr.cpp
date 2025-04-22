#include <iostream>
#include <string>
#include <windows.h>
using namespace std;
const int MPAG = 10;

struct Rec {
    string word;
    int pag[MPAG];
    int pagc;
    Rec* next;
};
void addRec(Rec*& head, const string& word, const int* pag, int �) {
    Rec* newNode = new Rec;
    newNode->word = word;
    newNode->pagc = � > MPAG ? MPAG : �;
    for (int i = 0; i < newNode->pagc; ++i)
        newNode->pag[i] = pag[i];
    newNode->next = head;
    head = newNode;
}
bool delRec(Rec*& head, const string& word) {
    Rec* cur = head;
    Rec* prev = nullptr;

    while (cur) {
        if (cur->word == word) {
            if (prev)
                prev->next = cur->next;
            else
                head = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}
bool editRec(Rec* head, const string& word) {
    while (head) {
        if (head->word == word) {
            cout << "������ ���� �����: ";
            cin >> head->word;
            cout << "������ ����� ������� (���� 10): ";
            cin >> head->pagc;
            if (head->pagc > MPAG) head->pagc = MPAG;
            for (int i = 0; i < head->pagc; ++i) {
                cout << "������� #" << (i + 1) << ": ";
                while (!(cin >> head->pag[i])) {
                    cin.clear(); cin.ignore(1000, '\n'); cout << "�������! ������ �����: ";
                }
            }
            return true;
        }
        head = head->next;
    }
    return false;
}
Rec* searchRec(Rec* head, const string& word) {
    while (head) {
        if (head->word == word)
            return head;
        head = head->next;
    }
    return nullptr;
}
void printList(Rec* head) {
    while (head) {
        cout << "�����: " << head->word << " | �������: ";
        for (int i = 0; i < head->pagc; ++i)
            cout << head->pag[i] << (i < head->pagc - 1 ? ", " : "");
        cout << endl;
        head = head->next;
    }
}
void freeList(Rec*& head) {
    while (head) {
        Rec* temp = head;
        head = head->next;
        delete temp;
    }
}
int main() {
    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251);
    Rec* list = nullptr;
    int ch;
    do {
        cout << "������ � ������\n";
        cout << "1 ��������� ������\n";
        cout << "2 ��������� ������\n";
        cout << "3 ����������� ������\n";
        cout << "4 ����� ������ �� �������� �������\n";
        cout << "5 ��������� ������ �� �����\n";
        cout << "6 �����\n";
        cout << "��� ����: ";
        cin >> ch;
        if (ch == 1) {
            string word;
            int pag[MPAG], �;
            cout << "������ �����: ";
            cin >> word;
            cout << "������ ������� (���� 10): ";
            cin >> �; 
            if (� > MPAG) � = MPAG;
            for (int i = 0; i < �; ++i) {
                cout << "������� #" << (i + 1) << ": ";
                while (!(cin >> pag[i])) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "�������! ������ �����: ";
                }
            }
            addRec(list, word, pag, �);
        }
        else if (ch == 2) {
            string word;
            cout << "����� ��� ���������: ";
            cin >> word;
            if (!delRec(list, word))
                cout << "����� �� ��������!\n";
        }
        else if (ch == 3) {
            string word;
            cout << "����� ��� �����������: ";
            cin >> word;
            if (!editRec(list, word))
                cout << "����� �� ��������!\n";
        }
        else if (ch == 4) {
            string word;
            cout << "����� ��� ������: ";
            cin >> word;
            Rec* found = searchRec(list, word);
            if (found) {
                cout << "��������! �����: " << found->word << " | �������: ";
                for (int i = 0; i < found->pagc; ++i)
                    cout << found->pag[i] << (i < found->pagc - 1 ? ", " : "");
                cout << endl;
            }
            else {
                cout << "�� ��������.\n";
            }
        }
        else if (ch == 5) {
            printList(list);
        }
    } while (ch != 0);
    freeList(list);
    return 0;
}