// implementation of B-tree in C++ with Insertion , deletion , Searching and traversal of the tree

#include <iostream>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
using namespace std;
#define ATLEAST 2 
#define ATMOST 4

 
struct BTnodeStruct {
    int val[ATMOST + 1], TotalAddup;
    BTnodeStruct *NextPointerArr[ATMOST + 1];
};
 
BTnodeStruct *BTrootNode;
 

BTnodeStruct * createNode(int val, BTnodeStruct *BTchildNode) {
    BTnodeStruct *FreshNode = new BTnodeStruct;
    FreshNode->val[1] = val;
    FreshNode->TotalAddup = 1;
    FreshNode->NextPointerArr[0] = BTrootNode;
    FreshNode->NextPointerArr[1] = BTchildNode;
    return FreshNode;
}
 

void addElementToNode(int val, int Location, BTnodeStruct *node, BTnodeStruct *BTchildNode) {
    int j = node->TotalAddup;
    while (j > Location) {
        node->val[j + 1] = node->val[j];
        node->NextPointerArr[j + 1] = node->NextPointerArr[j];
        j--;
    }
    node->val[j + 1] = val;
    node->NextPointerArr[j + 1] = BTchildNode;
    node->TotalAddup++;
}
 

void BTnodeSplit(int val, int *pval, int Location, BTnodeStruct *node,BTnodeStruct *BTchildNode, BTnodeStruct **FreshNode) {
    int median, j;
 
    if (Location > ATLEAST)
        median = ATLEAST + 1;
    else
        median = ATLEAST;
 
    *FreshNode = new BTnodeStruct;
    j = median + 1;
    while (j <= ATMOST) {
        (*FreshNode)->val[j - median] = node->val[j];
        (*FreshNode)->NextPointerArr[j - median] = node->NextPointerArr[j];
        j++;
    }
    node->TotalAddup = median;
    (*FreshNode)->TotalAddup = ATMOST - median;
 
    if (Location <= ATLEAST) {
        addElementToNode(val, Location, node, BTchildNode);
    }
    else {
        addElementToNode(val, Location - median, *FreshNode, BTchildNode);
    }
    *pval = node->val[node->TotalAddup];
    (*FreshNode)->NextPointerArr[0] = node->NextPointerArr[node->TotalAddup];
    node->TotalAddup--;
}
 

int InsertElementInNode(int val, int *pval,BTnodeStruct *node, BTnodeStruct **BTchildNode) {
 
    int Location;
    if (!node) {
        *pval = val;
        *BTchildNode = NULL;
        return 1;
    }
 
    if (val < node->val[1]) {
        Location = 0;
    }
    else {
        for (Location = node->TotalAddup;
            (val < node->val[Location] && Location > 1); Location--);
        if (val == node->val[Location]) {
            cout<<"Duplicate elements are not allowed\n";
            return 0;
        }
    }
    if (InsertElementInNode(val, pval, node->NextPointerArr[Location], BTchildNode)) {
        if (node->TotalAddup < ATMOST) {
            addElementToNode(*pval, Location, node, *BTchildNode);
        }
        else {
            BTnodeSplit(*pval, pval, Location, node, *BTchildNode, BTchildNode);
            return 1;
        }
    }
    return 0;
}
 

void Fun_BtreeInsertion(int val) {
    int TempFlag, i;
    BTnodeStruct *BTchildNode;
 
    TempFlag = InsertElementInNode(val, &i, BTrootNode, &BTchildNode);
    if (TempFlag)
        BTrootNode = createNode(i, BTchildNode);
}
 

void copySuccessor(BTnodeStruct *BTnode, int Location) {
    BTnodeStruct *tempN;
    tempN = BTnode->NextPointerArr[Location];
 
    for (; tempN->NextPointerArr[0] != NULL;)
        tempN = tempN->NextPointerArr[0];
    BTnode->val[Location] = tempN->val[1];
 
}
 

void fun_deleteValue(BTnodeStruct *BTnode, int Location) {
    int i = Location + 1;
    while (i <= BTnode->TotalAddup) {
        BTnode->val[i - 1] = BTnode->val[i];
        BTnode->NextPointerArr[i - 1] = BTnode->NextPointerArr[i];
        i++;
    }
    BTnode->TotalAddup--;
}
 

void BTreeRightShift(BTnodeStruct *BTnode, int Location) {
    BTnodeStruct *x = BTnode->NextPointerArr[Location];
    int j = x->TotalAddup;
 
    while (j > 0) {
        x->val[j + 1] = x->val[j];
        x->NextPointerArr[j + 1] = x->NextPointerArr[j];
    }
    x->val[1] = BTnode->val[Location];
    x->NextPointerArr[1] = x->NextPointerArr[0];
    x->TotalAddup++;
 
    x = BTnode->NextPointerArr[Location - 1];
    BTnode->val[Location] = x->val[x->TotalAddup];
    BTnode->NextPointerArr[Location] = x->NextPointerArr[x->TotalAddup];
    x->TotalAddup--;
    return;
}
 

void BTreeLeftShift(BTnodeStruct *BTnode, int Location) {
    int j = 1;
    BTnodeStruct *x = BTnode->NextPointerArr[Location - 1];
 
    x->TotalAddup++;
    x->val[x->TotalAddup] = BTnode->val[Location];
    x->NextPointerArr[x->TotalAddup] = BTnode->NextPointerArr[Location]->NextPointerArr[0];
 
    x = BTnode->NextPointerArr[Location];
    BTnode->val[Location] = x->val[1];
    x->NextPointerArr[0] = x->NextPointerArr[1];
    x->TotalAddup--;
 
    while (j <= x->TotalAddup) {
        x->val[j] = x->val[j + 1];
        x->NextPointerArr[j] = x->NextPointerArr[j + 1];
        j++;
    }
    return;
}
 

void mergeBTnode(BTnodeStruct *BTnode, int Location) {
    int j = 1;
    BTnodeStruct *x1 = BTnode->NextPointerArr[Location], *x2 = BTnode->NextPointerArr[Location - 1];
 
    x2->TotalAddup++;
    x2->val[x2->TotalAddup] = BTnode->val[Location];
    x2->NextPointerArr[x2->TotalAddup] = BTnode->NextPointerArr[0];
 
    while (j <= x1->TotalAddup) {
        x2->TotalAddup++;
        x2->val[x2->TotalAddup] = x1->val[j];
        x2->NextPointerArr[x2->TotalAddup] = x1->NextPointerArr[j];
        j++;
    }
 
    j = Location;
    while (j < BTnode->TotalAddup) {
        BTnode->val[j] = BTnode->val[j + 1];
        BTnode->NextPointerArr[j] = BTnode->NextPointerArr[j + 1];
        j++;
    }
    BTnode->TotalAddup--;
    free(x1);
}
 

void adjustBTnode(BTnodeStruct *BTnode, int Location) {
    if (!Location) {
        if (BTnode->NextPointerArr[1]->TotalAddup > ATLEAST) {
            BTreeLeftShift(BTnode, 1);
        }
        else {
            mergeBTnode(BTnode, 1);
        }
    }
    else {
        if (BTnode->TotalAddup != Location) {
            if (BTnode->NextPointerArr[Location - 1]->TotalAddup > ATLEAST) {
                BTreeRightShift(BTnode, Location);
            }
            else {
                if (BTnode->NextPointerArr[Location + 1]->TotalAddup > ATLEAST) {
                    BTreeLeftShift(BTnode, Location + 1);
                }
                else {
                    mergeBTnode(BTnode, Location);
                }
            }
        }
        else {
            if (BTnode->NextPointerArr[Location - 1]->TotalAddup > ATLEAST)
                BTreeRightShift(BTnode, Location);
            else
                mergeBTnode(BTnode, Location);
        }
    }
}
 

int Fun_RemoveValFrmNode(int val,BTnodeStruct *BTnode) {
    int Location, TempFlag = 0;
    if (BTnode) {
        if (val < BTnode->val[1]) {
            Location = 0;
            TempFlag = 0;
        }
        else {
            for (Location = BTnode->TotalAddup;
                (val < BTnode->val[Location] && Location > 1); Location--);
            if (val == BTnode->val[Location]) {
                TempFlag = 1;
            }
            else {
                TempFlag = 0;
            }
        }
        if (TempFlag) {
            if (BTnode->NextPointerArr[Location - 1]) {
                copySuccessor(BTnode, Location);
                TempFlag = Fun_RemoveValFrmNode(BTnode->val[Location], BTnode->NextPointerArr[Location]);
                if (TempFlag == 0) {
                    cout<<"Given data is not present in B-Tree\n";
                }
            }
            else {
                fun_deleteValue(BTnode, Location);
            }
        }
        else {
            TempFlag = Fun_RemoveValFrmNode(val, BTnode->NextPointerArr[Location]);
        }
        if (BTnode->NextPointerArr[Location]) {
            if (BTnode->NextPointerArr[Location]->TotalAddup < ATLEAST)
                adjustBTnode(BTnode, Location);
        }
    }
    return TempFlag;
}
 

void deletion(int val,BTnodeStruct *BTnode) {
    BTnodeStruct *tmp;
    if (!Fun_RemoveValFrmNode(val, BTnode)) {
        cout<<"Given value is not present in B-Tree\n";
        return;
    }
    else {
        if (BTnode->TotalAddup == 0) {
            tmp = BTnode;
            BTnode = BTnode->NextPointerArr[0];
            free(tmp);
        }
    }
    BTrootNode = BTnode;
    return;
}
 

void Fun_BtreeSearch(int val, int *Location,BTnodeStruct *BTnode) {
    if (!BTnode) {
        return;
    }
 
    if (val < BTnode->val[1]) {
        *Location = 0;
    }
    else {
        for (*Location = BTnode->TotalAddup;
            (val < BTnode->val[*Location] && *Location > 1); (*Location)--);
        if (val == BTnode->val[*Location]) {
            cout << "Given key Found!\n";
            return;
        }else{
            cout << "Given Key Not Found!";
        }
    }
    Fun_BtreeSearch(val, Location, BTnode->NextPointerArr[*Location]);
    return;
}
 

void Btree_Traversal(BTnodeStruct *BTnode) {
    int i;
    if (BTnode) {
        for (i = 0; i < BTnode->TotalAddup; i++) {
            Btree_Traversal(BTnode->NextPointerArr[i]);
            cout<< BTnode->val[i + 1]<<' ';
        }
        Btree_Traversal(BTnode->NextPointerArr[i]);
    }
}
 
int main() {
    int element, choice;
    while (true) {
        cout<<"\nPress 1 for Insertion in B-Tree \nPress 2 for Deletion in B-Tree \nPress 3 for Searching in B-Tree \nPress 4 for Traversal in B-Tree \n";
        cout<<"Press 5 : Exit \n";
        cin >> choice;
        cout << endl;
        switch (choice) {
        case 1:
            cout<<"Enter the value to insert :\t";
            cin >> element;
            Fun_BtreeInsertion(element);
			cout<<"Inserted Successfully!\n";
			cout<<"B-Tree : ";
			Btree_Traversal(BTrootNode);
			cout<<"\n___________________________________________\n";
            break;
        case 2:
            cout<<"Enter the value to delete:\t";
            cin >> element;
            deletion(element, BTrootNode);
            cout<<"Deleted Successfully!\n";
            cout<<"B-Tree : ";
            Btree_Traversal(BTrootNode);
            cout<<"\n___________________________________________\n";
            break;
        case 3:
            cout<<"Enter the value to search:\t";
            cin >> element;
            cout<<"Searching Done! :\t";
            Fun_BtreeSearch(element, &choice, BTrootNode);
            cout<<"\n___________________________________________\n";
            break;
        case 4:
        	cout<<"Traversing...\nB-Tree : ";
            Btree_Traversal(BTrootNode);
            cout<<"\n___________________________________________\n";
            break;
        case 5:
            cout<<"Program Terminated !";
            exit(0);
        }
        cout << endl;
    }
 
    system("pause");

return 0;
}