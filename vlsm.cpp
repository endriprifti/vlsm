/* 
 *  Endri Prifti
 *  Gjeneruesi VLSM
 *  (c) Copyright 2009
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. 
 *  If not, see http://www.gnu.org/licenses/.
*/

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

unsigned short bite_rrjeti;
unsigned nr_subnet;
vector<unsigned> nr_komp;

static union {
    unsigned adresaPlote;
    unsigned char adresa[4];
};

static union {
    unsigned subnetMaskPlote;
    unsigned char subnetMask[4];
};

inline unsigned nrNrMaxKombinimeve(unsigned nr) {
    return (unsigned) pow(2, ceil(log2(nr)));
}

bool egzistonShperndarja() {
    unsigned shumaHapsiraveIP = 0;
    for(unsigned i=0; i<nr_komp.size(); i++) {
        nr_komp[i] = nrNrMaxKombinimeve(nr_komp[i] + 2);
        shumaHapsiraveIP += nr_komp[i];
    }
    return shumaHapsiraveIP <= pow(2.0, 32.0 - bite_rrjeti);
}

void coptoAdresen(string &strAdreses) {
    unsigned short pikaFundit = 0, byte = 3;
    strAdreses.append(".");
    for(unsigned i=0; i<strAdreses.length(); i++)
        if(strAdreses[i] == '.') {
            adresa[byte--] = (char) atoi((strAdreses.substr(pikaFundit, i)).c_str());
            pikaFundit = i+1;
        }
}

void kontrolloAdresen() {
    string strAdreses;
    unsigned short nrPikave = 0;

    jepAdresen:
        cout << "\n\t\tJep adresen e rrjetit : ";
        cin >> strAdreses;

    for(unsigned i=0; i<strAdreses.length(); i++) {
        if(isalpha(strAdreses[i])) {
            cout << "\t\tAdresa ka germa!" << endl << endl;
            strAdreses.clear();
            goto jepAdresen;
        }
        if(strAdreses[i] == '.') nrPikave++;
    }

    if(nrPikave != 3) {
        cout << "\t\tAdresa nuk ka 4 pjese!" << endl;
        nrPikave = 0;
        strAdreses.clear();
        goto jepAdresen;
    }

    coptoAdresen(strAdreses);
}

void zeroHostin() {
    if(bite_rrjeti == 8) {
        adresa[0] = adresa[1] = adresa[2] = 0;
        return;
    }
    if(bite_rrjeti == 16) {
        adresa[0] = adresa[1] = 0;
        return;
    }
    if(bite_rrjeti == 24) adresa[0] = 0;
}

inline void afishoIP() {
    for(unsigned short j = 3; j>0; j--) cout << setw(3) << (int) adresa[j] << ".";
    cout << setw(3) << (int) adresa[0];
    
}

void afishoSubnetMasken(unsigned i) {
    subnetMaskPlote = UINT_MAX;
    subnetMaskPlote -= (nr_komp[i] - 1);
    for(unsigned short j = 3; j>0; j--) cout << setw(3) << (int) subnetMask[j] << ".";
    cout << setw(3) << (int) subnetMask[0];
}

void afishoSubnetet() {
    cout << endl;
    cout << endl;
    for(unsigned i = 0; i<nr_komp.size(); i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\t\tSUBNET " << setw(3) << i+1 << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << "\t\tAdresa rrjetit\t: ";
        afishoIP();
        cout << endl;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        adresaPlote++;
        cout << "\t\tHapsira e IP-ve\t: ";
        afishoIP();
        cout << " - ";

        adresaPlote += (nr_komp[i] - 3);
        afishoIP();
        cout << endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        adresaPlote++;
        cout << "\t\tAdresa broadcast: ";
        afishoIP();
        cout << endl;

        cout << "\t\tSubnet Mask\t: ";
        afishoSubnetMasken(i);
        cout << endl;
        adresaPlote++;
        cout << endl;
    }
}

main() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "\n\n\n\t\tGjenerues VLSM\n\t\tEndri Prifti (c) 2009\n\n\n\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    perseritBitRrjeti:
        cout << "\t\tJep bitet e rrjetit (8, 16, 24) : ";
        cin >> bite_rrjeti;
    if((bite_rrjeti != 8) && (bite_rrjeti != 16) && (bite_rrjeti != 24)) {
        cout << "\t\tNuk pranohet!" << endl << endl;
        goto perseritBitRrjeti;                
    }

    cout << endl << "\t\tJep numrin e subneteve : ";
    cin >> nr_subnet;

    nr_komp.resize(nr_subnet);
    cout << endl << "\t\tNumri i PC per subnet" << endl;
    for(unsigned i=0; i<nr_komp.size(); i++) {
        cout << "\t\tSubnet " << " : ";
        cin >> nr_komp[i];    
    }

    if(egzistonShperndarja()) {
        kontrolloAdresen();
        zeroHostin();
        sort(nr_komp.rbegin(), nr_komp.rend());
        afishoSubnetet();
        cout << "\n\t\t";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        system("pause");
    }
    else {
        cout << "\t\tNUK behet kjo shperndarje!" << endl;
        cout << "\n\t\t";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        system("pause");
    }
}
