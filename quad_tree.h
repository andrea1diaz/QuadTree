#include <iostream>
#include <string>
#include <fstream>
#define cimg_use_jpeg 1
#include "CImg.h"

using namespace cimg_library;
using namespace std;

const string filename = "data.dat";

struct Node {
    int x1;
    int y1;
    int x2;
    int y2;
    int color;
    Node *children[4];
    bool isLeaf = false;
    
    Node (int x1, int y1, int x2, int y2) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;

        for (int i = 0; i < 4; i++) children[i] = 0;
    }      
};

class QuadTree {
private:
    CImg<char> output;
    int width, height;

public:     
    QuadTree(CImg<char> img) : m_pRoot(0) {
        this->img = img;
        Node *r = new Node(0, 0, img.width() - 1, img.height() - 1);
        width = img.width();
        height = img.height();
        m_pRoot = r;
        output = img;
        
        build(0, 0, img.width() - 1, img.height() - 1, m_pRoot);
        build_output(m_pRoot);
    }

    void reconstruir() {
        CImg<char> img2(width, height);
        ifstream myfile(filename, ios::binary);
        if (myfile.is_open()) {
            myfile.seekg(0, ios::end);
            int bloques = myfile.tellg()/(4*sizeof(int) + sizeof(char));
            myfile.seekg(0, ios::beg);
            int x1, y1, x2, y2;
            char col;
            for (int i = 0; i < bloques; ++i) {
                myfile.read((char*) &x1, sizeof(int));
                myfile.read((char*) &y1, sizeof(int));
                myfile.read((char*) &x2, sizeof(int));
                myfile.read((char*) &y2, sizeof(int));
                myfile.read((char*) &col, sizeof(char));
                for (int j = x1; j <= x2; ++j) {
                    for (int k = y1; k <= y2; ++k) {
                        img2(j, k) = col;
                    }
                }
            }
            myfile.close();
        }
        img2.display();
    }
   
private:
    Node * m_pRoot;
    CImg<char> img;

    void build(int x1, int y1, int x2, int y2, Node* root) {
        if (check(x1, y1, x2, y2)) {
            root->color = img(x1,y1);
            root->isLeaf = true;
            ofstream myfile(filename, ios::binary | ios::app);
            if (myfile.is_open()) {
                myfile.write((char*) &x1, sizeof(int));
                myfile.write((char*) &y1, sizeof(int));
                myfile.write((char*) &x2, sizeof(int));
                myfile.write((char*) &y2, sizeof(int));
                myfile.write((char*) &(img(x1, y1)), sizeof(char));
                myfile.close();
            }
            return;
        }
        
        int x_m = floor((x2 + x1) / 2);
        int y_m = floor((y2 + y1) / 2);

        // cout << "x1: " << x1 << " y1: " << y1 << " x2: " << x2 << " y2: " << y2 << " x_m: " << x_m << " y_m: " << y_m << endl;
        
        root->children[0] = new Node (x1, y1, x_m - 1, y_m - 1);
        build(x1, y1, x_m - 1, y_m - 1, root->children[0]);

        root->children[1] = new Node(x_m + 1, y1, x2, y_m - 1);
        build(x_m + 1, y1, x2, y_m - 1, root->children[1]);
         
        root->children[2] = new Node (x1, y_m + 1, x_m - 1, y2);
        build(x1, y_m + 1, x_m - 1, y2, root->children[2]);

        root->children[3] = new Node(x_m + 1, y_m + 1, x2, y2);
        build(x_m + 1, y_m + 1, x2, y2, root->children[3]);
    }

    bool check (int x1, int y1, int x2, int y2) {
        auto c = img(x1,y1);
        
        for(int i = x1; i < x2; ++i) {
            for (int j = y1; j < y2; ++j) {
                if (img(i,j) != c) return false;
            }
        }
        
        return true;
    }

    void build_output(Node* p) {
        if (p) {
            if (p->isLeaf) {
                for (int i = p->x1; i <= p->x2; ++i) {
                    for (int j = p->y1; j <= p->y2; ++j) {
                        output(i, j) = p->color;
                    }
                }
            }
            build_output(p->children[0]);
            build_output(p->children[1]);
            build_output(p->children[2]);
            build_output(p->children[3]);
        }
    }

public:
    ~QuadTree() {
        if (m_pRoot) destroy(m_pRoot);
    }

    void destroy(Node* p) {
        if (p) {
            destroy(p->children[0]);
            destroy(p->children[1]);
            destroy(p->children[2]);
            destroy(p->children[3]);
            delete p;
        }
    }
    
};