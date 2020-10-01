#include<bits/stdc++.h>
using namespace std;
map<char,string>m;
map<string,char>ma;

class node
{
public:
    int freq;
    char symbol;
    string code;
    node *left,*right;
    node(int f,char s,node *l=NULL,node *r=NULL)
    {
        freq=f;
        symbol=s;
        left=l;
        right=r;
    }
};

string readInputFile(ifstream& in)
{
    stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}
string strToBinary(string s)
{
    int n = s.length();
    string out;
    for (int i = 0; i <n; i++)
    {
        unsigned char ch=s[i];
        //cout<<ch<<" ";
        int val=(int)ch;
        //cout<<val<<" ";
        string bin;
        while (val > 0)
        {
            (val % 2)? bin.push_back('1') :
                       bin.push_back('0');
            val /= 2;
        }
        int m=8-bin.size();
        for(int j=0;j<m;j++)
            bin=bin+"0";
            //cout<<bin<<" "<<endl;
            //reverse(bin.begin(),bin.end());

        out+=bin;
    }
    return out;
}

void getcodes(node * top,string prefix="")
{
    if(top->left==NULL && top->right==NULL)
    {
        top->code=prefix;
        cout<<top->symbol<<"->"<<top->code<<"\n";
        m.insert({top->symbol,prefix});
        ma.insert({prefix,top->symbol});
        return;
    }
    if(top->left!=NULL)
    {
        getcodes(top->left,prefix+"0");


    }
    if(top->right!=NULL)
    {
        getcodes(top->right,prefix+"1");
    }
}
void heapify(vector<node*>&arr, int i)
{
    int n=arr.size();
    int smallest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    if (l < n && arr[l]->freq < arr[smallest]->freq)
        smallest = l;
    if (r < n && arr[r]->freq < arr[smallest]->freq)
        smallest = r;
    if (smallest != i)
    {
        node *t=arr[i];
        arr[i]=arr[smallest];
        arr[smallest]=t;
        heapify(arr, smallest);
    }
}
void buildHeap(vector<node*> &arr,int n)
{
    for(int i=n/2;i>0;i--)
        heapify(arr,i);
}
node* extractMin(vector<node*>&arr)
{
    int heap_size=arr.size();
    if (heap_size <= 0)
        return NULL;
    node *root=arr[0];
    arr[0] = arr[heap_size-1];
    arr.pop_back();
    heapify(arr,0);
    return root;
}
void insertKey(vector<node*>&arr,node* x)
{
    arr.push_back(x);
    int i=arr.size()-1;
    while(i>0 && arr[(i-1)/2]>arr[i])
    {
        swap(arr[i],arr[(i-1)/2]);
        i=(i-1)/2;
    }
}
node* huffmancode(vector<char>c, vector<int>f,int n)
{
    vector<node*> nodes;
    for(int i=0;i<n;i++)
        nodes.push_back(new node(f[i],c[i]));

    //buildHeap(nodes,n);
    node *l,*r,*p;

    for(int i=0;i<n-1;i++)
    {
        l=extractMin(nodes);
        //cout<<"\nLEFT"<<l->symbol<<" "<<l->freq<<"\n";
        r=extractMin(nodes);
        //cout<<"\nRIGHT"<<r->symbol<<" "<<r->freq<<"\n";
        p=new node(l->freq+r->freq,'$',l,r);
        //cout<<p->symbol<<" "<<p->freq<<"\n";
        insertKey(nodes,p);
    }
    p=nodes.front();
    //getcodes(p,"");
    return p;
}
void getBackString(string code,int n)
{
    fstream extracted;
    extracted.open("extracted.txt",ios::out);
    //cout<<endl<<n<<endl;
    //node*curr=root;
    int i=0;
    //cout<<endl;
    string s="";
    while(i<n)
    {
        s+=code[i];

         map<string,char>::iterator it=ma.find(s);
        if(it==ma.end())
        {
            i++;

        }
        else
        {
          extracted<<it->second;
          //cout<<s<<" "<<it->second<<endl;
          s="";
          i++;
        }

    }

    extracted.close();
    return;
}
int main()
{
    ifstream inFile;
    inFile.open("input.txt",ios::in);
    string data,temp;
    data=readInputFile(inFile);
    inFile.close();
    //data="bcada";
    int l=data.size();
    cout<<data<<" "<<l<<endl;
    map<char,int>mp;
    for(int i=0;i<l;i++)
    {
        char ch=data[i];
        map<char,int>::iterator it=mp.find(ch);
        if(it==mp.end())
        {
            mp.insert({ch,1});
        }
        else
        {
            it->second++;
        }
    }
    map<char,int>::iterator it;
    vector<char>keys;
    vector<int>values;
    for(it=mp.begin();it!=mp.end();it++)
    {
        cout<<it->first<<"  "<<it->second<<endl;
        keys.push_back(it->first);
        values.push_back(it->second);
    }
    node* root=huffmancode(keys,values,keys.size());
    getcodes(root);
    ofstream out;
    out.open("out",ios::binary| ios::out);
        bitset<8>f;
        int index=0;
        int msize=0;
    for(int i=0;i<l;i++)
    {
        char ch=data[i];
        int le=m[ch].size();
        msize+=le;
  //      cout<<m[ch]<<"";
        for(int j=0;j<le;j++)
        {
            if(m[ch][j]==49)
            {
                //cout<<m[ch][j];
                f[index]=1;

            }
            index++;
            if(index==8)
            {
                /*for(int j=0;j<8;j++)
                    //cout<<f[j];
                    cout<<" ";*/
                //cout<<endl;
                unsigned int num=f.to_ulong();
                //cout<<num;
                char ch=(char)num;
                out.write(&ch,sizeof(ch));
                index=0;
                f.reset();
            }
        }
    }
    if(index!=0)
    {
        int num=0;
        int b=1;
        for(int k=index-1;k>=0;k--)
        {
            num+=f[k]*b;
            b*=2;
        }
        char ch=(char)num;
        out.write(&ch,sizeof(ch));
        index=0;
        f.reset();
    }
    cout<<endl;
    string buffer,ans;
    out.close();
    fstream compressed;
    compressed.open("out",ios::binary | ios::in);
    compressed>>buffer;
//8    cout<<buffer.size()<<" "<<msize<<endl;
    ans=strToBinary(buffer);
    cout<<ans<<" "<<ans.length()<<msize;

    getBackString(ans,msize);
    return 0;
}
