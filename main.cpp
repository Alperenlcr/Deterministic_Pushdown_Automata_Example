#include <bits/stdc++.h>

using namespace std;

bool ended_with_Q_we_want(string first_con, vector<string> start_end)
{
    int i;
    for ( i = 1; i < start_end.size(); i++)
        if (first_con == start_end[i])
            return true;
    return false;
}

void read_files(char dpda_file[50], char input_file[50], vector<string> &Q, vector<string> &start_end, string &A, string &Z, vector<vector<string>> &T, vector<string> &inputs)
{
    //read input
    char line[20];
    ifstream f(input_file);
    int i = 0;
    int c4 = 0;
    while (f.getline(line, 20))
    {
        string li(line);
        li.erase(remove(li.begin(), li.end(), ','), li.end());      //remove , from string
        li.erase(remove(li.begin(), li.end(), '\r'), li.end());      //remove '\r' from string
        inputs.push_back(li);
    }

    //read dpda
    char line2[100];
    ifstream f2(dpda_file);
	char *ptr, *ptr2, *ptr3;
    i = 0;
    int c_row = 0;
    
    while (f2.getline(line2, 100))      // line by line
    {
        string li(line2);
        if (c_row == 0)
        {
            int c = 0;
            string token1;
            stringstream s(li);
            while(s >> token1)
            {
                istringstream ss(token1);
                string token2;

                while(getline(ss, token2, ':'))
                {
                    if(c==1)
                    {
                        istringstream s(token2);
                        string token3;
                        while(getline(s, token3, ','))
                            Q.push_back(token3);
                    }
                    else if(c==3)
                    {
                        istringstream s(token2);
                        string token3;
                        while(getline(s, token3, ','))
                        {
                            token3.pop_back();
                            token3.erase(token3.begin());
                            start_end.push_back(token3);
                        }
                    }
                    c++;
                }
            }
        }
        else if (c_row == 1)
        {
            int c1 = 0;
            string token1;
            istringstream s(li);
            while(getline(s, token1, ':'))
            {
                if(c1==1)
                {
                    istringstream ss(token1);
                    string token2;
                    while(getline(ss, token2, ','))
                        A.push_back(token2[0]);
                }
                c1 ++;
            }
        }
        else if (c_row == 2)
        {
            int c = 0;
            string token1;
            istringstream s(li);
            while(getline(s, token1, ':'))
            {
                if(c==1)
                {
                    istringstream ss(token1);
                    string token2;
                    while(getline(ss, token2, ','))
                        Z.push_back(token2[0]);
                }
                c ++;
            }
        }
        else
        {
            int c = 0;
            string token1;
            istringstream s(li);
            while(getline(s, token1, ':'))
            {
                if(c==1)
                {
                    istringstream ss(token1);
                    string token2;
                    T.push_back(vector<string>());
                    while(getline(ss, token2, ','))
                        T[c_row-3].push_back(token2);
                }
                c ++;
            }
        }
        c_row ++;
    }
}

void solution(char output_file[50], vector<string> &Q, vector<string> &start_end, string &A, string &Z, vector<vector<string>> &T, vector<string> &inputs)
{
// checking variables valid
    bool valid = true;
    int i, j, len, equ;
    ofstream fp(output_file);
    for ( i = 0; i < T.size(); i++)
    {
        equ = 0;
        for ( j = 0; j < Q.size(); j++)
        {
            if (Q[j] == T[i][0])
                equ += 1;
            if (Q[j] == T[i][3])
                equ += 1;
        }
        for ( j = 0; j < A.size(); j++)
        {
            if (A[j] == T[i][1][0] || "e" == T[i][1])
            {
                equ += 1;
                break;
            }
        }
        for ( j = 0; j < Z.size(); j++)
        {
            if (Z[j] == T[i][4][0] || "e" == T[i][4])
            {
                equ += 1;
                break;
            }
        }
        if (equ != 4)
        {
            valid = false;
        }
    }
// end

// if not valid then write to file and exit function
    if (valid == false)
    {
        fp << "Error [1]:DPDA description is invalid!";
        fp.close();
        return;
    }
// end

// if valid we'll solve
    int k;
    for ( k = 0; k < inputs.size(); k++)
    {
        stack<char> s;
        string first_con=start_end[0], third_con="e", fourth_con;   // these are values we'll compare
        string second_con(1, inputs[k][0]);
        vector<int> possible_indexes;       // every compare process we'll chose from T and contuine with it for that loop
        int countt = 0;     // this is input index counter
        while (!(countt == inputs[k].size() && ended_with_Q_we_want(first_con, start_end)))        // we should loop until input achieved and ended with Q we want     or no possible solutions
        {
            for ( i = 0; i < T.size(); i++)     // looking for state_number
                if (T[i][0] == first_con)
                    possible_indexes.push_back(i);

            for ( i = possible_indexes.size()-1; i >= 0; i--)       // looking for input order
                if (T[possible_indexes[i]][1] != second_con && T[possible_indexes[i]][1] != "e")
                    possible_indexes.erase(possible_indexes.begin()+i);

            for ( i = possible_indexes.size()-1; i >= 0; i--)       // looking for element which will be popped from stack
                if (T[possible_indexes[i]][2] != third_con && T[possible_indexes[i]][2] != "e")
                    possible_indexes.erase(possible_indexes.begin()+i);

            if (possible_indexes.size() == 0)   // no possible solutions
                break;

            i = possible_indexes[0];

    // stack pop-push
            if (T[i][2] != "e")
                s.pop();

            if (T[i][4] != "e")
            {
                s.push(T[i][4][0]);
                third_con = s.top();
            }
            else if (s.size() != 0)
                third_con = s.top();
            else
                third_con = "e";
    // end

    // file write
            for ( j = 0; j < T[i].size(); j++)
            {
                fp << T[i][j];
                if (j == 2)
                    fp << " => ";
                else if (j == 4)
                    fp << " [STACK]:";
                else
                    fp << ",";
            }
            stack<char> temp;
            while (s.empty() == false)
            {
                temp.push(s.top());
                s.pop();
            }  
        
            while (temp.empty() == false)
            {
                char t = temp.top();
                if (temp.size() == 1)
                    fp << t;
                else
                    fp << t << ",";
                temp.pop();
                s.push(t);      // To restore contents of the original stack. 
            }
            fp << "\n";
    // end

    // modify con. variables
            first_con = T[possible_indexes[0]][3];
            if (T[possible_indexes[0]][1] != "e")
            {
                countt ++;
                string temp(1, inputs[k][countt]);
                second_con = temp;
            }
            possible_indexes.clear();
    // end
    }
    if ((countt == inputs[k].size() && ended_with_Q_we_want(first_con, start_end)))
        fp << "ACCEPT\n\n";
    else
        fp << "REJECT\n\n";

//end
    }
    fp.close();
}

int main()
{
    char file_names[5][3][50] = {
        {"sample1/dpda1.txt","sample1/dpda-input1.txt","sample1/my_output1.txt"},
        {"sample2/dpda2.txt","sample2/dpda-input2.txt","sample2/my_output2.txt"},
        {"sample3/dpda3-1.txt","sample3/dpda-input3-1.txt","sample3/my_output3-1.txt"},
        {"sample3/dpda3-2.txt","sample3/dpda-input3-2.txt","sample3/my_output3-2.txt"},
        {"sample3/dpda3-3.txt","sample3/dpda-input3-3.txt","sample3/my_output3-3.txt"}
    };
    int i = 0, j = 0;
    for ( i = 0; i < 5; i++)
    {
        vector<string> Q;
        vector<string> start_end;
        string A, Z;
        vector<string> inputs;
        vector<vector<string>> T;
        read_files(file_names[i][0], file_names[i][1], Q, start_end, A, Z, T, inputs);  //read files and fills variables
        solution(file_names[i][2], Q, start_end, A, Z, T, inputs);      // process and output as a file
    }
    cout << "\nProgram successed !\n";
    return 0;
}
