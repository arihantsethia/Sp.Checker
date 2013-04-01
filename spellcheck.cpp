#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
using namespace std;

ofstream fout;

class node
{
  char val;
  node* sibling;
  node* child; 
  node* parent;
public:
  node()
  {
    sibling=NULL;
    child=NULL;
    parent=NULL;
  }
  char retval() {return this->val;}
  node* retchild() {return this->child;}
  node* retsib() {return this->sibling;}
  node* retparent() {return this->parent;}
  void insertval(char val) {this->val = val;}
  void insertchild(node *child) {this->child = child;}
  void insertsib(node *sibling) {this->sibling = sibling;}
  void insertparent(node *parent) {this->parent = parent;}
  // Add the remainning string after mismatch occurs 
  void add(string s)
  {
    int i;
    node *temp,*prev;
    prev = this;
    for(i=0;i<s.length();i++)
    {
      temp = new node;
      temp->insertval(s[i]);
      prev->insertchild(temp);
      temp->insertparent(prev);
      prev=temp;
    }
    temp = new node;
    temp->insertval('$');
    prev->insertchild(temp);
    temp->insertparent(prev);
  }
};

class tree
{
  node *start;
public:
  tree(string s)
  {
    start = new node;
    string s1=s;
    start->insertval(s[0]);
    start->add(s1.erase(0,1));
  }
  void srchcumadd(string s,int flag)
  {
    int level=0,found;
    node *temp,*prev,*parent;
    string s1=s;
    temp = start;
    parent = NULL;
    // Scans the word character by character, flag decides whether add(flag=0) or search(flag=1)
    while(level!=s.length())
    {
      if(flag)	cout << level << endl;
      found = 0;
      while(temp!=NULL)
      {
	if(temp->retval()==s[level])
	{
	  found=1;
	  break;
	}
	prev=temp;
	temp=temp->retsib();
      }
      if(found)
      {
	if(flag) cout << "if" << endl;
	parent = temp;
	temp=temp->retchild();
      }
      else if(!found && !flag)
      {
	temp=new node();
	temp->insertval(s[level]);
	prev->insertsib(temp);
	temp->insertparent(parent);
	temp->add(s1.erase(0,level+1));
	return;
      }
      else if(!found && flag)
      {
	if(flag) cout << "else if" << endl;
	fout << endl << s << "\nWord Not Found\nPossible Matches\n";
	s1.erase(level,s.length());
	if(parent != NULL)
	suggest(parent->retchild(),s,s1,1);
	return ;
      }
      level++;
    }
    prev = temp;
    while(temp!=NULL)
    {
      if(temp->retval()=='$')
      {
	return;
      }
      temp=temp->retsib();
    }
    if(flag)
    {
      fout << endl << s << "\nWord Not Found\nPossible Matches:\n";
      adsrch(prev,s,s,1,1,5);
    }
    else
    {
      while(prev->retsib()!=NULL)
      {
	prev=prev->retsib();
      }
      temp = new node();
      temp->insertval('$');
      prev->insertsib(temp);
      temp->insertparent(parent);
    }
  }
  // adsrch -> function to further search for termination character '$' if it doesn't exist at the end of string
  int adsrch(node *start,string s0,string s,int flag,int prnt,int prntcnt)
  {
    static int cnt,cnt1;
    static string s1 = s;
    if(flag)	{cnt=0; cnt1=prntcnt;}
    node *temp,*child;
    temp=start;
    while(temp!=NULL)
    {
      child = temp->retchild();
      if(child->retval()=='$')
      {
	if(levdist(s+temp->retval(),s0)<2*s0.length())
	{
	  cnt++;
	  if(prnt && cnt1)
	  {
	    fout << cnt << ". " << s+temp->retval() << "\t" << levdist(s+temp->retval(),s0) << endl;
	    cnt1--;
	  }
	  if(!prntcnt) return 0;
	}
	adsrch(child->retsib(),s0,s+temp->retval(),0,prnt,cnt1);
	temp=temp->retsib();
      }
      else
      {
	adsrch(child,s0,s+temp->retval(),0,prnt,cnt1);
	temp=temp->retsib();
      }
    }
    return cnt;
  }
  // suggest -> function to suggest possible matches for a word that containers characters not present in dictionary
  void suggest(node *start,string s0,string s,int flag)
  {
    int cnt;
    string s1=s;
    if(start->retval() == '$' && start->retsib() != NULL) start=start->retsib();
    else if(start->retval() == '$' && start->retsib() == NULL) 
    {
      start = start->retparent();
      s.erase(s.length()-1,s.length());
    }
    cnt = adsrch(start,s0,s,1,0,1);
    if(cnt>5)
    {
      adsrch(start,s0,s,1,1,5);
    }
    else if(cnt>0 && cnt<6)
    {
      adsrch(start,s0,s,1,1,cnt);
    }
    else
    {
      start = start->retparent();
      if(start == NULL)	return;
      else suggest(start,s0,s1.erase(s.length()-1,s.length()),0);
    }
  }

  int levdist(const std::string &s1, const std::string &s2)
  {
    const size_t m(s1.size());
    const size_t n(s2.size());
  
    if( m==0 ) return n;
    if( n==0 ) return m;
  
    size_t *costs = new size_t[n + 1];
  
    for( size_t k=0; k<=n; k++ ) costs[k] = k;
  
    size_t i = 0;
    for ( std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
    {
      costs[0] = i+1;
      size_t corner = i;
  
      size_t j = 0;
      for ( std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
      {
	size_t upper = costs[j+1];
	if( *it1 == *it2 )
	{
		    costs[j+1] = corner;
	}
	else
	    {
		  size_t t(upper<corner?upper:corner);
	  costs[j+1] = (costs[j]<t?costs[j]:t)+1;
	    }
  
	corner = upper;
      }
    }
  
    size_t result = costs[n];
    delete [] costs;
  
    return result;
  }
};

int main(int argc,char* argv[])
{
  int flag,len,i;
  string name,id,roll,name1,output_file;
  ifstream fin;
  fin.open("dictionary.txt",ios::in);
  getline(fin,name);
  tree T(name);
  while(!fin.eof())
  {
    fin >> name;
    T.srchcumadd(name,0);
  }
  fin.close();
  fin.open(argv[1],ios::in);
  fout.open("output.txt",ios::app);
  while(!fin.eof())
  {
    flag = 1;
    fin >> name;
    cout << name << endl;
    if(name[0]<65 || (name[0]>90 && name[0]<97) || name[0]>122) name.erase(0,1);
    len = name.length();
    if(len && name[len-1]<65 || (name[len-1]>90 && name[len-1]<97) || name[len-1]>122) name.erase(len-1,len);
    if(name[0]=='\'')	flag=0;
    else
    {
      for(int i=0;i<name.length();i++)
      {
	if((name[i]>64 && name[i]<91) || (name[i]>96 && name[i]<123) || name[i]=='\'') name[i] = tolower(name[i]);
	else flag=0;
      }
    }
    if(flag && name.length() && name!="a" && name!="i") {cout<<name; T.srchcumadd(name,1);}
  }
  fin.close();
  fout.close();
  return 0;
}