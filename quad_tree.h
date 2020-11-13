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
    
    Node (int x1, int y1, int x2, int y2) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;

        for (int i = 0; i < 4; i++) children[i] = 0;
    }      
};

class Quadtree {
public:     
    Quadtree(CImg<char> img) : m_pRoot(0) {
        this->img = img;
        m_pRoot->x1 = 0;
        m_pRoot->y1 = 0;
        m_pRoot->x2 = img.width()-1;
        m_pRoot->y2 = img.height()-1;

        build(0, 0, img.width() - 1, img.height() - 1, m_pRoot);
    }
    
    void Insert(int x, int y, string name, Node* p){          
        if (!p){
            p = new Node(x, y, name);
            return;
        }
        
        Insert(x, y, name, p->children[cuadrante(x, y, p)]);
        
    }

    void Print (Node* r) {
        if(!r) return;	   
        cout<<"("<<r->x<<" "<<r->y<<") -- > ";
        cout<<endl;
                    
        for(int i =0;i<4;i++) {
            Print(r->children[i]);
        }      
    }

   
private:
    Node * m_pRoot;
    CImg<char> img;

    void build(int x1, int y1, int x2, int y2, Node* root) {
        if (check(x1, y1, x2, y2)) {
            root->color = img(x1,y1);
            return;
        }
        
        int x_m = (x2 - x1) / 2;
        int y_m = (y2 - y1) / 2;
        
        root->children[0] = new Node (x1, y1, x_m, y_m);
        build(x1, y1, x_m, y_m, root->children[0]);

        root->children[1] = new Node(x_m, y1, x2, y_m);
        build(x_m, y1, x2, y_m, root->children[1]);
         
        root->children[2] = new Node (x1, y_m, x_m, y2);
        build(x1, y_m, x_m, y2, root->children[2]);

        root->children[3] = new Node(x_m, y_m, x2, y2);
        build(x_m, y_m, x2, y2, root->children[3]);
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


    
};