#include "quaternary.h"

void print_qs(vector<Quarternary> Qs) {
    vector<double*> points;
    for (unsigned int i = 0; i < Qs.size(); i++) {
        cout << "(" << Qs[i].op << ", ";
        vector<double*>::iterator it_1 = find(points.begin(), points.end(), Qs[i].a);
        vector<double*>::iterator it_2 = find(points.begin(), points.end(), Qs[i].b);
        if (it_1 == points.end()) {
            cout << setw(8) << left << *(Qs[i].a) << ", ";
        } else {
            cout << "t" << setw(7) << left << it_1 - points.begin() << ", ";
        }
        if (it_2 == points.end()) {
            cout << setw(8) << left << *(Qs[i].b) << ", ";
        } else {
            cout << "t" << setw(7) << left << it_2 - points.begin() << ", ";
        }
        points.push_back(Qs[i].res);
        cout << "t" << setw(7) << left << points.size() - 1 << ")" << endl;
    }
}
