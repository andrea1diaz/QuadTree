#include <iostream>
#include <string>
#define cimg_use_jpeg 1
#include "CImg.h"

using namespace cimg_library;
using namespace std;

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

public:     
    QuadTree(CImg<char> img) : m_pRoot(0) {
        this->img = img;
        Node *r = new Node(0, 0, img.width() - 1, img.height() - 1);
        m_pRoot = r;
        // CImg<char> tmp (img.width(), img.height(), 1, 0);
        output = img;
        // cout << img.width() - 1 << "|" << img.height() - 1 << endl;
        build(0, 0, img.width() - 1, img.height() - 1, m_pRoot);
        build_output(m_pRoot);
        output.display();
    }
    

    /*void Print (Node* r) {
        if(!r) return;	   
        cout<<"("<<r->x<<" "<<r->y<<") -- > ";
        cout<<endl;
                    
        for(int i =0;i<4;i++) {
            Print(r->children[i]);
        }      
    }*/

   
private:
    Node * m_pRoot;
    CImg<char> img;

    void build(int x1, int y1, int x2, int y2, Node* root) {
        if (check(x1, y1, x2, y2)) {
            root->color = img(x1,y1);
            root->isLeaf = true;
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