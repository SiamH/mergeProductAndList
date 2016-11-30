// Program to print BFS traversal from a given source vertex. BFS(int s)
// traverses vertices reachable from s.
#include <iostream>
#include <list>
#include <vector>
#include "assert.h"
#include <fstream>
#include <sstream>
#include <string>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <fstream>


using namespace std;

struct Product
{
   unsigned int manuid;
   string ProductName;  

   bool operator<(const Product& other) const
   {
    return (manuid < other.manuid);
   }
};

struct Listing
{   
    unsigned int manuid;
    string fulllist;

    bool operator<(const Listing& other) const{
        return (manuid < other.manuid);
    }
};


vector<int> GetIndicesToFecth(vector<Listing>& list)
{
  vector<int> result;
  int index = 0;
  result.push_back(index);

  for (int  i = 0; i < list.size() - 1; i++)
  {
    if (list[i].manuid == list[i + 1].manuid)
      list[index].fulllist = list[index].fulllist + '\n' + list[i + 1].fulllist;
    else
    {
      index = i + 1;
      result.push_back(index);
    }
  }

  return result;
}


class Parser
{
    bool m_bProduct;
    Product m_Prod;
    Listing m_List;

    unsigned int hash(string s)
    {
        unsigned int h = 0;
        for (int i = 0; i < s.length(); i++)
           h = h << 1 ^ s[i];

        return h;
    }

    struct RemoveDelimiter
    {
      bool operator()(char c)
      {
        return (c =='{' || c =='}' || c == '"');
      }
    };



    string GetToken(string s, int index)
    {
      string res;
      while (s[index] != ',')
      {
        res += s[index];
        index++;
      }

      return res;
    }

    void GetProductName(string s, string& rprod, string& rman)
    {

      s.erase(remove_if(s.begin(), s.end(), RemoveDelimiter()), s.end());

      string man = "manufacturer:";
      string prod = "product_name:";
      int lengthm = man.length();
      int lengthp = prod.length();
      int i = 0;
      while (true)
      {
        if (i > s.length())
          break;

        string match1 = s.substr(i, lengthm);
        string match2 = s.substr(i, lengthp);
        if (match1.compare(man) == 0)
        {
          i += lengthm;
          rman = GetToken(s, i);
          return;
        }
        else if (match2.compare(prod) == 0)
        {
          i+= lengthp;
          rprod = GetToken(s, i);
        }

        i++;
      }
    }


public:
    Parser(bool bProduct)
    {
        m_bProduct = bProduct;
    }

    void Parse(string s)
    {
        string prod, man;
        GetProductName(s, prod, man);
        if (m_bProduct)
        {
            m_Prod.manuid = hash(man);
            m_Prod.ProductName = prod;
        }
        else
        {
            m_List.manuid = hash(man);
            m_List.fulllist = s;
        }

    }

    Product GetProduct() const
    {
        assert(m_bProduct);
        return m_Prod;
    }

    Listing GetList() const
    {
        assert(!m_bProduct);
        return m_List;
    }

};

void PopulateProducts(vector<Product>& avProds)
{
    string line;

    Parser vParser(true);

    ifstream infile1("products.txt");

    while (getline(infile1, line))
    {
        vParser.Parse(line);
        avProds.push_back(vParser.GetProduct());
    } 

    infile1.close();
}

void PopulateList(vector<Listing>& avLists)
{
    string line;

    Parser vParser(false);

    ifstream infile2("listings.txt");

    while (getline(infile2, line))
    {
        vParser.Parse(line);
        avLists.push_back(vParser.GetList());
    }  

    infile2.close();
}

string CreateFormat(string s, string t)
{
    string res = "{\"product_name\" : " + s + " , ";
    res += "\"listings\" : " + t + "}";

    return res;
}


// Driver program to test methods of graph class
int main()
{
    vector<Product> avProducts;
    PopulateProducts(avProducts);

    vector<Listing> avLists;
    PopulateList(avLists);     

    sort(avProducts.begin(), avProducts.end());
    sort(avLists.begin(), avLists.end());
    vector<int> Result = GetIndicesToFecth(avLists);


    int i = 0; 
    int j = 0;

    ofstream outfile("Result.txt");
    while (i < avProducts.size())
    {
        if (j >= Result.size())
             break;

        if (avProducts[i].manuid == avLists[Result[j]].manuid)
        {
        	outfile << CreateFormat(avProducts[i].ProductName, avLists[Result[j]].fulllist) << endl;
        	i++;
        }
       	else if (avProducts[i].manuid < avLists[Result[j]].manuid)
       		i++;
       	else
       		j++;
    }

    return 0;
}
