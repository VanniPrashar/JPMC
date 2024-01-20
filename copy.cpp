#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <queue>


using namespace std;



void print_vec( vector<std::vector<double> > in_vec){
    for (auto row : in_vec){
        for (auto val : row){
            cout << val << " "; 
        }
        cout << endl;
    }
}

void print_vec( vector<std::vector<int> > in_vec){
   
    for (auto row : in_vec){
        cout <<"( ";
        for (auto val : row){
            cout << val <<" "; 
        }
        cout <<") ";
    }
    cout<<endl;
}

void print_vec( vector<int> in_vec){
    for (auto row : in_vec){
        cout << row << " ";
    }

        cout << endl;
}




struct Cluster{
    vector<int> nodes;
    Cluster* left=NULL;
    Cluster* right=NULL;
};

struct TreeCLuster {
    Cluster* left;
    Cluster* right;
};


vector<int> mergeClusters(Cluster *a , Cluster *b){
    vector<int> mergedCluster;
    for (auto x : a->nodes){
        mergedCluster.push_back(x);
    }
    for (auto x : b->nodes){
        mergedCluster.push_back(x);
    }
     sort(mergedCluster.begin() , mergedCluster.end());
     return mergedCluster;
}


Cluster* solver(vector< vector<double> > &data , vector<Cluster*> clusters ){
    if(clusters.size() == 1){
        return clusters[0];
    }


    double mini = 1000000;
    int min_index_i = -1;
    int min_index_j = -1;
    for (int i = 0 ; i < data.size() ; i++){
        for (int j = 0 ; j < i ; j++ ){
            if (data[i][j] < mini){
                mini = data[i][j];
                min_index_i = i;
                min_index_j = j;
            }
        }
    }
    vector <Cluster*> new_cluster;
    for (int i = 0 ; i< clusters.size() ; i++){
        if (i != min_index_i && i != min_index_j){
            new_cluster.push_back(clusters[i]);
        } else if ( i == min_index_j){
            Cluster *p = new Cluster();
            p->nodes = mergeClusters(clusters[i] , clusters[min_index_i]);
            p->left = clusters[i];
            p->right = clusters[min_index_i];
            // clusters[i].nodes = 
            new_cluster.push_back(p);
        } 
    }

    vector < vector < double > > new_data;

    for (int i = 0 ; i < data.size() ; i++){
        if(i==min_index_i){
            continue;
        }
        else if (i == min_index_j){
            for (int j = 0 ; j < data.size() ; j++ ){
                data[i][j] = min(data[i][j], data[min_index_i][j]);
            }
        }
        else {
            for (int j = 0 ; j < data.size() ; j++ ){
                if(j== min_index_i || j == min_index_j){
                    data[i][j] = min(data[i][min_index_i], data[i][min_index_j]);
                }
            }
        }
    }


    for (int i = 0 ; i < data.size() ; i++){
        if ( i != min_index_i){
            vector <double> temp;
            for (int j = 0 ; j< data.size() ; j++){
                if (j != min_index_i){
                    temp.push_back(data[i][j]);
                }
            }
            new_data.push_back(temp);
        }
    }
    print_vec(new_data);
    cout << "\n\n";
    return solver( new_data , new_cluster);
}

map<int,vector< vector <int> > > print_dendogram(Cluster* head){
    map<int,vector< vector <int> > > levelCluster;
    queue< pair<Cluster* , int> > q;
    q.push(make_pair(head,0));
    while (!q.empty())
    {
        auto p = q.front();
        q.pop();
        int l = p.second;
        Cluster* curr = p.first;
        levelCluster[l].push_back(curr -> nodes);
        if (curr->left != NULL){
            q.push(make_pair(curr->left,l+1));
        } 
        if (curr->right){
            q.push(make_pair(curr->right,l+1));
        }
    }
    return levelCluster;
    
}




int main() {
    ifstream inputFile("inp.txt"); // Replace "filename.txt" with the actual file name

    vector<vector <double> > data;
    vector<Cluster*> clusters;

    string s;
    bool first_row= true;
    while(inputFile >> s){
        if(first_row){
            first_row = false;
            continue;
        }
        // cout<<s<<"\n";
        std::vector<double> row;
        std::istringstream iss(s);  
        std::string value;
        bool firstColumn = true;

        while (std::getline(iss, value, ',')) {
            if (firstColumn) {
                firstColumn = false;
                continue; // Skip the first column
            }
            row.push_back(std::stod(value));
        }

        data.push_back(row);
    }

    for (int i = 0; i < data.size() ;i++){
        Cluster *temp = new Cluster();
        temp->nodes.push_back(i);
        clusters.push_back(temp);
    }
    Cluster* head = solver(data,clusters);
    print_vec(head->left->left->nodes);
    print_vec(head->right->right->nodes);

    auto map_level = print_dendogram(head);
    for (auto x: map_level){
        sort(x.second.begin(),x.second.end());
        print_vec(x.second);
    }


    inputFile.close();

}

